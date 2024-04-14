#pragma once
#include <Wire.h>

#include <FullLoopbackStream.h>

// #ifndef WIRE
//     # define WIRE Wire;
// #endif

// #if I2C_BYPASS_MASTER && IC2_SLAVE
//     #define I2C_BYPASS_MASTER
// #endif

// #ifndef IC2_ADDRESS
//     #define IC2_ADDRESS 0x08
// #endif
// #define StreamRead WIRE.read
//  #if I2C_BYPASS_MASTER
//     ic2SetupMaster();
// #endif
// #if IC2_SLAVE
//     ic2SetupSlave();
// #endif

class I2CTransport {
	public:
	I2CTransport(){}
	virtual void loop() = 0 ;
	virtual void flush() = 0;
	virtual void setup(Stream *) = 0;
};