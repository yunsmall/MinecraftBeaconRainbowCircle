#include "BeaconColorCalculator.h"

#include <iostream>

#include <tbb/tbb.h>

BeaconColorCalculator::BeaconColorCalculator() {
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, std::size(all_mc_glasses)),
                      [this](const tbb::blocked_range<std::size_t> &range) {
                          glasses_type glasses;
                          for (auto idx = range.begin(); idx != range.end(); ++idx) {
                              auto current_data_vector_idx = powi(std::size(all_mc_glasses), max_search_num - 1) * idx;
                              glasses[0] = all_mc_glasses[idx];
                              dfs_generate_block(current_data_vector_idx, 1, glasses);
                              // colors[0] = all_mcglasses[idx];
                              // for (auto glass_2: all_mcglasses) {
                              //     colors[1] = glass_2;
                              //     for (auto glass_3: all_mcglasses) {
                              //         colors[2] = glass_3;
                              //         for (auto glass_4: all_mcglasses) {
                              //             colors[3] = glass_4;
                              //             for (auto glass_5: all_mcglasses) {
                              //                 colors[4] = glass_5;
                              //                 auto color = glasses_to_rgb(colors);
                              //
                              //                 color_vector[](color, colors);
                              //             }
                              //         }
                              //     }
                              // }
                          }
                      });
};

BeaconColorCalculator::glasses_type BeaconColorCalculator::calculate_colors(RGBColor target_color) const {
    using compare_data_type = std::pair<glasses_type, double>;
    auto minimum_colors = tbb::parallel_reduce(
            tbb::blocked_range(color_vector.begin(), color_vector.end()),
            compare_data_type({}, std::numeric_limits<double>::max()),
            [&](const auto &range, compare_data_type v) {
                for (auto it = range.begin(); it != range.end(); ++it) {
                    auto &current_colors = *it;
                    auto color = glasses_to_rgb(current_colors.second);
                    auto current_distance = distance(target_color, color);
                    if (current_distance < v.second) {
                        v.first = it->second;
                        v.second = current_distance;
                    }
                }
                return v;
            },
            [&](const compare_data_type &a, const compare_data_type &b) {
                return std::min(a, b, [&](const compare_data_type &a, const compare_data_type &b) {
                    auto distance_a = distance(glasses_to_rgb(a.first), target_color);
                    auto distance_b = distance(glasses_to_rgb(b.first), target_color);
                    return distance_a < distance_b;
                });
            }
            );

    // auto minimum_colors = tbb::parallel_reduce(
    //         tbb::blocked_range<std::size_t>(0, std::size(all_mcglasses)),
    //         parallel_for_type({}, std::numeric_limits<double>::max()),
    //         [&](const tbb::blocked_range<std::size_t> &range, parallel_for_type v) {
    //             glasses_type colors;
    //             for (auto idx = range.begin(); idx != range.end(); idx++) {
    //                 colors[0] = all_mcglasses[idx];
    //                 for (auto glass_2: all_mcglasses) {
    //                     colors[1] = glass_2;
    //                     for (auto glass_3: all_mcglasses) {
    //                         colors[2] = glass_3;
    //                         for (auto glass_4: all_mcglasses) {
    //                             colors[3] = glass_4;
    //                             for (auto glass_5: all_mcglasses) {
    //                                 colors[4] = glass_5;
    //                                 auto color = glasses_to_rgb(colors);
    //                                 auto current_distance = distance(target_color, color);
    //                                 if (current_distance < v.second) {
    //                                     v.first = colors;
    //                                     v.second = current_distance;
    //                                 }
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //             return v;
    //         },
    //         [&](const parallel_for_type &a, const parallel_for_type &b) {
    //             return std::min(a, b, [&](const parallel_for_type &a, const parallel_for_type &b) {
    //                 auto distance_a = distance(glasses_to_rgb(a.first), target_color);
    //                 auto distance_b = distance(glasses_to_rgb(b.first), target_color);
    //                 return distance_a < distance_b;
    //             });
    //         }
    //         );
    return minimum_colors.first;

    // for (auto glass_1: all_mcglasses) {
    //     // std::println(std::cout, "当前搜索的第一个玻璃为{}", glass_to_mc_name(glass_1));
    //     colors[0] = glass_1;
    //     for (auto glass_2: all_mcglasses) {
    //         colors[1] = glass_2;
    //         for (auto glass_3: all_mcglasses) {
    //             colors[2] = glass_3;
    //             for (auto glass_4: all_mcglasses) {
    //                 colors[3] = glass_4;
    //                 for (auto glass_5: all_mcglasses) {
    //                     colors[4] = glass_5;
    //                     auto color = glasses_to_rgb(colors);
    //                     auto current_distance = distance(target_color, color);
    //                     if (current_distance < minimum_distance) {
    //                         minimum_distance = current_distance;
    //                         minimum_colors = colors;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // return std::ranges::min(color_vector,
    //                         [&](const decltype(color_vector)::value_type &a,
    //                             const decltype(color_vector)::value_type &b)-> bool {
    //                             auto distance_a = distance(glasses_to_rgb(a.second), target_color);
    //                             auto distance_b = distance(glasses_to_rgb(b.second), target_color);
    //                             return distance_a < distance_b;
    //                         }).second;

}

void BeaconColorCalculator::dfs_generate_block(int data_vector_start_idx, int current_glass_num,
                                               glasses_type &glasses) {
    if (current_glass_num == max_search_num) {
        color_vector[data_vector_start_idx] = std::make_pair(glasses_to_rgb(glasses), glasses);
        return;
    }

    for (const auto &[i,glass]: std::views::enumerate(all_mc_glasses)) {
        auto current_idx = data_vector_start_idx + powi(std::size(all_mc_glasses),
                                                        max_search_num - current_glass_num - 1) * i;
        glasses[current_glass_num] = glass;
        dfs_generate_block(current_idx, current_glass_num + 1, glasses);
    }
}
