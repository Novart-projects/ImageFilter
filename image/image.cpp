#include "image.h"

void Color::operator*=(double coef) {
    red *= coef;
    blue *= coef;
    green *= coef;
}

void Color::operator+=(const Color &color) {
    red += color.red;
    green += color.green;
    blue += color.blue;
}

Color Color::operator*(double coef) const {
    return Color({red * coef, green * coef, blue * coef});
}

Color Color::operator*(int64_t coef1) const {
    double coef = static_cast<double>(coef1);
    return operator*(coef);
}

Color Color::operator+(const Color &color) const {
    return Color({red + color.red, green + color.green, blue + color.blue});
}

void Color::RaiseSatur() {
    const double mx = 1.0;
    const double raise = 1.1;
    const double lower = 0.9;
    if (red >= blue && red >= green) {
        red = std::min(mx, red * raise);
        if (blue >= green) {
            green = green * lower;
        } else {
            blue = blue * lower;
        }
    } else if (green >= blue && green >= red) {
        green = std::min(mx, green * raise);
        if (blue >= red) {
            red = red * lower;
        } else {
            blue = blue * lower;
        }
    } else if (blue >= green && blue >= red) {
        blue = std::min(mx, blue * raise);
        if (green >= red) {
            red = red * lower;
        } else {
            green = green * lower;
        }
    }
}

Image::Image(std::filesystem::path input_path) {
    Read(input_path);
}

void Image::Read(std::filesystem::path input_path) {
    std::ifstream stream;
    stream.open(input_path, std::ios::binary);
    if (!stream.is_open()) {
        throw std::runtime_error("Input file error: file can't be opened.");
    }
    stream.read(reinterpret_cast<char *>(&file_header_), sizeof(file_header_));
    stream.read(reinterpret_cast<char *>(&info_header_), sizeof(info_header_));
    if (static_cast<int>(info_header_.bit_count) != BMP_BITS) {
        throw std::runtime_error("Intput file error: invalid bmp file format. 1");
    }
    if (static_cast<int>(info_header_.header_size) != HEADER_SIZE) {
        throw std::runtime_error("Intput file error: invalid bmp file format. 2");
    }
    if (file_header_.signature != BMP_SIGNATURE) {
        throw std::runtime_error("Intput file error: invalid bmp file format.3");
    }
    width_ = info_header_.width;
    height_ = info_header_.height;
    const int64_t image_padding = (4 - (static_cast<int64_t>(width_) * 3) % 4) % 4;
    img_matrix_.resize(height_, std::deque<Color>(width_));
    for (size_t x = 0; x < height_; ++x) {
        for (size_t y = 0; y < width_; ++y) {
            unsigned char color[3];
            stream.read(reinterpret_cast<char *>(color), 3);
            img_matrix_[x][y] = {static_cast<double>(color[2]) / MaxColorNum,
                                 static_cast<double>(color[1]) / MaxColorNum,
                                 static_cast<double>(color[0]) / MaxColorNum};
        }
        stream.ignore(image_padding);
    }
    stream.close();
}

size_t Image::Height() const {
    return height_;
}

size_t Image::Width() const {
    return width_;
}

void Image::Resize(size_t new_height, size_t new_width) {
    while (img_matrix_.size() > new_height) {
        img_matrix_.pop_front();
    }
    for (size_t i = 0; i < img_matrix_.size(); ++i) {
        while (img_matrix_[i].size() > new_width) {
            img_matrix_[i].pop_back();
        }
    }
    height_ = std::min(height_, new_height);
    width_ = std::min(width_, new_width);
}

Color Image::GetPixel(int64_t x, int64_t y) const {
    if (x < 0) {
        x = 0;
    }
    if (x >= height_) {
        x = static_cast<int64_t>(height_) - 1;
    }
    if (y < 0) {
        y = 0;
    }
    if (y >= width_) {
        y = static_cast<int64_t>(width_) - 1;
    }
    return img_matrix_[x][y];
}

void Image::SetPixel(size_t x, size_t y, Color color) {
    img_matrix_[x][y] = color;
}

void Image::Write(std::filesystem::path output_path) {
    std::ofstream stream;
    stream.open(output_path, std::ios::binary);
    if (!stream.is_open()) {
        throw std::runtime_error("Output file error: file can't be opened.");
    }
    unsigned char bmp_pad[3] = {0, 0, 0};
    info_header_.height = static_cast<int32_t>(height_);
    info_header_.width = static_cast<int32_t>(width_);
    const int64_t image_padding = (4 - (static_cast<int64_t>(width_) * 3) % 4) % 4;
    stream.write(reinterpret_cast<char *>(&file_header_), sizeof(file_header_));
    stream.write(reinterpret_cast<char *>(&info_header_), sizeof(info_header_));
    for (int64_t x = 0; x < height_; ++x) {
        for (int64_t y = 0; y < width_; ++y) {
            unsigned char red = static_cast<unsigned char>(img_matrix_[x][y].red * MaxColorNum);
            unsigned char green = static_cast<unsigned char>(img_matrix_[x][y].green * MaxColorNum);
            unsigned char blue = static_cast<unsigned char>(img_matrix_[x][y].blue * MaxColorNum);
            unsigned char color[] = {blue, green, red};
            stream.write(reinterpret_cast<char *>(color), 3);
        }
        stream.write(reinterpret_cast<char *>(bmp_pad), image_padding);
    }
    stream.close();
}
