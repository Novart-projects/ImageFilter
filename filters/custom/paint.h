#pragma once

#include "../basic/filter.h"

class Paint : public BaseFilter {
public:
    ~Paint() override = default;
    void Apply(Image& img) override;

private:
    const std::vector<int64_t> edge_matrix_ = {-1, -1, 4, -1, -1};
};
