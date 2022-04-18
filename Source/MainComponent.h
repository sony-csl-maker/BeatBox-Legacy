#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::TextButton _openButton;
    void openButtonClicked(void);
    juce::AudioFormatManager _formatManager;
    juce::AudioSampleBuffer _buffer;
    std::unique_ptr<juce::FileChooser> _fileChooser;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
