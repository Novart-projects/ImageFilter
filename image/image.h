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

    void operator+=(const Color &color);

    Color operator*(double coef) const;

    Color operator*(int64_t coef1) const;

    Color operator+(const Color &color) const;

    void RaiseSatur();
};

using ColorMatrix = std::deque<std::deque<Color>>;

class Image {
#pragma pack(push, 1)
    struct BMPHeader {
        uint16_t signature = 0;
        uint32_t file_size = 0;
        uint32_t reserved = 0;
        uint32_t data_offset = 0;
    };

    struct BMPInfoHeader {
        uint32_t header_size = 0;
        int32_t width = 0;
        int32_t height = 0;
        uint16_t planes = 0;
        uint16_t bit_count = 0;
        uint32_t compression = 0;
        uint32_t image_size = 0;
        int32_t x_pixels_per_meter = 0;
        int32_t y_pixels_per_meter = 0;
        uint32_t colors_used = 0;
        uint32_t colors_important = 0;
    };
#pragma pack(pop)
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
    size_t height_ = 0;
    size_t width_ = 0;
    BMPHeader file_header_;
    BMPInfoHeader info_header_;
    static const uint8_t BMP_BITS = 24;
    static const uint8_t HEADER_SIZE = 40;
    static const uint16_t BMP_SIGNATURE = 0x4D42;
    static constexpr double MaxColorNum = 255.0f;
};
