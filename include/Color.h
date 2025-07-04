#pragma once
#include <cstdint>
#include <cmath>
#include <compare>

struct RGBColor {
    std::uint8_t red, green, blue;

    auto operator<=>(const RGBColor &other) const = default;
};

struct HSLColor {
    float hue; // [0,360]
    float saturation; // [0,1]
    float luminance; // [0,1]

    auto operator<=>(const HSLColor &other) const = default;
};

[[nodiscard]] RGBColor HSLtoRGB(const HSLColor &hsl);

[[nodiscard]] inline double distance(const RGBColor &a, const RGBColor &b) {
    // return std::sqrt(std::pow(a.red - b.red, 2.0) + std::pow(a.green - b.green, 2) + std::pow(a.blue - b.blue, 2.0));
    return std::pow(a.red - b.red, 2.0) + std::pow(a.green - b.green, 2) + std::pow(a.blue - b.blue, 2.0);
}
