#include "image_processor.h"

void ImageProcessor::Process(int argc, char** argv) {
    Parser parser(argc, argv);
    if (!parser.GetParseStatus()) {
        return;
    }
    Image image(parser.GetInputPath());
    std::queue<std::unique_ptr<BaseFilter>> sequence_of_filters = std::move(parser.GetFilterSequence());
    while (!std::empty(sequence_of_filters)) {
        sequence_of_filters.front()->Apply(image);
        sequence_of_filters.pop();
    }
    image.Write(parser.GetOutputPath());
}
int main(int argc, char** argv) {
    try {
        ImageProcessor processor;
        processor.Process(argc, argv);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what();
    }
    return 0;
}
