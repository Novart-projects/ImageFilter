#pragma once

#include "image/image.h"
#include "command-parser/parser.h"
#include "filters/basic/filter.h"

class ImageProcessor {
public:
    void Process(int argc, char** argv);
};
