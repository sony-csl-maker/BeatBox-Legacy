/*
** EPITECH PROJECT, 2022
** JUCE
** File description:
** BeatBoxComponent
*/

#ifndef BEATBOXCOMPONENT_HPP_
#define BEATBOXCOMPONENT_HPP_

#include "ProcessorComponent.h"

class BeatBoxComponent
{
public:
    BeatBoxComponent(void);
    ~BeatBoxComponent(void);

    void findPeaks(void);
    void findPeaksWithSmoothness(int smoothness);

    Array<float> encodeSample(Array<float> audioBuffer, const int audioLength);
    Array<float> decodeSample(Array<float> z_c_array_ptr);

    void transferTrack(void);
    Array<float> transferSample(std::vector<float> sample);

    void fillEncodedSamples(void);

    void findStartEndOnset(void);

    void saveOriginFile(void);
    void saveTransferredFile(void);
    void saveSamplesFile(long unsigned int sampleIndex);

    // tools
    void fillAudioTimeSeries(juce::AudioSampleBuffer buffer);
    void onsetDetection(juce::AudioFormatReader *reader, juce::AudioSampleBuffer buffer);
    void setTorchModules(void);
    void setFilename(std::string filename);

    // getters/setters
    std::vector<float> getAudioTrack() { return _audioTimeSeries; };
    std::vector<float> getEncodedAudioTimeSeries() { return _encodedAudioTimeSeries; };
    std::vector<float> getOnsets() { return _onsets; };
    std::vector<float> getPeaks() { return _peaks; };

    Array<float> getZ() { return _newZ; };
    Array<float> getC() { return _normalizedClasses; };

protected:
private:
    std::vector<float> _audioTimeSeries;
    std::vector<float> _encodedAudioTimeSeries;

    std::vector<float> _onsets;
    std::vector<float> _peaks;
    std::vector<float> _peaksValues;

    std::vector<std::pair<float, float>> _startEnd;

    std::vector<std::vector<float>> _samplesTab;
    std::vector<std::vector<float>> _encodedSamplesTab;

    std::string _filename;
    int _index = 0;

    std::string _encoderPath = "JUCE/examples/CMake/BeatBox/encoderOlesia15_r50_4.pt";
    std::string _decoderPath = "JUCE/examples/CMake/BeatBox/gen_noattr_128.pt";
    torch::jit::script::Module _encoder;
    torch::jit::script::Module _decoder;

    Array<float> _newZ;
    Array<float> _normalizedClasses;

    int _numberOfClasses = 128;
    int _numberOfDimensions = 3;

    int _smoothness = 60;

    int _drumify = 0;
};

#endif /* !BEATBOXCOMPONENT_HPP_ */
