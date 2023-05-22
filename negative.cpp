#include "negative.h"

void Negative::Apply(Image& img) {
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            const Color pixel = img.GetPixel(x, y);
            img.SetPixel(x, y, {1.0 - pixel.red, 1.0 - pixel.green, 1.0 - pixel.blue});
        }
    }
}