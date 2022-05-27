#pragma once

// Juce
#include <JuceHeader.h>

// std
#include <utility>

// gist
#include "Gist.h"

// libtorch
#include <torch/script.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//==============================================================================
class ProcessorComponent : public juce::Timer
{
public:
    ProcessorComponent();
    ~ProcessorComponent();
    void timerCallback() override;

    bool isFileLoaded() { return fileWasLoaded; };
    bool isOnsetsProcessed() { return onsetsProcessed; };
    bool isPeaksProcessed() { return peaksProcessed; };

    void loadFile();
    void loadModel();

    void processOnsets();
    void processPeaks(float value);

    void extractPeaks();

    void transferTrack();

    juce::Array<float> encodeSample(juce::Array<float> audioBuffer, const int audioLength);
    juce::Array<float> decodeSample(juce::Array<float> z_c_array_ptr);

    juce::Array<float> processSamples(std::vector<float> sample);

    void processAudioTrack();

    void processOutputStream();

    void downloadOriginalFile();
    void downloadProcessedFile();
    void downloadSamplesFile(long unsigned int sampleIndex);

    void sendData(std::pair<juce::AudioSampleBuffer, juce::AudioFormatReader *> data) { _data = data; };


private:
    //==========================================================================
    std::vector<float> audioTimeSeries;
    std::vector<float> encodedAudioTimeSeries;

    std::vector<float> onsets;
    std::vector<float> peaksIndex;
    std::vector<float> peaksValues;

    std::vector<std::pair<float, float>> startEnd;

    std::vector<std::vector<float>> samplesTab;
    std::vector<std::vector<float>> encodedSamplesTab;

    std::string filename;
    int _index = 0;

    std::string encoderPath = "JUCE/examples/CMake/BeatBox/encoderOlesia15_r50_4.pt";
    std::string decoderPath = "JUCE/examples/CMake/BeatBox/gen_noattr_128.pt";
    torch::jit::script::Module encoder;
    torch::jit::script::Module decoder;

    Array<float> newZ;
    Array<float> normalizedClasses;

    std::pair<juce::AudioSampleBuffer, juce::AudioFormatReader *> _data;

    int numberOfClasses = 128;
    int numberOfDimensions = 3;

    float smoothness = 0.0f;
    float threshold = 9.5f;

    int drumify = 0;

    bool fileWasLoaded = false;
    bool onsetsProcessed = false;
    bool peaksProcessed = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorComponent)
};
