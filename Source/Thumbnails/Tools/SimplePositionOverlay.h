#pragma once

// Juce
#include <JuceHeader.h>

//------------------------------------------------------------------------------
class SimplePositionOverlay : public juce::Component,
                              private juce::Timer
{
public:
    SimplePositionOverlay (juce::AudioTransportSource& transportSourceToUse)
       : transportSource (transportSourceToUse)
    {
        startTimer (40);
    }

    void paint (juce::Graphics& g) override
    {
        auto duration = (float) transportSource.getLengthInSeconds();

        if (duration > 0.0)
        {
            auto audioPosition = (float) transportSource.getCurrentPosition();
            auto drawPosition = (audioPosition / duration) * (float) getWidth();

            g.setColour (juce::Colours::red);
            g.drawLine (drawPosition, 0.0f, drawPosition, (float) getHeight(), 2.0f);
        }
    }

    void mouseDown (const juce::MouseEvent& event) override
    {
        auto duration = transportSource.getLengthInSeconds();

        if (duration > 0.0)
        {
            auto clickPosition = event.position.x;
            auto audioPosition = (clickPosition / (float) getWidth()) * duration;

            transportSource.setPosition (audioPosition);
        }
    }

private:
    void timerCallback() override
    {
        repaint();
    }

    juce::AudioTransportSource& transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplePositionOverlay)
};
