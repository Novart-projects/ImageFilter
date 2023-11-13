#pragma once

#include "../image.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <numbers>
#include <queue>
#include <vector>

class BaseFilter {
public:
    explicit BaseFilter(std::vector<double> parameters = {}) : parameters_(parameters){};
    virtual ~BaseFilter() = default;
    virtual void Apply(Image& img) = 0;

protected:
    std::vector<double> parameters_;
};
