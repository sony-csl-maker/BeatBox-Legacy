#pragma once

// Juce
#include <JuceHeader.h>

//------------------------------------------------------------------------------
class SimpleThresholdIndex : public juce::Component,
                              private juce::Timer
{
public:
    SimpleThresholdIndex ()
    {
        startTimer (4);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::orange);

        g.drawHorizontalLine(50 - (threshold) * 5, 0, 200);
        g.drawHorizontalLine(50 + (threshold) * 5, 0, 200);
        g.drawHorizontalLine(50 - (threshold) * 5, 200, 300);
        g.drawHorizontalLine(50 + (threshold) * 5, 200, 300);
        g.drawHorizontalLine(50 - (threshold) * 5, 300, 500);
        g.drawHorizontalLine(50 + (threshold) * 5, 300, 500);
    }

    void sendThresholdValue(float value)
    {
        threshold = value;
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    float threshold;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleThresholdIndex)
};
