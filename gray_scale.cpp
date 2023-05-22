#include "gray_scale.h"

void GrayScale::Apply(Image& img) {
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            const Color pixel = img.GetPixel(x, y);
            double rgb = GrayscaleRed * pixel.red + GrayscaleGreen * pixel.green + GrayscaleBlue * pixel.blue;
            img.SetPixel(x, y, {rgb, rgb, rgb});
        }
    }
}
