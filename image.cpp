#include "image.h"

namespace {
static const uint8_t BMP_BITS = 24;
static constexpr uint8_t BinShift[4] = {4, 8, 16, 24};
static constexpr double MaxColorNum = 255.0f;
static const uint8_t FILE_FORMAT_INDEX1 = 0;
static const uint8_t FILE_FORMAT_INDEX2 = 1;
static const uint8_t FILE_SIZE_INDEX1 = 2;
static const uint8_t FILE_SIZE_INDEX2 = 3;
static const uint8_t FILE_SIZE_INDEX3 = 4;
static const uint8_t FILE_SIZE_INDEX4 = 5;
static const uint8_t HEADER_SIZE_INDEX1 = 10;
static const uint8_t HEADER_SIZE_INDEX2 = 0;
static const uint8_t WIDTH_INDEX1 = 4;
static const uint8_t WIDTH_INDEX2 = 5;
static const uint8_t WIDTH_INDEX3 = 6;
static const uint8_t WIDTH_INDEX4 = 7;
static const uint8_t HEIGHT_INDEX1 = 8;
static const uint8_t HEIGHT_INDEX2 = 9;
static const uint8_t HEIGHT_INDEX3 = 10;
static const uint8_t HEIGHT_INDEX4 = 11;
static const uint8_t BIT_INDEX = 14;
static const uint8_t PLANE_INDEX = 12;
static const uint8_t PLANE = 1;
}  // namespace

void Color::operator*=(double coef) {
    red *= coef;
    blue *= coef;
    green *= coef;
}
void Color::operator+=(const Color& color) {
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
Color Color::operator+(const Color& color) const {
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
    stream.open(input_path, std::ios::in | std::ios::binary);
    if (!stream.is_open()) {
        throw std::runtime_error("Input file error: file can't be opened.");
    }
    stream.read(reinterpret_cast<char*>(file_header_), FILE_HEADER_SIZE);
    stream.read(reinterpret_cast<char*>(info_header_), INFORMATION_HEADER_SIZE);
    if (static_cast<int>(info_header_[BIT_INDEX]) != BMP_BITS) {
        throw std::runtime_error("Intput file error: invalid bmp file format.");
    }
    if (file_header_[FILE_FORMAT_INDEX1] != 'B' || file_header_[FILE_FORMAT_INDEX2] != 'M') {
        throw std::runtime_error("Intput file error: invalid bmp file format.");
    }
    width_ = info_header_[WIDTH_INDEX1] + (info_header_[WIDTH_INDEX2] << BinShift[1]) +
             (info_header_[WIDTH_INDEX3] << BinShift[2]) + (info_header_[WIDTH_INDEX4] << BinShift[3]);
    height_ = info_header_[HEIGHT_INDEX1] + (info_header_[HEIGHT_INDEX2] << BinShift[1]) +
              (info_header_[HEIGHT_INDEX3] << BinShift[2]) + (info_header_[HEIGHT_INDEX4] << BinShift[3]);
    const int64_t image_padding = (4 - (static_cast<int64_t>(width_) * 3) % 4) % 4;
    img_matrix_.resize(height_, std::deque<Color>(width_));
    for (size_t x = 0; x < height_; ++x) {
        for (size_t y = 0; y < width_; ++y) {
            unsigned char color[3];
            stream.read(reinterpret_cast<char*>(color), 3);
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
    const int64_t image_padding = (4 - (static_cast<int64_t>(width_) * 3) % 4) % 4;
    const int64_t filesize = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE +
                             static_cast<int64_t>(width_) * static_cast<int64_t>(height_) * 3 +
                             image_padding * static_cast<int64_t>(height_);
    file_header_[FILE_FORMAT_INDEX1] = 'B';
    file_header_[FILE_FORMAT_INDEX2] = 'M';
    file_header_[FILE_SIZE_INDEX1] = filesize;
    file_header_[FILE_SIZE_INDEX2] = filesize >> BinShift[1];
    file_header_[FILE_SIZE_INDEX3] = filesize >> BinShift[2];
    file_header_[FILE_SIZE_INDEX4] = filesize >> BinShift[3];
    file_header_[HEADER_SIZE_INDEX1] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE;
    info_header_[HEADER_SIZE_INDEX2] = INFORMATION_HEADER_SIZE;
    info_header_[WIDTH_INDEX1] = width_;
    info_header_[WIDTH_INDEX2] = width_ >> BinShift[1];
    info_header_[WIDTH_INDEX3] = width_ >> BinShift[2];
    info_header_[WIDTH_INDEX4] = width_ >> BinShift[3];
    info_header_[HEIGHT_INDEX1] = height_;
    info_header_[HEIGHT_INDEX2] = height_ >> BinShift[1];
    info_header_[HEIGHT_INDEX3] = height_ >> BinShift[2];
    info_header_[HEIGHT_INDEX4] = height_ >> BinShift[3];
    info_header_[PLANE_INDEX] = PLANE;
    info_header_[BIT_INDEX] = BMP_BITS;
    stream.write(reinterpret_cast<char*>(file_header_), FILE_HEADER_SIZE);
    stream.write(reinterpret_cast<char*>(info_header_), INFORMATION_HEADER_SIZE);
    for (int64_t x = 0; x < height_; ++x) {
        for (int64_t y = 0; y < width_; ++y) {
            unsigned char red = static_cast<unsigned char>(img_matrix_[x][y].red * MaxColorNum);
            unsigned char green = static_cast<unsigned char>(img_matrix_[x][y].green * MaxColorNum);
            unsigned char blue = static_cast<unsigned char>(img_matrix_[x][y].blue * MaxColorNum);
            unsigned char color[] = {blue, green, red};
            stream.write(reinterpret_cast<char*>(color), 3);
        }
        stream.write(reinterpret_cast<char*>(bmp_pad), image_padding);
    }
    stream.close();
}
