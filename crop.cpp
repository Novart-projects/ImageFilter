#include "crop.h"

void Crop::Apply(Image& img) {
    img.Resize(static_cast<int64_t>(parameters_[1]), static_cast<int64_t>(parameters_[0]));
}
