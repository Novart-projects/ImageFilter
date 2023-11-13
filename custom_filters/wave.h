#pragma once

#include "../filters/filter.h"

class Wave : public BaseFilter {
public:
    ~Wave() override = default;
    void Apply(Image& img) override;
};