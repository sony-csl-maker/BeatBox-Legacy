#include <boost/program_options.hpp>

#include <torch/script.h>

#include <exception>
#include <iostream>
#include <memory>

int main(int ac, char **av)
{
    std::string filename;
    std::string outFolder;
    std::string encoder;
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
            ("encoder", boost::program_options::value<std::string>(&encoder)->default_value("./encoderOlesia15_r50_4.p"), "path to the encoder");
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

        if (vm.count("filename")) {
            std::cout << filename << std::endl;
        } else {
            std::cout << "Filename was not set.\n";
        }

        if (vm.count("out-folder")) {
            std::cout << outFolder << std::endl;
        } else {
            std::cout << "Out-folder was not set.\n";
        }

        if (vm.count("encoder")) {
            std::cout << encoder << std::endl;
        } else {
            std::cout << "Encoder was not set.\n";
        }

        if (vm.count("thresh")) {
            std::cout << threshold << std::endl;
        } else {
            std::cout << "Threshold was not set.\n";
        }

        if (vm.count("emph-classes")) {
            std::cout << emphClasses << std::endl;
        } else {
            std::cout << "Emp-classes were not set.\n";
        }

        if (vm.count("strore-samples")) {
            std::cout << stroreSamples << std::endl;
        } else {
            std::cout << "Store-samples was not set.\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Errors(s): " << e.what() << "\n";
        return (1);
    }

    return (0);
}
