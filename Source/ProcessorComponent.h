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

    std::vector<float> getPeaksIndex() const { return peaksIndex; };
    Array<float> getCArray() const { return normalizedClasses; };
    AudioBuffer<float> getConvertBuffer() const { return _convertedBuffer; };

    bool isFileLoaded() { return fileWasLoaded; };
    bool isOnsetsProcessed() { return onsetsProcessed; };
    bool isPeaksProcessed() { return peaksProcessed; };

    void loadFile();
    void loadModel();

    void processOnsets();
    void processPeaks(float smoothnessValue, float treshold);

    bool isLocalMaximum(int index, int smoothness);

    void extractPeaks();

    void transferTrack();

    void toggleDrumify() { drumifyState = !drumifyState; };

    juce::Array<float> encodeSample(juce::Array<float> audioBuffer, const int audioLength);
    juce::Array<float> decodeSample(juce::Array<float> z_c_array_ptr);

    juce::Array<float> drumify(Array<float> &audioBuffer);

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

    std::string encoderPath = "SONYCSL_REPO/examples/CMake/BeatBox/encoderOlesia15_r50_4.pt";
    std::string decoderPath = "SONYCSL_REPO/examples/CMake/BeatBox/gen_noattr_128.pt";
    torch::jit::script::Module encoder;
    torch::jit::script::Module decoder;

    Array<float> newZ;
    Array<float> normalizedClasses;

    std::pair<juce::AudioSampleBuffer, juce::AudioFormatReader *> _data;

    juce::AudioBuffer<float> _convertedBuffer;

    int numberOfClasses = 3;
    int numberOfDimensions = 128;

    int smoothness = 50;
    float threshold = 5.0f;

    bool drumifyState = false;

    bool fileWasLoaded = false;
    bool onsetsProcessed = false;
    bool peaksProcessed = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorComponent)
};
