#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <tuple>
#include <filesystem>
#include <numbers>
#include <ranges>
#include <vector>

#include "BeaconColorCalculator.h"


using position_data_type = std::list<std::array<int, 2>>;

std::string package_name = "my_test";

double transform_to_circle(double angle) {
    while (not(-std::numbers::pi <= angle && angle <= std::numbers::pi)) {
        if (angle > std::numbers::pi) {
            angle -= 2 * std::numbers::pi;
        } else if (angle < -std::numbers::pi) {
            angle += 2 * std::numbers::pi;
        }
    }
    return angle;
}

double get_angle_distance(double angle1, double angle2) {
    angle1 = transform_to_circle(angle1);
    angle2 = transform_to_circle(angle2);

    auto delta = angle1 - angle2;
    if (delta < -std::numbers::pi) {
        delta += 2 * std::numbers::pi;
    } else if (delta > std::numbers::pi) {
        delta -= 2 * std::numbers::pi;
    }
    return delta;
}

auto calculate_positions(int point_num, double minimum_distance,
                         double maximum_distance) -> position_data_type {
    std::vector<std::tuple<int, int, double>> candidate_positions;

    //方块坐标限制，入参决定外切圆和内接圆半径，这里计算内部正方形大小
    int block_max_distance = std::ceil(maximum_distance);
    int block_min_distance = std::floor(minimum_distance / std::sqrt(2));
    //顶部长方形
    for (int x = -block_max_distance; x <= block_max_distance; x++) {
        for (int z = -block_max_distance; z <= -block_min_distance; z++) {
            auto distance = std::sqrt(std::pow(x, 2) + std::pow(z, 2));
            if (distance >= minimum_distance && distance <= maximum_distance) {
                candidate_positions.emplace_back(x, z, std::atan2(z, x));
            }
        }
    }
    //左中长方形
    for (int x = -block_max_distance; x <= -block_min_distance; x++) {
        for (int z = -block_min_distance; z <= block_min_distance; z++) {
            auto distance = std::sqrt(std::pow(x, 2) + std::pow(z, 2));
            if (distance >= minimum_distance && distance <= maximum_distance) {
                candidate_positions.emplace_back(x, z, std::atan2(z, x));
            }
        }
    }
    //右中长方形
    for (int x = block_min_distance; x <= block_max_distance; x++) {
        for (int z = -block_min_distance; z <= block_min_distance; z++) {
            auto distance = std::sqrt(std::pow(x, 2) + std::pow(z, 2));
            if (distance >= minimum_distance && distance <= maximum_distance) {
                candidate_positions.emplace_back(x, z, std::atan2(z, x));
            }
        }
    }
    //底部长方形
    for (int x = -block_max_distance; x <= block_max_distance; x++) {
        for (int z = block_min_distance; z <= block_max_distance; z++) {
            auto distance = std::sqrt(std::pow(x, 2) + std::pow(z, 2));
            if (distance >= minimum_distance && distance <= maximum_distance) {
                candidate_positions.emplace_back(x, z, std::atan2(z, x));
            }
        }
    }

    position_data_type result;

    auto delta_theta = 2 * std::numbers::pi / point_num;
    for (int point_index = 0; point_index < point_num; point_index++) {
        auto current_angle = -std::numbers::pi + point_index * delta_theta;
        //找到最接近的那个点
        auto approximate_element = std::ranges::min(candidate_positions, [current_angle](auto &a, auto &b) {
            return std::abs(get_angle_distance(std::get<2>(a), current_angle)) < std::abs(
                           get_angle_distance(std::get<2>(b), current_angle));
        });
        result.push_back({std::get<0>(approximate_element), std::get<1>(approximate_element)});
    }

    return result;
}

