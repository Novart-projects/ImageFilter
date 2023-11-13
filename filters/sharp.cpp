#include "sharp.h"

void Sharp::Apply(Image& img) {
    ColorMatrix new_img(img.Height(), std::deque<Color>(img.Width()));
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            Color new_pixel;
            new_pixel = img.GetPixel(x - 1, y) * sharp_matrix_[0] + img.GetPixel(x, y - 1) * sharp_matrix_[1] +
                        img.GetPixel(x, y) * sharp_matrix_[2] + img.GetPixel(x, y + 1) * sharp_matrix_[3] +
                        img.GetPixel(x + 1, y) * sharp_matrix_[4];
            new_pixel.red = new_pixel.red <= 1 ? new_pixel.red : 1;
            new_pixel.green = new_pixel.green <= 1 ? new_pixel.green : 1;
            new_pixel.blue = new_pixel.blue <= 1 ? new_pixel.blue : 1;
            new_pixel.red = new_pixel.red >= 0 ? new_pixel.red : 0;
            new_pixel.green = new_pixel.green >= 0 ? new_pixel.green : 0;
            new_pixel.blue = new_pixel.blue >= 0 ? new_pixel.blue : 0;
            new_img[x][y] = new_pixel;
        }
    }
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            img.SetPixel(x, y, new_img[x][y]);
        }
    }
}