#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <torch/script.h>

#include "audiofile/AudioFile.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <filesystem>

int main(int ac, char **av)
{
    std::string filename;
    std::string outFolder;
    std::string encoder;
    std::string decoder;
    float threshold;
    bool emphClasses;
    bool stroreSamples;

    try
    {
        boost::program_options::options_description description("Transfer samples of a drum track into DrumGAN samples");
        description.add_options()
            ("help,h", "display this information");
        description.add_options()
            ("filename", boost::program_options::value<std::string>(&filename), "filename");
        description.add_options()
            ("out-folder", boost::program_options::value<std::string>(&outFolder)->default_value("output"), "out folder");
        description.add_options()
            ("encoder", boost::program_options::value<std::string>(&encoder)->default_value("../encoderOlesia15_r50_4.pt"), "path to the encoder");
        description.add_options()
            ("decoder", boost::program_options::value<std::string>(&decoder)->default_value("../gen_noattr_128.pt"), "path to the decoder");
        description.add_options()
            ("thresh", boost::program_options::value<float>(&threshold)->default_value(0.3), "peak picking threshold");
        description.add_options()
            ("emph-classes", boost::program_options::value<bool>(&emphClasses)->default_value(false), "if percussion type classes should be emphasized");
        description.add_options()
            ("strore-samples", boost::program_options::value<bool>(&stroreSamples)->default_value(false), "if single input samples should be written, too");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(ac, av, description), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << description << "\n";
            return (0);
        }

        // if (vm.count("out-folder")) {
        //     std::cout << outFolder << "\n";
        //     return (0);
        // }

    }
    catch (const std::exception &e)
    {
        std::cerr << "Errors(s): " << e.what() << "\n";
        return (1);
    }

    torch::jit::script::Module encoderModule;
    torch::jit::script::Module decoderModule;

    try {
        encoderModule = torch::jit::load(encoder);
        decoderModule = torch::jit::load(decoder); //a regarder Torch::device / TorchOption
    } catch (const c10::Error& e) {
        std::cerr << "Errors(s): " << e.what() << "\n";
        return (1);
    }

    AudioFile<double> audioFile;
    audioFile.load(filename);

    std::cout << "Bit Depth: " << audioFile.getBitDepth() << std::endl;
    std::cout << "Sample Rate: " << audioFile.getSampleRate() << std::endl;
    std::cout << "Length in Seconds: " << audioFile.getLengthInSeconds() << std::endl;
    std::cout << std::endl;

    std::string keyWord = std::filesystem::path(filename).filename();

    boost::filesystem::create_directory(outFolder);
    outFolder.append("/");

    audioFile.save(outFolder + keyWord + "-compare.wav", AudioFileFormat::Wave);
    audioFile.save(outFolder + keyWord + "-transfert.wav", AudioFileFormat::Wave);
    audioFile.save(outFolder + keyWord + "-original.wav", AudioFileFormat::Wave);

    return (0);
}
