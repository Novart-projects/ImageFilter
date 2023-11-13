#pragma once

#include "filter.h"

class Sharp : public BaseFilter {
public:
    ~Sharp() override = default;
    void Apply(Image& img) override;

private:
    const std::vector<int64_t> sharp_matrix_ = {-1, -1, 5, -1, -1};
};