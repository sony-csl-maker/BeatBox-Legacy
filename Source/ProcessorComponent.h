#pragma once

// Juce
#include <JuceHeader.h>

// std
#include <utility>

// gist
#include "Gist.h"

// libtorch
#include <torch/script.h>

// BeatBoxComponent
#include "BeatBoxComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//==============================================================================
class ProcessorComponent : public juce::Component, public juce::Timer
{
public:
    ProcessorComponent();

    ~ProcessorComponent() override;

    void paint(juce::Graphics &g) override;

    void resized() override;

    void timerCallback() override;


    void updateThreshold();

    void updateSmoothness();


private:
    //==========================================================================

    juce::AudioFormatReader *_reader;
    juce::AudioSampleBuffer _buffer;

    // GET Informations from BeatBox
    std::unique_ptr<BeatBoxComponent> beatBox = std::make_unique<BeatBoxComponent>();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorComponent)
};
