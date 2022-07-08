#pragma once
#include <JuceHeader.h>

constexpr float MAX_DELAY_TIME = 5.00f;
constexpr float TIME_SMOOTHING = 0.02f;
constexpr float FBK_SMOOTHING = 0.02f;
constexpr float PH_SMOOTHING = 0.02f;
constexpr float LEVEL_SMOOTHING_TIME = 0.02f;
constexpr double GLIDE_TIME = 0.01;
constexpr double SMOOTHING_TIME = 0.04;
constexpr float DISTORTION_ALPHA = 0.9f;
constexpr float BASE_CUTOFF_FREQ = 5000.0f;
constexpr float DELAYTIME_CUTOFF_ALPHA = 50.0f;
constexpr float QUALITY_FACTOR = 0.7f;
constexpr float DISTORTION_A = 1.0f / 8.0f;
constexpr float DISTORTION_B = 1.0f / 16.0f;

constexpr char* NAME_DW = "dw";
constexpr char* NAME_DT = "dt";
constexpr char* NAME_FB = "fb";
constexpr char* NAME_FREQ = "lfoFreq";;
constexpr char* NAME_PH_DELTA = "lfoPhaseDelta";
constexpr char* NAME_PH_DELTA_LFO2 = "lfo2PhaseDelta";
constexpr char* NAME_FREQ_LFO2 = "lfo2Freq";
constexpr char* NAME_MOD = "timeMod";
constexpr char* NAME_WF = "waveform";
constexpr char* NAME_WF_LFO2 = "lfo2Waveform";
constexpr char* NAME_LFO_BLEND = "lfoBlend";

constexpr float DEFAULT_DW = 0.5f;
constexpr float DEFAULT_DT = 0.1f;
constexpr float DEFAULT_FB = 0.0f;
constexpr float DEFAULT_FREQ = 0.1f;
constexpr float DEFAULT_PH_DELTA = 0.0f;
constexpr float DEFAULT_MOD = 0.0f;
constexpr int DEFAULT_WF = 0;
constexpr float DEFAULT_LFO_BLEND = 0.5f;

constexpr int MAX_NUM_INPUT_CHANNELS = 2;

namespace Parameters
{
	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<RangedAudioParameter>> params;

		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB, "Feedback", 0.0f, 0.75f, DEFAULT_FB));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DT, "Delay time (s)", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.3f), DEFAULT_DT));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FREQ, "LFO Freq (Hz)", NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.3f), DEFAULT_FREQ));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_PH_DELTA, "LFO channel phase delta", -1.0f, 1.0f, DEFAULT_PH_DELTA));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_WF, "LFO shape", StringArray{ "Sin", "Tri", "Saw up", "Saw down" }, DEFAULT_WF));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FREQ_LFO2, "LFO2 Freq (Hz)", NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.3f), DEFAULT_FREQ));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_PH_DELTA_LFO2, "LFO2 channel phase delta", -1.0f, 1.0f, DEFAULT_PH_DELTA));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_WF_LFO2, "LFO2 shape", StringArray{ "Sin", "Tri", "Saw up", "Saw down" }, DEFAULT_WF));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_MOD, "Mod amount (s)", NormalisableRange<float>(0.0f, MAX_DELAY_TIME / 2.0f, 0.001f, 0.3f), DEFAULT_MOD));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_LFO_BLEND, "LFO1/LFO2 Blend", 0.0f, 1.0f, DEFAULT_LFO_BLEND));

		return { params.begin(), params.end() };
	}
}