void insert_clear_code(std::ofstream &file, const position_data_type &datas) {
    for (auto &data: datas) {
        auto offset_x = data[0];
        auto offset_z = data[1];
        std::println(
                file,
                "fill ~{} ~ ~{} ~{} ~10 ~{} minecraft:air",
                offset_x + 1, offset_z + 1, offset_x - 1, offset_z - 1);
    }
}

auto generate_clear_function_file(const std::string &clear_function_name, const position_data_type &datas) {
    std::string clear_function_file_name = clear_function_name + ".mcfunction";
    std::string core_function_name = clear_function_name + "_core";
    std::string core_function_file_name = core_function_name + ".mcfunction";
    std::ofstream clear_function_file(clear_function_file_name);
    clear_function_file << "# 本文件由程序自动生成\n" << std::endl;
    std::println(
            clear_function_file,
            "$execute positioned $(origin_x) $(origin_y) $(origin_z) run function {}:{}", package_name,
            core_function_name);

    std::ofstream core_function_file(core_function_file_name);
    core_function_file << "# 本文件由程序自动生成\n" << std::endl;
    insert_clear_code(core_function_file, datas);
}

auto generate_function_file(const std::string &generate_function_name, const position_data_type &datas) {
    std::string generate_function_file_name = generate_function_name + ".mcfunction";
    std::string core_function_name = generate_function_name + "_core";
    std::string core_function_file_name = core_function_name + ".mcfunction";


    std::ofstream generate_function_file(generate_function_file_name);
    generate_function_file << "# 本文件由程序自动生成\n" << std::endl;
    std::println(
            generate_function_file,
            "$execute positioned $(origin_x) $(origin_y) $(origin_z) run function {}:{}", package_name,
            core_function_name);

    std::ofstream core_function_file(core_function_file_name);
    core_function_file << "# 本文件由程序自动生成\n" << std::endl;
    //落脚点
    std::println(
            core_function_file,
            "fill ~1 ~-1 ~1 ~-1 ~-1 ~-1 minecraft:gold_block");
    std::println(
            core_function_file,
            "setblock ~ ~-1 ~ minecraft:diamond_block");
    insert_clear_code(core_function_file, datas);

    BeaconColorCalculator calculator;

    HSLColor hsl_color{};
    hsl_color.luminance = 0.5;
    hsl_color.saturation = 1;
    for (const auto &[i,data]: std::views::enumerate(datas)) {
        std::println(std::cout, "正在生成创建第{}个信标的代码", i);
        hsl_color.hue = (float) i / datas.size() * 360;
        // std::println(std::cout, "当前hue值为{}",hsl_color.hue);
        auto rgb_color = HSLtoRGB(hsl_color);
        // std::println(std::cout, "当前rgb值为r:{},g:{},b:{}", rgb_color.red,rgb_color.green,rgb_color.blue);
        auto blocks = calculator.calculate_colors(rgb_color);

        auto offset_x = data[0];
        auto offset_z = data[1];

        for (const auto &[i,glass]: std::views::enumerate(blocks)) {
            std::println(
                    core_function_file,
                    "setblock ~{} ~{} ~{} {}",
                    offset_x, i + 2, offset_z, glass_to_mc_name(glass));
        }

        std::println(
                core_function_file,
                "fill ~{} ~ ~{} ~{} ~ ~{} minecraft:iron_block",
                offset_x + 1, offset_z + 1, offset_x - 1, offset_z - 1);
        std::println(
                core_function_file,
                "setblock ~{} ~1 ~{} minecraft:beacon",
                offset_x, offset_z);
    }
}

void generate_files(const std::string &generate_function_name, const std::string &clear_function_name,
                    const position_data_type &target_positions) {
    generate_function_file(generate_function_name, target_positions);
    generate_clear_function_file(clear_function_name, target_positions);
}


int main() {
    int point_num = 300;
    double minimum_distance = 100;
    double maximum_distance = 150;

    auto target_positions = calculate_positions(point_num, minimum_distance, maximum_distance);
    generate_files("generate", "clear", target_positions);
    return 0;
}
