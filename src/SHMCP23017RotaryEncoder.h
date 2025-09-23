#ifndef __SHMCP23017ROTARYENCODER_H__
#define __SHMCP23017ROTARYENCODER_H__

#include <Arduino.h>
#include "Adafruit_MCP23X17.h"
#include "SHDebouncer.h"

typedef void(*SHMCP23017RotaryEncoderPositionChanged) (int, int, byte);

class SHMCP23017RotaryEncoder {
private:
    Adafruit_MCP23X17 *mcp;           // Puntero al MCP23017
    byte mcpIndex;                    // Índice del MCP (0-7)
    byte clkPin;                      // Pin CLK en el MCP23017 (0-15)
    byte dtPin;                       // Pin DT en el MCP23017 (0-15)
    byte buttonPin;                   // Pin del botón en el MCP23017 (0-15, -1 si no hay)
    
    int counter = 0;
    bool halfSteps = false;
    bool reverseDirection = false;
    bool enablePullup = true;
    
    uint8_t inputLastState;
    uint8_t buttonState;
    int buttonLastState;
    unsigned long lastbuttonchange;
    unsigned long positionLastChanged;
    unsigned long directionLastChange = 255;
    SHDebouncer buttonDebouncer;
    SHDebouncer encoderDebouncer;
    uint8_t direction;
    
    byte id;
    SHMCP23017RotaryEncoderPositionChanged positionChangedCallback;
    
    // Constantes de dirección
    static const uint8_t R_START = 0x0;
    static const uint8_t DIR_CW = 0x10;
    static const uint8_t DIR_CCW = 0x20;
    
    // Full steps
    static const uint8_t FS_R_CW_FINAL = 0x1;
    static const uint8_t FS_R_CW_BEGIN = 0x2;
    static const uint8_t FS_R_CW_NEXT = 0x3;
    static const uint8_t FS_R_CCW_BEGIN = 0x4;
    static const uint8_t FS_R_CCW_FINAL = 0x5;
    static const uint8_t FS_R_CCW_NEXT = 0x6;
    
    // Half steps
    static const uint8_t HS_R_CCW_BEGIN = 0x1;
    static const uint8_t HS_R_CW_BEGIN = 0x2;
    static const uint8_t HS_R_START_M = 0x3;
    static const uint8_t HS_R_CW_BEGIN_M = 0x4;
    static const uint8_t HS_R_CCW_BEGIN_M = 0x5;
    
    int toLogicalState(bool vccToPinWiring, int logicMode, int rawValue) {
        int res = 0;
        if (!vccToPinWiring) {
            res = rawValue == HIGH ? 0 : 1;
        } else {
            res = rawValue == HIGH ? 1 : 0;
        }
        if (logicMode == 1) {
            res = res ? 0 : 1;
        }
        return res;
    }

public:
    void begin(Adafruit_MCP23X17 *mcp, byte mcpIndex, byte clkPin, byte dtPin, byte buttonPin, 
               bool reverse, bool enablePullup, byte encoderid, bool half, 
               SHMCP23017RotaryEncoderPositionChanged changedcallback) {
        
        this->mcp = mcp;
        this->mcpIndex = mcpIndex;
        this->clkPin = clkPin;
        this->dtPin = dtPin;
        this->buttonPin = buttonPin;
        this->reverseDirection = reverse;
        this->enablePullup = enablePullup;
        this->halfSteps = half;
        this->id = encoderid;
        this->positionChangedCallback = changedcallback;
        
        buttonDebouncer.begin(50);
        encoderDebouncer.begin(10);
        
        // Configurar pines del encoder
        if (enablePullup) {
            mcp->pinMode(clkPin, INPUT_PULLUP);
            mcp->pinMode(dtPin, INPUT_PULLUP);
        } else {
            mcp->pinMode(clkPin, INPUT);
            mcp->pinMode(dtPin, INPUT);
        }
        
        // Configurar pin del botón si existe
        if (buttonPin != 255) {
            if (enablePullup) {
                mcp->pinMode(buttonPin, INPUT_PULLUP);
            } else {
                mcp->pinMode(buttonPin, INPUT);
            }
            buttonLastState = mcp->digitalRead(buttonPin);
        } else {
            buttonLastState = HIGH; // Estado por defecto si no hay botón
        }
        
        inputLastState = 0;
        positionLastChanged = millis();
    }
    
    uint8_t getDirection(uint8_t delay, unsigned long referenceTime) {
        if (directionLastChange != 255 && (referenceTime - positionLastChanged) < delay) {
            return directionLastChange;
        }
        return 255;
    }
    
    uint8_t getPressed() {
        return (buttonPin != 255) && !buttonLastState;
    }
    
