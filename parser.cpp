#include "parser.h"

#include <math.h>

Parser::Parser(int argc, char** argv) {
    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "-help")) {
        std::cout << help_message_;
        parse_status_ = false;
        return;
    }
    if (argc < 3) {
        throw std::runtime_error("Parameter error: too few parameters.");
    }
    if (!std::filesystem::is_directory(std::filesystem::path(argv[1]).parent_path())) {
        throw std::runtime_error("Input file error: invalid path to file.");
    }
    if (!std::filesystem::exists(argv[1]) || !std::filesystem::is_regular_file(argv[1])) {
        throw std::runtime_error("Input file error: file doesn't exist.");
    }
    if (std::filesystem::path(argv[1]).extension() != ".bmp") {
        throw std::runtime_error("Input file error: invalid file extension.");
    }
    if (!std::filesystem::is_directory(std::filesystem::path(argv[2]).parent_path())) {
        throw std::runtime_error("Output file error: invalid path to file.");
    }
    if (std::filesystem::path(argv[2]).extension() != ".bmp") {
        throw std::runtime_error("Output file error: invalid file extension.");
    }
    input_path_ = std::filesystem::path(argv[1]);
    output_path_ = std::filesystem::path(argv[2]);
    for (size_t i = 3; i < argc; ++i) {
        if (GOOD_PARAMETERS.find(argv[i]) != GOOD_PARAMETERS.end()) {
            std::vector<double> params;
            if (i + 1 + GOOD_PARAMETERS.at(argv[i]) > argc) {
                throw std::runtime_error("Parameter error: invalid filter parameter count for " + std::string(argv[i]));
            }
            for (size_t j = 0; j < GOOD_PARAMETERS.at(argv[i]) && i + j < argc; ++j) {
                double a = NAN;
                try {
                    a = std::stod(argv[i + j + 1]);
                } catch (std::invalid_argument r) {
                    throw std::runtime_error("Parameter error: invalid parameter for filter " + std::string(argv[i]));
                }
                if (a >= 0) {
                    params.push_back(a);
                } else {
                    throw std::runtime_error("Parameter error: invalid parameter value for filter " +
                                             std::string(argv[i]));
                }
            }
            switch (GOOD_PARAMETER_SWITCH.at(argv[i])) {
                case Filters::CROP:
                    if (static_cast<double>(static_cast<int64_t>(params[0])) != params[0] ||
                        static_cast<double>(static_cast<int64_t>(params[1])) != params[1]) {
                        throw std::runtime_error("Parameter error: invalid crop filter parameter");
                    }
                    filters_sequence_.push(std::make_unique<Crop>(params));
                    break;
                case Filters::SHARP:
                    filters_sequence_.push(std::make_unique<Sharp>());
                    break;
                case Filters::EDGE:
                    if (params[0] > 1.0) {
                        throw std::runtime_error("Parameter error: invalid edge filter parameter value.");
                    }
                    filters_sequence_.push(std::make_unique<GrayScale>());
                    filters_sequence_.push(std::make_unique<Edge>(params));
                    break;
                case Filters::BLUR:
                    filters_sequence_.push(std::make_unique<Blur>(params));
                    break;
                case Filters::GRAY_SCALE:
                    filters_sequence_.push(std::make_unique<GrayScale>());
                    break;
                case Filters::NEGATIVE:
                    filters_sequence_.push(std::make_unique<Negative>());
                    break;
                case Filters::GLASS:
                    filters_sequence_.push(std::make_unique<Glass>());
                    break;
                case Filters::SKETCH:
                    filters_sequence_.push(std::make_unique<Sketch>());
                    break;
                case Filters::PAINT:
                    filters_sequence_.push(std::make_unique<Paint>());
                    break;
                case Filters::WAVE:
                    filters_sequence_.push(std::make_unique<Wave>());
                    break;
            }
            i += GOOD_PARAMETERS.at(argv[i]);
        } else {
            throw std::runtime_error("Parameter error: no parameter with name " + std::string(argv[i]));
        }
    }
}

std::filesystem::path Parser::GetOutputPath() const {
    return output_path_;
}
std::filesystem::path Parser::GetInputPath() const {
    return input_path_;
}

std::queue<std::unique_ptr<BaseFilter>>& Parser::GetFilterSequence() {
    return filters_sequence_;
}
bool Parser::GetParseStatus() const {
    return parse_status_;
}
