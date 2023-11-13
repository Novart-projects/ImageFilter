#include "paint.h"
#include "../filters/blur.h"

void Paint::Apply(Image& img) {
    Color expexted = {0, 0, 0};
    Color dispersion = {0, 0, 0};
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            expexted += img.GetPixel(x, y);
        }
    }
    expexted *= 1.0 / static_cast<double>(img.Height() * img.Width());
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            dispersion += {(expexted + img.GetPixel(x, y) * -1.0).red * (expexted + img.GetPixel(x, y) * -1.0).red,
                           (expexted + img.GetPixel(x, y) * -1.0).green * (expexted + img.GetPixel(x, y) * -1.0).green,
                           (expexted + img.GetPixel(x, y) * -1.0).blue * (expexted + img.GetPixel(x, y) * -1.0).blue};
        }
    }
    dispersion *= 1.0 / static_cast<double>(img.Height() * img.Width());
    std::vector<std::vector<bool>> processed(img.Height(), std::vector<bool>(img.Width(), false));
    std::vector<std::vector<Color>> new_img(img.Height(), std::vector<Color>(img.Width()));
    // const double eps = parameters_[0] * (dispersion.red + dispersion.blue + dispersion.blue) / 3.0;
    const double eps = 0.1 + ((dispersion.red + dispersion.blue + dispersion.blue) / 3.0) *
                                 ((dispersion.red + dispersion.blue + dispersion.blue) / 3.0);
    Blur blur({12 * eps});
    blur.Apply(img);
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            if (!processed[x][y]) {
                std::queue<std::pair<int64_t, int64_t>> process;
                process.push({x, y});
                processed[x][y] = true;
                while (!std::empty(process)) {
                    auto [x0, y0] = process.front();
                    new_img[x0][y0] = img.GetPixel(x, y);
                    for (int i = -1; i < 2; ++i) {
                        for (int j = -1; j < 2; ++j) {
                            if (x0 + i >= 0 && x0 + i < img.Height() && y0 + j >= 0 && y0 + j < img.Width()) {
                                if (!processed[x0 + i][y0 + j] &&
                                    std::abs(img.GetPixel(x0 + i, y0 + j).red - img.GetPixel(x, y).red) < eps &&
                                    std::abs(img.GetPixel(x0 + i, y0 + j).green - img.GetPixel(x, y).green) < eps &&
                                    std::abs(img.GetPixel(x0 + i, y0 + j).blue - img.GetPixel(x, y).blue) < eps) {
                                    process.push({x0 + i, y0 + j});
                                    processed[x0 + i][y0 + j] = true;
                                }
                            }
                        }
                    }
                    process.pop();
                }
            }
        }
    }
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            img.SetPixel(x, y, new_img[x][y]);
        }
    }
}
