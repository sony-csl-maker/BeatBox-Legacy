#pragma once

// Juce
#include <JuceHeader.h>

#include "../../ProcessorComponent.h"

//------------------------------------------------------------------------------
// DO NOT USE YET
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

    void sendPeaksValue()
    {
        peaks = _processor->sendPeaks();
    }

    void sendTrackLength()
    {
        length = _processor->sendAudioTimeSeriesLength();
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    std::unique_ptr<ProcessorComponent> _processor = std::make_unique<ProcessorComponent>();

    std::vector<float> peaks;
    std::size_t length;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplePeakModelization)
};
