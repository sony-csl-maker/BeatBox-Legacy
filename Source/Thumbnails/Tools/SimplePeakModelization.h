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
        startTimer (4);
    }

    void paint (juce::Graphics& g) override
    {
        if (_lengthOfTrack == 0)
            paintNoFileLoaded(g);
        else
            paintFileLoaded(g);
    }

    void paintNoFileLoaded(juce::Graphics &g) {
        g.fillAll (juce::Colours::white);
        g.setColour (juce::Colours::darkgrey);
        g.drawFittedText ("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
    }

    void paintFileLoaded(juce::Graphics &g) {
        g.fillAll (juce::Colours::white);
        g.setColour (juce::Colours::darkgrey);
        drawPeaks(g);
        g.drawFittedText (std::to_string(_peaksIndex.size()), getLocalBounds(), juce::Justification::centred, 1);
    }

    void drawPeaks(juce::Graphics &g) {
        for (auto peak : _peaksIndex) {
            g.drawVerticalLine(peak * 500 / _lengthOfTrack, 0, 600);
        }
    }

    void sendPeaksIndex(std::vector<float> peaksIndex)
    {
        std::cout << "send peaks index" << std::endl;
        _peaksIndex = peaksIndex;
    }

    void sendLengthOfTrack(std::size_t lengthOfTrack)
    {
        _lengthOfTrack = lengthOfTrack;
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    std::unique_ptr<ProcessorComponent> _processor = std::make_unique<ProcessorComponent>();

    std::vector<float> _peaksIndex;
    std::size_t _lengthOfTrack = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplePeakModelization)
};
