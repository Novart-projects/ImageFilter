#pragma once

#include "../filters/basic/crop.h"
#include "../filters/basic/blur.h"
#include "../filters/basic/gray_scale.h"
#include "../filters/basic/negative.h"
#include "../filters/custom/glass.h"
#include "../filters/basic/edge.h"
#include "../filters/custom/wave.h"
#include "../filters/basic/sharp.h"
#include "../filters/custom/paint.h"
#include "../filters/custom/sketch.h"

#include <unordered_map>
#include <memory>
#include <queue>

class Parser {
public:
    Parser(int argc, char** argv);
    std::filesystem::path GetInputPath() const;
    std::filesystem::path GetOutputPath() const;
    std::queue<std::unique_ptr<BaseFilter>>& GetFilterSequence();
    bool GetParseStatus() const;

private:
    enum class Filters { BLUR, CROP, SHARP, EDGE, GRAY_SCALE, NEGATIVE, PAINT, SKETCH, GLASS, WAVE };
    inline static const std::unordered_map<std::string, uint8_t> PARAMETERS_ARGS_CNT = {
        {"-blur", 1}, {"-gs", 0},     {"-neg", 0},   {"-sharp", 0}, {"-edge", 1},
        {"-crop", 2}, {"-sketch", 0}, {"-paint", 0}, {"-glass", 0}, {"-wave", 0}};

    inline static const std::unordered_map<std::string, Filters> GOOD_PARAMETER_SWITCH = {
        {"-blur", Filters::BLUR},     {"-gs", Filters::GRAY_SCALE}, {"-neg", Filters::NEGATIVE},
        {"-sharp", Filters::SHARP},   {"-edge", Filters::EDGE},     {"-crop", Filters::CROP},
        {"-sketch", Filters::SKETCH}, {"-paint", Filters::PAINT},   {"-glass", Filters::GLASS},
        {"-wave", Filters::WAVE}};
    std::filesystem::path input_path_;
    std::filesystem::path output_path_;
    std::queue<std::unique_ptr<BaseFilter>> filters_sequence_;
    bool parse_status_ = true;
    std::string_view help_message_ =
        "This is a simple console app to filter your bmp images!\n\n"
        "To use it put to the command line command in accordance with the pattern:\n\n"
        "{app name} {input file path} {output file path} [-{filter name 1} [filter parameter 1] [filter parameter 2] "
        "...]\n[-{filter name 2} [filter parameter 1] [filter parameter 2] ...] ...\n\n"
        "List of available filters:\n\n"
        "-neg - make image negative\n"
        "-gs - make a black and white image\n"
        "-sharp - raise sharpness of image\n"
        "-edge threshold - detect edges (threshold is a number from 0 to 1)\n"
        "-blur sigma - blur image (sigma is a positive number)\n"
        "-glass - make glass effect\n"
        "-wave - make glass wave looking effect\n"
        "-paint - make your picture look like painting\n"
        "-sketch - make your picture look like sketch (based on -paint)\n"
        "-crop width height - crop image (width and height are positive integer numbers)\n\n"
        "Notice: this app works only with 24-bit version of bmp files.\n\n"
        "To get information run app without parameters or with only parameter -help\n";
};
