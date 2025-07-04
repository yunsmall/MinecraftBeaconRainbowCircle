#pragma once

#include <vector>

#include "MCGlass.h"


class BeaconColorCalculator {
public:
    static constexpr int max_search_num = 5;
    using glasses_type = std::array<MCGlass, max_search_num>;

    BeaconColorCalculator();

    [[nodiscard]] glasses_type calculate_colors(RGBColor target_color) const;

private:
    constexpr static int powi(int num, int exponent) {
        auto result = 1;
        for (auto i = 0; i < exponent; i++) {
            result *= num;
        }
        return result;
    }

    void dfs_generate_block(int data_vector_start_idx, int current_glass_num, glasses_type &glasses);

    std::vector<std::pair<RGBColor, glasses_type>> color_vector{
            (std::size_t) powi(std::size(all_mc_glasses), max_search_num)};
};
