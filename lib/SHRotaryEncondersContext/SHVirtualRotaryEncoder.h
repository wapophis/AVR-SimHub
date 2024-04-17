#pragma once
#ifndef __SHVIRTUALROTARYENCODER_H__
#define __SHVIRTUALROTARYENCODER_H__

#include <Arduino.h>
#include <Stream.h>
#include <SHRotaryEncodersContext.h>
#include <SHRotaryEncoder.h>

class SHVirtualRotaryEncoder : public SHRotaryEncoder {
private:
    SHRotaryEncoderContext *context;

public:

    // SHVirtualRotaryEncoder(SHRotaryEncoderContext context){
    //     this->context=&context;
    // }

    void begin(uint8_t outputAPin, uint8_t outputBPin, int buttonPin, bool reverse, bool enablePullup, byte encoderid, bool half, SHRotaryEncoderPositionChanged changedcallback) override{
        SHRotaryEncoder::begin(outputAPin,outputBPin,buttonPin,reverse,enablePullup,encoderid,half,changedcallback);
    }

    uint8_t getDirection(uint8_t delay, unsigned long referenceTime) override{
        return SHRotaryEncoder::getDirection(delay,referenceTime);
    }

    uint8_t getPressed() override{
		return SHRotaryEncoder::getPressed();
	}

    void read() override{
        SHRotaryEncoder::read();
    }


};


#endif