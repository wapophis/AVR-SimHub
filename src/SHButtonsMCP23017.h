#ifndef __SHBUTTONSMCP23017_H__
#define __SHBUTTONSMCP23017_H__

#include <Arduino.h>
#include "Adafruit_MCP23X17.h"
#include "SHDebouncer.h"

typedef void(*SHMcpButtonChanged) (int, byte);

class SHButtonsMCP23017 {
private:
	Adafruit_MCP23X17 *mcps[8];  // Array de hasta 8 MCP23017
	byte mcpCount;                // Número de MCPs activos
	byte totalButtonsCount;       // Total de botones en todos los MCPs
	byte * buttonIds;             // IDs de todos los botones
	byte * mcpPins;               // Pines de todos los botones
	bool * vccToPinWirings;       // Configuración de cableado
	int * logicModes;            // Modos de lógica
	byte * lastStates;            // Estados anteriores
	unsigned long * lastChangedAt; // Timestamps de cambio
	SHMcpButtonChanged callback;
	SHDebouncer debouncer;

	int toLogicalState(bool vccToPinWiring, int logicMode, int rawValue) {
		int res = 0;
		if (!vccToPinWiring) {
			res = rawValue == HIGH ? 0 : 1;
		}
		else {
			res = rawValue == HIGH ? 1 : 0;
		}
		if (logicMode == 1) {
			res = res ? 0 : 1;
		}
		return res;
	}

public:
	void begin(Adafruit_MCP23X17 **mcps, byte mcpCount, byte totalButtonsCount, 
	           byte *buttonIds, byte *mcpPins, bool *vccToPinWirings, int *logicModes, 
	           SHMcpButtonChanged changedCallback) {
		
		this->mcpCount = mcpCount;
		this->totalButtonsCount = totalButtonsCount;
		this->buttonIds = buttonIds;
		this->mcpPins = mcpPins;
		this->vccToPinWirings = vccToPinWirings;
		this->logicModes = logicModes;
		this->callback = changedCallback;

		// Copiar punteros a MCPs
		for (int i = 0; i < mcpCount; i++) {
			this->mcps[i] = mcps[i];
		}

		debouncer.begin(10);

		lastStates = (byte*)malloc(totalButtonsCount * sizeof(byte));
		lastChangedAt = (unsigned long*)malloc(totalButtonsCount * sizeof(unsigned long));

		// Configurar todos los botones
		for (int btnIdx = 0; btnIdx < totalButtonsCount; btnIdx++) {
			byte pin = mcpPins[btnIdx];
			byte mcpIndex = pin / 16;  // Determinar qué MCP basado en el pin
			byte localPin = pin % 16;  // Pin local dentro del MCP
			
			if (mcpIndex < mcpCount) {
				if (!vccToPinWirings[btnIdx]) {
					mcps[mcpIndex]->pinMode(localPin, INPUT_PULLUP);
				}
				else {
					mcps[mcpIndex]->pinMode(localPin, INPUT);
				}
				
				int initial = toLogicalState(vccToPinWirings[btnIdx], logicModes[btnIdx], 
				                           mcps[mcpIndex]->digitalRead(localPin));
				lastStates[btnIdx] = initial;
				lastChangedAt[btnIdx] = millis();
				if (initial) {
					callback(buttonIds[btnIdx], initial);
				}
			}
		}
	}

	void read() {
		if (!debouncer.Debounce()) return;
		
		for (int btnIdx = 0; btnIdx < totalButtonsCount; btnIdx++) {
			byte pin = mcpPins[btnIdx];
			byte mcpIndex = pin / 16;  // Determinar qué MCP basado en el pin
			byte localPin = pin % 16;  // Pin local dentro del MCP
			
			if (mcpIndex < mcpCount) {
				int state = toLogicalState(vccToPinWirings[btnIdx], logicModes[btnIdx], 
				                          mcps[mcpIndex]->digitalRead(localPin));
				if (state != lastStates[btnIdx] && lastChangedAt[btnIdx] - millis() > 50) {
					callback(buttonIds[btnIdx], state);
					lastStates[btnIdx] = state;
					lastChangedAt[btnIdx] = millis();
				}
			}
		}
	}
};

#endif 