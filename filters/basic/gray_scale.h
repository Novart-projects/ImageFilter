#pragma once

#include "filter.h"

class GrayScale : public BaseFilter {
public:
    void Apply(Image& img) override;
    ~GrayScale() override = default;

private:
    static constexpr double GrayscaleRed = 0.299;
    static constexpr double GrayscaleGreen = 0.587;
    static constexpr double GrayscaleBlue = 0.114;
};
