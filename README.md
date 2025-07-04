# MinecraftBeaconRainbowCircle

一个在Minecraft java版中生成彩虹信标圆的命令行工具。

A command-line tool to generate a rainbow beacon circle in Minecraft java edition.

本程序输出用的是中文，注释同样是中文

The output of this program is in Chinese, and the comments are also in Chinese.

# Usage

Just like other command-line tools, use `minecraft_beacon_circle.exe -h` to see help information.

```
> minecraft_beacon_circle -h
A generator for minecraft beacon colorful circle
Usage:
  Minecraft Beacon Circle Generator [OPTION...]

  -n, --point_num arg          The number of beacons (default: 300)
  -M, --maximum_distance arg   The maximum distance of generated beacons to
                               origin point (default: 100)
  -m, --minimum_distance arg   The minimum distance of generated beacons to
                               origin point (default: 150)
  -d, --output_directory arg   The output directory of generated files
                               (default: .)
      --package_namespace arg  The namespace of target datapack (default:
                               my_test)
  -h, --help                   Show help
```

After running, copy the 4 output files to your datapack's function directory or specify your datapack's function
directory as output directory.

In Minecraft, run

```mcfunction
/function your_datapack_namespace:generate {origin_x:"~",origin_y:"~",origin_z:"~"}
```

to build the circle.

if you want to destroy the circle, stand on the diamond block at middle and run

```mcfunction
/function your_datapack_namespace:clear {origin_x:"~",origin_y:"~",origin_z:"~"}
```

Change "~" to different words to customize your original point.

If you want to use your current position as original point, you can simply run the following commands

```mcfunction
/function your_datapack_namespace:generate_core
/function your_datapack_namespace:clear_core
```

Because **Function Macro** is added in Minecraft 1.20.2, so the first two are not available before 1.20.2, but the last 2 are
still available.

# Build

Use cmake to build the project, the building method is familiar with most other project.
run following commands to build

```shell
cmake -Bbuild
cmake --build build
./build/minecraft_beacon_circle.exe
```

I strongly recommend to use vcpkg to manage third-party libs.
For this project, run following command to prepare third-party libs

```shell
vcpkg install cxxopts tbb
```

And follow the output of `vcpkg integrate install` to configure project with cmake.

# Requirements

tbb = 2022.0.0

cxxopts = 3.3.1

I didn't use very new features, so different versions of these lib should still work.

# Thanks

Get rgb values from project [minecraft-beacon-util](https://github.com/Evolved-Cow-Man/minecraft-beacon-util)