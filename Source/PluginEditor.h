/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"
#include "Meter.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    //==============================================================================
    PluginEditor (BucketBrigadeAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~PluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* background_png;
    static const int background_pngSize;
    static const char* logobb_png;
    static const int logobb_pngSize;
    static const char* logoFullBlack_png;
    static const int logoFullBlack_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    BucketBrigadeAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> timeAttachment;
    std::unique_ptr<SliderAttachment> fbAttachment;
    std::unique_ptr<SliderAttachment> dwAttachment;
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> amtAttachment;
    std::unique_ptr<SliderAttachment> wfAttachment;
    std::unique_ptr<SliderAttachment> wf2Attachment;
    std::unique_ptr<SliderAttachment> rate2Attachment;
    std::unique_ptr<SliderAttachment> phaseAttachment;
    std::unique_ptr<SliderAttachment> phase2Attachment;
    std::unique_ptr<SliderAttachment> lfoBlendAttachment;

    MyLookAndFeel temaBellissimo;
    MyLookAndFeel temaBellissimo_con_4_tick;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> juce__groupComponent;
    std::unique_ptr<juce::Slider> lfoBlendSlider;
    std::unique_ptr<juce::GroupComponent> juce__groupComponent3;
    std::unique_ptr<juce::GroupComponent> juce__groupComponent2;
    std::unique_ptr<juce::Slider> rateSlider;
    std::unique_ptr<juce::Slider> wfSlider;
    std::unique_ptr<juce::Slider> phaseSlider;
    std::unique_ptr<juce::Slider> rateSlider2;
    std::unique_ptr<juce::Slider> wfSlider2;
    std::unique_ptr<juce::Slider> phaseSlider2;
    std::unique_ptr<juce::Slider> amtSlider;
    std::unique_ptr<juce::Slider> fbSlider;
    std::unique_ptr<juce::Slider> dwSlider;
    std::unique_ptr<juce::Slider> timeSlider;
    std::unique_ptr<Meter> meterComponent2;
    std::unique_ptr<juce::Label> juce__label;
    std::unique_ptr<juce::Label> juce__label2;
    std::unique_ptr<juce::Label> juce__label3;
    std::unique_ptr<juce::Label> juce__label4;
    std::unique_ptr<juce::Label> juce__label5;
    std::unique_ptr<juce::Label> juce__label6;
    std::unique_ptr<juce::Label> juce__label7;
    std::unique_ptr<juce::Label> juce__label8;
    std::unique_ptr<Meter> meterComponent;
    std::unique_ptr<juce::Label> juce__label10;
    std::unique_ptr<juce::Label> juce__label11;
    std::unique_ptr<juce::Label> juce__label12;
    std::unique_ptr<juce::Label> juce__label9;
    std::unique_ptr<juce::Label> juce__label13;
    std::unique_ptr<juce::Label> juce__label14;
    juce::Image cachedImage_background_png_1;
    juce::Image cachedImage_logobb_png_2;
    juce::Image cachedImage_logoFullBlack_png_3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