    void read() {
        if (!encoderDebouncer.Debounce()) return;
        
        // Leer estados de los pines CLK y DT
        uint8_t clkState = mcp->digitalRead(clkPin);
        uint8_t dtState = mcp->digitalRead(dtPin);
        
        // Aplicar dirección inversa si está configurada
        if (reverseDirection) {
            uint8_t temp = clkState;
            clkState = dtState;
            dtState = temp;
        }
        
        // Decodificar movimiento del encoder usando las mismas tablas que SHRotaryEncoder
        if (!halfSteps) {
            // Tabla de estados para pasos completos
            static const unsigned char fullStepsTable[7][4] = {
                // 00         01           10           11
                {R_START,     FS_R_CW_BEGIN, FS_R_CCW_BEGIN, R_START},           // R_START
                {FS_R_CW_NEXT, R_START,      FS_R_CW_FINAL,  R_START | DIR_CW},  // R_CW_FINAL
                {FS_R_CW_NEXT, FS_R_CW_BEGIN, R_START,       R_START},           // R_CW_BEGIN
                {FS_R_CW_NEXT, FS_R_CW_BEGIN, FS_R_CW_FINAL,  R_START},           // R_CW_NEXT
                {FS_R_CCW_NEXT, R_START,     FS_R_CCW_BEGIN, R_START},           // R_CCW_BEGIN
                {FS_R_CCW_NEXT, FS_R_CCW_FINAL, R_START,     R_START | DIR_CCW}, // R_CCW_FINAL
                {FS_R_CCW_NEXT, FS_R_CCW_FINAL, FS_R_CCW_BEGIN, R_START}        // R_CCW_NEXT
            };
            inputLastState = fullStepsTable[inputLastState & 0xf][(dtState << 1) | clkState];
        } else {
            // Tabla de estados para medios pasos
            static const unsigned char halfStepsTable[][4] = {
                // 00                  01              10                    11
                {HS_R_START_M,           HS_R_CW_BEGIN,     HS_R_CCW_BEGIN,    R_START},           // R_START (00)
                {HS_R_START_M | DIR_CCW, R_START,           HS_R_CCW_BEGIN,     R_START},           // R_CCW_BEGIN
                {HS_R_START_M | DIR_CW,  HS_R_CW_BEGIN,     R_START,           R_START},           // R_CW_BEGIN
                {HS_R_START_M,           HS_R_CCW_BEGIN_M,  HS_R_CW_BEGIN_M,   R_START},           // R_START_M (11)
                {HS_R_START_M,           HS_R_START_M,      HS_R_CW_BEGIN_M,   R_START | DIR_CW},  // R_CW_BEGIN_M 
                {HS_R_START_M,           HS_R_CCW_BEGIN_M,  HS_R_START_M,      R_START | DIR_CCW}  // R_CCW_BEGIN_M
            };
            inputLastState = halfStepsTable[inputLastState & 0xf][(dtState << 1) | clkState];
        }
        
        direction = (inputLastState & 0x30);
        
        if (direction == DIR_CCW) {
            counter++;
            positionChangedCallback(id, counter, 0);
            positionLastChanged = millis();
            directionLastChange = 0;
        }
        else if (direction == DIR_CW) {
            counter--;
            positionChangedCallback(id, counter, 1);
            positionLastChanged = millis();
            directionLastChange = 1;
        }
        
        // Leer estado del botón si existe
        if (buttonPin != 255) {
            buttonState = mcp->digitalRead(buttonPin);
            if (buttonState != buttonLastState) {
                if (buttonDebouncer.Debounce()) {
                    positionChangedCallback(id, counter, buttonState == HIGH ? 2 : 3);
                    buttonLastState = buttonState;
                }
            }
        }
    }
    
    int getCounter() {
        return counter;
    }
    
    void setCounter(int value) {
        counter = value;
    }
};

// Definición de las constantes estáticas
const uint8_t SHMCP23017RotaryEncoder::R_START = 0x0;
const uint8_t SHMCP23017RotaryEncoder::DIR_CW = 0x10;
const uint8_t SHMCP23017RotaryEncoder::DIR_CCW = 0x20;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CW_FINAL = 0x1;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CW_BEGIN = 0x2;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CW_NEXT = 0x3;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CCW_BEGIN = 0x4;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CCW_FINAL = 0x5;
const uint8_t SHMCP23017RotaryEncoder::FS_R_CCW_NEXT = 0x6;
const uint8_t SHMCP23017RotaryEncoder::HS_R_CCW_BEGIN = 0x1;
const uint8_t SHMCP23017RotaryEncoder::HS_R_CW_BEGIN = 0x2;
const uint8_t SHMCP23017RotaryEncoder::HS_R_START_M = 0x3;
const uint8_t SHMCP23017RotaryEncoder::HS_R_CW_BEGIN_M = 0x4;
const uint8_t SHMCP23017RotaryEncoder::HS_R_CCW_BEGIN_M = 0x5;

#endif
