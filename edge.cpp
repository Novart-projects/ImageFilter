#include "edge.h"

void Edge::Apply(Image& img) {
    const double threshold = parameters_[0];
    ColorMatrix new_img(img.Height(), std::deque<Color>(img.Width()));
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            Color new_pixel;
            new_pixel = img.GetPixel(x - 1, y) * edge_matrix_[0] + img.GetPixel(x, y - 1) * edge_matrix_[1] +
                        img.GetPixel(x, y) * edge_matrix_[2] + img.GetPixel(x, y + 1) * edge_matrix_[3] +
                        img.GetPixel(x + 1, y) * edge_matrix_[4];
            if (new_pixel.red > threshold || new_pixel.green > threshold || new_pixel.blue > threshold) {
                new_pixel.red = 1.0;
                new_pixel.blue = 1.0;
                new_pixel.green = 1.0;
            } else {
                new_pixel.red = 0.0;
                new_pixel.blue = 0.0;
                new_pixel.green = 0.0;
            }
            new_img[x][y] = new_pixel;
        }
    }
    for (int64_t x = 0; x < img.Height(); ++x) {
        for (int64_t y = 0; y < img.Width(); ++y) {
            img.SetPixel(x, y, new_img[x][y]);
        }
    }
}