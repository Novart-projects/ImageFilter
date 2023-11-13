#pragma once

#include "filter.h"

class Blur : public BaseFilter {
public:
    explicit Blur(std::vector<double> parameters) : BaseFilter(parameters) {
    }
    ~Blur() override = default;
    void Apply(Image& img) override;
};
