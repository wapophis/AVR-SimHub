
#pragma once
#include "I2CTransport.h"
#include "I2CSerialBridge.h"

#ifndef WIRE
#error WIRE must be settled to have a correct custom wire config in your MASTER config.
#endif

#if I2C_MASTER
I2CSerialBridge instance(I2C_ADDRESS);

class I2CTransportMaster : public I2CTransport {
    public:

    I2CTransportMaster(){}
	 void setup(Stream *outgoingStream){
		i2cSetupSerialBypass();
		instance.setup((FullLoopbackStream*) outgoingStream);
	}
	 void loop() {
		instance.loop();
	}
	 void flush() {
		instance.flush();
	}


	 void i2cSetupSerialBypass(){
         #if I2C_SERIAL_BYPASS_DEBUG
            Serial.print("\n Setting up the master slave connection with slave at ");
            Serial.print(I2C_ADDRESS);
            Serial.println("\n");
        #endif
			#if I2C_MASTER
				instance.i2cSetupMaster();
			#endif
		
	}

    // static size_t write(const uint8_t *buffer, size_t size){
	// 	Wire.beginTransmission(I2C_ADDRESS);
	// 	Wire.write(buffer,size);
	// 	return Wire.endTransmission();
	// }
    // static size_t write(const char *buffer, size_t size){
	// 	Wire.beginTransmission(I2C_ADDRESS);
	// 	Wire.write(buffer,size);
	// 	return Wire.endTransmission();
	// }
    // static size_t write(const char *str){
	// 	Wire.beginTransmission(I2C_ADDRESS);
	// 	Wire.write(str);
	// 	return Wire.endTransmission();
	// }
	// static size_t write(uint8_t data){
	// 	Wire.beginTransmission(I2C_ADDRESS);
	// 	Wire.write(data);
	// 	return Wire.endTransmission();
	// }

};



// #define StreamAvailable WIRE.available
// #define FlowSerialFlush WIRE.flush
// #define StreamFlush WIRE.flush
// #define StreamWrite outgoingStream.write
// #define StreamPrint WIRE.print
// /** SETUP SERIAL BYPASS I2C SLAVE, USE WHEN THIS DEVICE IS CONNECTED TO SIMHUB*/
// #define FlowSerialBegin [](unsigned long baud) { Serial.printf("Hola mundo");}
#endif