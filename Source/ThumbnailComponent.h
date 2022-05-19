#pragma once

// Juce
#include <JuceHeader.h>

//==============================================================================
class ThumbnailComponent   :    public juce::Component,
                                private juce::Timer
{
public:
    ThumbnailComponent(juce::File file)
        : thumbnailCache (5),
          thumbnail(512, formatManager, thumbnailCache)
    {
        thumbnail.setSource (new juce::FileInputSource (file));

        formatManager.registerBasicFormats();
        // transportSource.addChangeListener (this);
        // thumbnail.addChangeListener (this);

        startTimer (40);
    }

    ~ThumbnailComponent() override
    {

    }

    void paint (juce::Graphics& g) override
    {
        // modify here to match actual bounds
        juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);

        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g, thumbnailBounds);
        else
            paintIfFileLoaded (g, thumbnailBounds);
    }

    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::darkgrey);
        g.fillRect (thumbnailBounds);
        g.setColour (juce::Colours::white);
        g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
    }

    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
    {
        g.setColour (juce::Colours::white);
        g.fillRect (thumbnailBounds);

        g.setColour (juce::Colours::red);

        auto audioLength = (float) thumbnail.getTotalLength();
        thumbnail.drawChannels (g, thumbnailBounds, 0.0, audioLength, 1.0f);

        g.setColour (juce::Colours::green);

        // auto audioPosition = (float) transportSource.getCurrentPosition();
        auto drawPosition = (audioLength) * (float) thumbnailBounds.getWidth()
                            + (float) thumbnailBounds.getX();
        g.drawLine (drawPosition, (float) thumbnailBounds.getY(), drawPosition,
                    (float) thumbnailBounds.getBottom(), 2.0f);
    }

private:
    void thumbnailChanged()
    {
        repaint();
    }

    void timerCallback() override
    {
        repaint();
    }

    //==========================================================================
    juce::AudioFormatManager formatManager;
    // juce::AudioTransportSource transportSource;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};
