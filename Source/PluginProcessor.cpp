/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Parameters.h"
#include "PluginEditor.h"

//==============================================================================
BucketBrigadeAudioProcessor::BucketBrigadeAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ), parameters(*this, nullptr, "BucketBrigadeParameters", Parameters::createParameterLayout())
{
    parameters.addParameterListener(NAME_DW, this);
    parameters.addParameterListener(NAME_DT, this);
    parameters.addParameterListener(NAME_FB, this);
    parameters.addParameterListener(NAME_FREQ, this);
    parameters.addParameterListener(NAME_MOD, this);
    parameters.addParameterListener(NAME_WF, this);
    parameters.addParameterListener(NAME_PH_DELTA, this);
    parameters.addParameterListener(NAME_FREQ_LFO2, this);
    parameters.addParameterListener(NAME_WF_LFO2, this);
    parameters.addParameterListener(NAME_PH_DELTA_LFO2, this);
    parameters.addParameterListener(NAME_LFO_BLEND, this);

    delay.setFeedback(DEFAULT_FB);
    drywetter.setDryWetRatio(DEFAULT_DW);

    //set LFO 1
    LFO.setFrequency(DEFAULT_FREQ);
    LFO.setWaveform(DEFAULT_WF);

    //set LFO 2
    LFO2.setFrequency(DEFAULT_FREQ);
    LFO2.setWaveform(DEFAULT_WF);

    LFOBlend.setDryWetRatio(DEFAULT_LFO_BLEND);

    timeAdapter.setModAmount(DEFAULT_MOD);
    timeAdapter.setParameter(DEFAULT_DT);
}

BucketBrigadeAudioProcessor::~BucketBrigadeAudioProcessor()
{
}

//==============================================================================
void BucketBrigadeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    delay.prepareToPlay(sampleRate, samplesPerBlock);
    LFOBlend.prepareToPlay(sampleRate, samplesPerBlock);

    // Init LFO 1
    LFO.prepareToPlay(sampleRate);

    // Init LFO 2
    LFO2.prepareToPlay(sampleRate);

    // Init modulation signal 1
    modulationSignal.setSize(2, samplesPerBlock);


    timeAdapter.prepareToPlay(sampleRate);
}

void BucketBrigadeAudioProcessor::releaseResources()
{
    drywetter.releaseResources();
    delay.releaseResurces();

    LFOBlend.releaseResources();

    // Release resources of modulation signal 1
    modulationSignal.setSize(0, 0);
}

bool BucketBrigadeAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void BucketBrigadeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto mainBuffer = getBusBuffer(buffer, true, 0);

    juce::ScopedNoDenormals noDenormals;
    const auto numSamples = mainBuffer.getNumSamples();
    const auto numChannels = mainBuffer.getNumChannels();

    // Scrivo su buffer modulationSignal un blocco dell'LFO 1
    LFO.getNextAudioBlock(modulationSignal, numSamples);

    // Salvo la wave di LFO1 su LFOBlend
    LFOBlend.setDry(modulationSignal);

    // Scrivo su buffer modulationSignal un blocco dell'LFO 2
    LFO2.getNextAudioBlock(modulationSignal, numSamples);

    // Blendo il segnale salvato nel Blend con il blocco appena scritto da LFO2
    LFOBlend.merge(modulationSignal);

    // Scalare modulante
    timeAdapter.processBlock(modulationSignal, numSamples);

    // Salvo il segnale in input pulito
    drywetter.setDry(mainBuffer);

    // Applicare delay
    delay.processBlock(mainBuffer, modulationSignal);

    // Miscelo il segnale pulito salvato in drywetter con quello processato da delay
    drywetter.merge(mainBuffer);

    envelopeLeft.set(jmax(mainBuffer.getMagnitude(0, 0, numSamples), envelopeLeft.get()));
    envelopeRight.set(jmax(mainBuffer.getMagnitude(jmin(1, numChannels - 1), 0, numSamples), envelopeRight.get()));
}

juce::AudioProcessorEditor* BucketBrigadeAudioProcessor::createEditor()
{
    return new PluginEditor(*this, parameters);
}

//==============================================================================
void BucketBrigadeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BucketBrigadeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void BucketBrigadeAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_DW)
        drywetter.setDryWetRatio(newValue);

    if (paramID == NAME_DT)
        timeAdapter.setParameter(newValue);

    if (paramID == NAME_FB)
        delay.setFeedback(newValue);

    if (paramID == NAME_FREQ)
        LFO.setFrequency(newValue);

    if (paramID == NAME_MOD)
        timeAdapter.setModAmount(newValue);

    if (paramID == NAME_WF)
        LFO.setWaveform(newValue);

    if (paramID == NAME_PH_DELTA)
        LFO.setPhaseDelta(newValue);

    if (paramID == NAME_FREQ_LFO2)
        LFO2.setFrequency(newValue);

    if (paramID == NAME_PH_DELTA_LFO2)
        LFO2.setPhaseDelta(newValue);

    if (paramID == NAME_WF_LFO2)
        LFO2.setWaveform(newValue);

    if (paramID == NAME_LFO_BLEND)
        LFOBlend.setDryWetRatio(newValue);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BucketBrigadeAudioProcessor();
}
