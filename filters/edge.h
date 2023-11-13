#pragma once

#include "filter.h"

class Edge : public BaseFilter {
public:
    explicit Edge(std::vector<double> parameters) : BaseFilter(parameters) {
    }
    ~Edge() override = default;
    void Apply(Image& img) override;

private:
    const std::vector<int64_t> edge_matrix_ = {-1, -1, 4, -1, -1};
};