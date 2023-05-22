#include "filter.h"

class Negative : public BaseFilter {
public:
    void Apply(Image& img) override;
    ~Negative() override = default;
};
