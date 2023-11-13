#pragma once

#include "filter.h"

class Crop : public BaseFilter {
public:
    explicit Crop(std::vector<double> parameters) : BaseFilter(parameters) {
    }
    ~Crop() override = default;
    void Apply(Image& img) override;
};