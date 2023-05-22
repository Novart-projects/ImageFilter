#include "blur.h"

void Blur::Apply(Image& img) {
    // std::time_t t = std::time(0);
    const double sigma = parameters_[0];
    const int64_t mnhw = 0;
    const int64_t mxh = static_cast<int64_t>(img.Height());
    const double gauss_formula_const = 1.0f / (2 * std::numbers::pi * std::pow(sigma, 2));
    const int64_t sigma_rad = std::ceil(3 * sigma);
    std::vector<std::vector<double>> ker(2 * sigma_rad, std::vector<double>(2 * sigma_rad));
    for (int64_t i = 0; i < 2 * sigma_rad; ++i) {
        for (int64_t j = 0; j < 2 * sigma_rad; ++j) {
            ker[i][j] = std::pow(std::numbers::e, -((std::pow(sigma_rad - i, 2) + std::pow(sigma_rad - j, 2)) /
                                                    (2 * std::pow(sigma, 2))));
        }
    }
    std::vector<std::vector<Color>> new_img(img.Height(), std::vector<Color>(img.Width()));
    std::vector<std::vector<Color>> new_img1(img.Height(), std::vector<Color>(img.Width()));
    for (int64_t x0 = 0; x0 < img.Height(); ++x0) {
        for (int64_t y0 = 0; y0 < img.Width(); ++y0) {
            Color new_pixel = {0, 0, 0};
            for (int64_t y = y0 - sigma_rad; y < y0 + sigma_rad; ++y) {
                Color img_pixel = img.GetPixel(x0, y);
                new_pixel += img_pixel * ker[sigma_rad][y - y0 + sigma_rad];
            }
            new_img1[x0][y0] = new_pixel * gauss_formula_const;
        }
    }
    for (int64_t x0 = 0; x0 < img.Height(); ++x0) {
        for (int64_t y0 = 0; y0 < img.Width(); ++y0) {
            Color new_pixel = {0, 0, 0};
            for (int64_t x = x0 - sigma_rad; x < x0 + sigma_rad; ++x) {
                Color img_pixel = new_img1[std::max(mnhw, std::min(x, mxh - 1))][y0];
                new_pixel += img_pixel * ker[x - x0 + sigma_rad][sigma_rad];
            }
            new_img[x0][y0] = new_pixel;
        }
    }
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            img.SetPixel(x, y, new_img[x][y]);
        }
    }
    //    std::cout << std::endl
    //              << "Врем"
    //                 "я блюра: "
    //              << static_cast<double>(std::time(0) - t) / constants::SEC_IN_MINUTE << std::endl;
}
