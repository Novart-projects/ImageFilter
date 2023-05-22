#pragma once

#include "filter.h"

class Sketch : public BaseFilter {
public:
    ~Sketch() override = default;
    void Apply(Image& img) override;

private:
    const std::vector<int64_t> edge_matrix_ = {-1, -1, 4, -1, -1};
    static constexpr double GrayscaleRed = 0.299;
    static constexpr double GrayscaleGreen = 0.587;
    static constexpr double GrayscaleBlue = 0.114;
};
