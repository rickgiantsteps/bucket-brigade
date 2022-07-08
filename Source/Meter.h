#pragma once
#include <JuceHeader.h>

#define FPS       25
#define DB_FLOOR -48.0f
#define RT         0.5f

class Meter : public Component, public Timer
{
public:
    Meter() 
    {
        alpha = exp(-1.0f / (FPS * RT));
        startTimerHz(FPS);
    }

    ~Meter()
    {
        stopTimer();
    }

    void paint(Graphics& g) override
    {
        auto W = getWidth();
        auto H = getHeight();

        g.fillAll(Colours::black);
        g.setColour(Colours::grey);
        g.drawRect(0, 0, W, H, 1);

        if (observedEnvelope != nullptr)
        {
            auto envelopeSnapshot = observedEnvelope->get();
            observedEnvelope->set(envelopeSnapshot * alpha);

            auto peak = Decibels::gainToDecibels(envelopeSnapshot);

            //auto barHeight = (H - 2) * (peak - DB_FLOOR) / abs(DB_FLOOR);
            auto barHeight = jmap(peak, DB_FLOOR, 0.0f, 0.0f, H - 2.0f);
            barHeight = jlimit(0.0f, H - 2.0f, barHeight);

            auto topColour = peak >= 0.0f ? Colours::goldenrod : Colours::rosybrown;
            ColourGradient filler = ColourGradient(Colours::grey, 0, H, topColour, 0, 0, false);
            filler.addColour(0.8f, Colours::lightgoldenrodyellow);

            //g.setColour(peak >= 0.0f ? Colours::red : Colours::blueviolet);
            g.setGradientFill(filler);

            g.fillRect(1.0f, H - 1.0f - barHeight, W - 2.0f, barHeight);
        }

    }

    void resized() override {}

    void connectTo(Atomic<float>& targetVariable)
    {
        observedEnvelope = &targetVariable;
    }

private:

    void timerCallback() override
    {
        //DBG("CIAO");
        repaint();
    }

    float alpha = 0.0f;

    Atomic<float>* observedEnvelope = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter);
};