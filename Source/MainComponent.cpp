#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : _openButton("Click to choose a file")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 800);

    _openButton.onClick = [this] {openButtonClicked();};
    addAndMakeVisible(&_openButton);
    _formatManager.registerBasicFormats();

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::openButtonClicked()
{
    shutdownAudio();
    _fileChooser = std::make_unique<juce::FileChooser> ("Select a Wave file shorter than 2 seconds to play...",
                                                       juce::File{},
                                                       "*.wav");

    // if (chooser.browseForFileToOpen()) {
    //     juce::File myFile;

    //     myFile = chooser.getResult();
    //     DBG(myFile.getFullPathName());

    //     std::unique_ptr<juce::AudioFormatReader> reader (_formatManager.createReaderFor(myFile));
    //     // std::unique_ptr<juce::AudioFormatReaderSource> tempSource (new juce::AudioFormatReaderSource (reader, true));
    //     // juce::AudioSampleBuffer buffer(reader->numChannels, reader->lengthInSamples);
    //     // auto duration = reader->lengthInSamples / reader->sampleRate;
    //     // DBG(duration);

    // }
    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;
    _fileChooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        DBG(file.getFileName());
        if (file == juce::File{})
            return;
        std::unique_ptr<juce::AudioFormatReader> reader (_formatManager.createReaderFor (file)); // [2]
        juce::AudioSampleBuffer buffer(reader->numChannels, reader->lengthInSamples);
        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;
            DBG((int) reader->numChannels);            // [3]
            buffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);  // [4]
            reader->read (&buffer,                                                      // [5]
                          0,                                                                //  [5.1]
                          (int) reader->lengthInSamples,                                    //  [5.2]
                          0,
                          true,
                          true);
            DBG((float) buffer.getSample(0, 400));
            setAudioChannels (0, (int) reader->numChannels);
        }
    });
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    _openButton.setBounds(10, 10, getWidth() - 20, 30);
}
