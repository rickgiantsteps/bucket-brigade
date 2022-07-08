#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class NaiveOscillator
{
public:
	NaiveOscillator() {};
	~NaiveOscillator() {};

	void prepareToPlay(double sr)
	{
		samplePeriod = 1.0 / sr;
		frequency.reset(sr, GLIDE_TIME);
		phaseDelta.reset(sr, PH_SMOOTHING);

		for (int ch = 0; ch < MAX_NUM_INPUT_CHANNELS; ch++)
			currentPhase[ch] = 0.0f;
	}

	void setFrequency(float newValue)
	{
		frequency.setTargetValue(newValue);
	}

	void setWaveform(float newValue)
	{
		waveform = roundToInt(newValue);
	}

	void setPhaseDelta(float newValue)
	{
		phaseDelta.setTargetValue(newValue);
	}

	void setChannelPhaseDelta(float delta)
	{
		for (int ch = 0; ch < MAX_NUM_INPUT_CHANNELS; ch++)
		{
			if (delta >= 0 && ((ch % 2) == 0))
			{
				channelPhaseOffset[ch] = (delta+1.0f)/2;
			}

			if (delta <= 0 && !((ch % 2) == 0))
			{
				//channelPhaseOffset[ch] = -delta;
			}
		}
	}

	void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		const int numCh = buffer.getNumChannels();
		auto bufferData = buffer.getArrayOfWritePointers();

		for (int smp = 0; smp < numSamples; ++smp)
		{
			auto nextFrequency = frequency.getNextValue();
			setChannelPhaseDelta(phaseDelta.getNextValue());
			for (int ch = 0; ch < numCh; ++ch)
			{
				bufferData[ch][smp] = getNextAudioSample(currentPhase[ch], nextFrequency, channelPhaseOffset[ch]);
			}
		}
	}

	float getNextAudioSample(float& phase, float nextFrequency, float delta = 0.0f)
	{
		auto sampleValue = 0.0f;
		auto offsettedPhase = phase + delta;
		offsettedPhase -= static_cast<int>(offsettedPhase);

		switch (waveform)
		{
		case Waves::Sine: 
			sampleValue = sin(MathConstants<float>::twoPi * offsettedPhase);
			break;
		case Waves::Triangular:
			sampleValue = 4.0f * abs(offsettedPhase - 0.5f) - 1.0f;
			break;
		case Waves::SawUp:
			sampleValue = 2.0f * offsettedPhase - 1.0f;
			break;
		case Waves::SawDown:
			sampleValue = -2.0f * offsettedPhase - 1.0f;
			break;
		default:
			break;
		}

		phaseIncrement = nextFrequency * samplePeriod;
		phase += phaseIncrement;
		phase -= static_cast<int>(phase);

		return sampleValue;
	}

private:

	enum Waves {Sine = 0, Triangular = 1, SawUp = 2, SawDown = 3};

	int waveform = Waves::Sine; 

	double samplePeriod = 1.0;
	float currentPhase[MAX_NUM_INPUT_CHANNELS];
	float channelPhaseOffset[MAX_NUM_INPUT_CHANNELS];
	float phaseIncrement = 0.0f;
	SmoothedValue<float, ValueSmoothingTypes::Linear> phaseDelta;
	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NaiveOscillator)
};

class ParametrModulation
{
public:
	ParametrModulation() {}
	~ParametrModulation() {}

	void prepareToPlay(double sr)
	{
		parameter.reset(sr, SMOOTHING_TIME);
		modAmount.reset(sr, SMOOTHING_TIME);
	}

	void setParameter(float newValue)
	{
		parameter.setTargetValue(newValue);
	}

	void setModAmount(float newValue)
	{
		modAmount.setTargetValue(newValue);
	}

	void processBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		auto bufferData = buffer.getArrayOfWritePointers();
		auto numCh = buffer.getNumChannels();

		// Scalo la modulante tra 0 e 1
		for (int ch = 0; ch < numCh; ++ch)
		{
			FloatVectorOperations::add(bufferData[ch], 1.0, numSamples);
			FloatVectorOperations::multiply(bufferData[ch], 0.5f, numSamples);
		}

		// Scalo la modulante tra 0 e la modulaziuone massima desiderata
		modAmount.applyGain(buffer, numSamples);

		// Sommo alla modulante il tempo di delay proveniente dal parametro
		if (parameter.isSmoothing())
			for (int smp = 0; smp < numSamples; ++smp)
			{
				const auto param = parameter.getNextValue();

				for (int ch = 0; ch < numCh; ++ch)
					bufferData[ch][smp] += param;
			}
		else
			for (int ch = 0; ch < numCh; ++ch)
				FloatVectorOperations::add(bufferData[ch], parameter.getCurrentValue(), numSamples);

		// Controllo di essere dentro al tempo di delay massimo
		for (int ch = 0; ch < numCh; ++ch)
			FloatVectorOperations::min(bufferData[ch], bufferData[ch], MAX_DELAY_TIME, numSamples);

	}

private:

	SmoothedValue<float, ValueSmoothingTypes::Linear> modAmount;
	SmoothedValue<float, ValueSmoothingTypes::Linear> parameter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametrModulation)

};