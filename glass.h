#pragma once

#include "filter.h"

class Glass : public BaseFilter {
public:
    ~Glass() override = default;
    void Apply(Image& img) override;
};