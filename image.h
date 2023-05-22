#pragma once

#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>

struct Color {
    double red = 0;
    double green = 0;
    double blue = 0;
    void operator*=(double coef);
    void operator+=(const Color& color);
    Color operator*(double coef) const;
    Color operator*(int64_t coef1) const;
    Color operator+(const Color& color) const;
    void RaiseSatur();
};

using ColorMatrix = std::deque<std::deque<Color>>;

class Image {
public:
    explicit Image(std::filesystem::path input_path);
    void Read(std::filesystem::path input_path);
    void Write(std::filesystem::path output_path);
    void SetPixel(size_t x, size_t y, Color color);
    Color GetPixel(int64_t x, int64_t y) const;
    void Resize(size_t new_height, size_t new_width);
    size_t Height() const;
    size_t Width() const;

private:
    ColorMatrix img_matrix_;
    size_t height_;
    size_t width_;
    static const uint16_t FILE_HEADER_SIZE = 14;
    static const uint16_t INFORMATION_HEADER_SIZE = 40;
    unsigned char file_header_[FILE_HEADER_SIZE];
    unsigned char info_header_[INFORMATION_HEADER_SIZE];
};
