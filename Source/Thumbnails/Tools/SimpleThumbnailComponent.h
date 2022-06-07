#pragma once

// Juce
#include <JuceHeader.h>

//==============================================================================
class SimpleThumbnailComponent : public juce::Component,
                                 private juce::ChangeListener
{
public:
    SimpleThumbnailComponent (int sourceSamplesPerThumbnailSample,
                              juce::AudioFormatManager& formatManager,
                              juce::AudioThumbnailCache& cache)
       : thumbnail (sourceSamplesPerThumbnailSample, formatManager, cache)
    {
        thumbnail.addChangeListener (this);
    }

    void setFile (const juce::File& file)
    {
        thumbnail.setSource (new juce::FileInputSource (file));
    }

    void paint (juce::Graphics& g) override
    {
        if (thumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded (g);
        else
            paintIfFileLoaded (g);
    }

    void paintIfNoFileLoaded (juce::Graphics& g)
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::white);
        g.drawFittedText ("Please load a file", getLocalBounds(), juce::Justification::centred, 1);
    }

    void paintIfFileLoaded (juce::Graphics& g)
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::white);
        thumbnail.drawChannels (g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &thumbnail)
            thumbnailChanged();
    }

private:
    void thumbnailChanged()
    {
        repaint();
    }

    juce::AudioThumbnail thumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleThumbnailComponent)
};
