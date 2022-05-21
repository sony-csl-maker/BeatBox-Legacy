/*
  ==============================================================================
    This file contains the code for peak modelization based on the data
    provided by torch and gist.
  ==============================================================================
*/

#pragma once

// Juce
#include <JuceHeader.h>

//------------------------------------------------------------------------------
class SimplePeakModelization : public juce::Component,
                              private juce::Timer
{
public:
    SimplePeakModelization ()
    {
        startTimer (40);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::black);

        std::cout << "size: " << length << std::endl;
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    std::vector<float> peaks;
    std::size_t length;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplePeakModelization)
};
