#pragma once 
#include<JuceHeader.h> 

#include <math.h> 

#include "Parameters.h"

class Distorsion {
public:
    Distorsion() {};
    ~Distorsion() {};
    float static getNextAudioSample(float sample) {
        //return (2.f / MathConstants<float>::pi) * atan(DISTORTION_ALPHA * sample);

        // Implementazione del paper semplice (corretta)

        if (sample <= 1.0f && sample >= -1.0f) {
            return sample - DISTORTION_A * pow(sample, 2) - DISTORTION_B * pow(sample, 3);
        } else if (sample > 1) {
            return 1.0f - DISTORTION_A - DISTORTION_B;
        } else {
            return -1.0f - DISTORTION_A + DISTORTION_B;
        }
    }
};


class LowPass {
public:
    LowPass() {};
    ~LowPass();

    // Calcoli dei parametri
    void preapareToPlay(float samplingFrequency, float cutoffFrequency = BASE_CUTOFF_FREQ, float Q = QUALITY_FACTOR)
    {
        this->samplingFrequency = samplingFrequency;
        this->cutoffFrequency = cutoffFrequency;
        this->Q = Q;

        calcBiquad();
    }

    void setCutoffFrequency(float cutoffFrequency) 
    {
        this->cutoffFrequency = cutoffFrequency;
        calcBiquad();
    }

    void calcBiquad() {
        auto n = 1.0f / std::tan(MathConstants<float>::pi * cutoffFrequency / samplingFrequency);
        auto nSquared = n * n;
        a0 = 1.0f / (1.0f + 1.0f / Q * n + nSquared);
        a1 = a0 * 2.0f;
        a2 = a0;
        b1 = a0 * 2.0f * (1.0f - nSquared);
        b2 = a0 * (1.0f - 1.0f / Q * n + nSquared);
    }

    float getNextAudioSample(float in) {
        float out = in * a0 + z1;
        z1 = in * a1 + z2 - b1 * out;
        z2 = in * a2 - b2 * out;
        return out;
    }

protected:
    float a0 = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;

    
    float samplingFrequency = 44100.0f;
    float cutoffFrequency = BASE_CUTOFF_FREQ;
    float Q = QUALITY_FACTOR;

   
    float z1 = 0.0f;
    float z2 = 0.0f;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowPass);
};