#pragma once
#include <string>
#include <vector>
#include <ranges>
#include <stdexcept>

#include "Color.h"


enum class MCGlass {
    White,
    LightGray,
    Gray,
    Black,
    Brown,
    Red,
    Orange,
    Yellow,
    Lime,
    Green,
    Cyan,
    LightBlue,
    Blue,
    Purple,
    Magenta,
    Pink,
};

inline constexpr MCGlass all_mc_glasses[] = {
        MCGlass::White,
        MCGlass::LightGray,
        MCGlass::Gray,
        MCGlass::Black,
        MCGlass::Brown,
        MCGlass::Red,
        MCGlass::Orange,
        MCGlass::Yellow,
        MCGlass::Lime,
        MCGlass::Green,
        MCGlass::Cyan,
        MCGlass::LightBlue,
        MCGlass::Blue,
        MCGlass::Purple,
        MCGlass::Magenta,
        MCGlass::Pink,
};

[[nodiscard]] inline RGBColor glass_to_rgb(MCGlass glass) {
    switch (glass) {
        case MCGlass::White:
            return {249, 255, 254};
        case MCGlass::LightGray:
            return {157, 157, 151};
        case MCGlass::Gray:
            return {71, 79, 82};
        case MCGlass::Black:
            return {29, 29, 33};
        case MCGlass::Brown:
            return {131, 84, 50};
        case MCGlass::Red:
            return {176, 46, 38};
        case MCGlass::Orange:
            return {249, 128, 29};
        case MCGlass::Yellow:
            return {254, 216, 61};
        case MCGlass::Lime:
            return {128, 199, 31};
        case MCGlass::Green:
            return {94, 124, 22};
        case MCGlass::Cyan:
            return {22, 156, 156};
        case MCGlass::LightBlue:
            return {58, 179, 218};
        case MCGlass::Blue:
            return {60, 68, 170};
        case MCGlass::Purple:
            return {137, 50, 184};
        case MCGlass::Magenta:
            return {199, 78, 189};
        case MCGlass::Pink:
            return {243, 139, 170};
        default:
            throw std::logic_error("未知颜色");
    }
}

[[nodiscard]] inline std::string glass_to_mc_name(MCGlass glass) {
    switch (glass) {
        case MCGlass::White:
            return "minecraft:white_stained_glass";
        case MCGlass::LightGray:
            return "minecraft:light_gray_stained_glass";
        case MCGlass::Gray:
            return "minecraft:gray_stained_glass";
        case MCGlass::Black:
            return "minecraft:black_stained_glass";
        case MCGlass::Brown:
            return "minecraft:brown_stained_glass";
        case MCGlass::Red:
            return "minecraft:red_stained_glass";
        case MCGlass::Orange:
            return "minecraft:orange_stained_glass";
        case MCGlass::Yellow:
            return "minecraft:yellow_stained_glass";
        case MCGlass::Lime:
            return "minecraft:lime_stained_glass";
        case MCGlass::Green:
            return "minecraft:green_stained_glass";
        case MCGlass::Cyan:
            return "minecraft:cyan_stained_glass";
        case MCGlass::LightBlue:
            return "minecraft:light_blue_stained_glass";
        case MCGlass::Blue:
            return "minecraft:blue_stained_glass";
        case MCGlass::Purple:
            return "minecraft:purple_stained_glass";
        case MCGlass::Magenta:
            return "minecraft:magenta_stained_glass";
        case MCGlass::Pink:
            return "minecraft:pink_stained_glass";
        default:
            throw std::logic_error("未知颜色");
    }
}


template<std::ranges::range Range>
[[nodiscard]] RGBColor glasses_to_rgb(const Range &glasses) {
    auto calculate_channel_avg = [](const std::vector<std::uint8_t> &channel)-> std::uint8_t {
        double sum = 0;
        for (const auto& [i,c_i]: std::views::enumerate(std::views::reverse(channel))) {
            sum += std::pow(1.0 / 2, i + 1) * c_i;
        }
        return static_cast<std::uint8_t>(std::round(sum));
    };

    std::vector<std::uint8_t> red_channel(std::size(glasses));
    std::vector<std::uint8_t> green_channel(std::size(glasses));
    std::vector<std::uint8_t> blue_channel(std::size(glasses));

    for (auto &glass: glasses) {
        auto [red, green, blue] = glass_to_rgb(glass);;
        red_channel.emplace_back(red);
        green_channel.emplace_back(green);
        blue_channel.emplace_back(blue);
    }

    return RGBColor{calculate_channel_avg(red_channel), calculate_channel_avg(green_channel),
                    calculate_channel_avg(blue_channel)};
}
