#include "glass.h"

void Glass::Apply(Image& img) {
    const double glass_const = 5.0;
    const double sinus_const = 50.0;
    std::vector<std::vector<Color>> new_img(img.Height(), std::vector<Color>(img.Width()));
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            new_img[x][y] =
                img.GetPixel(x + static_cast<int64_t>(glass_const * std::sin(static_cast<double>(x) * sinus_const) *
                                                      std::sin(static_cast<double>(y) * sinus_const) * 2),
                             y + static_cast<int64_t>(glass_const * std::sin(static_cast<double>(x) * sinus_const) *
                                                      std::sin(static_cast<double>(y) * sinus_const)) *
                                     2);
        }
    }
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            img.SetPixel(x, y, new_img[x][y]);
        }
    }
}
