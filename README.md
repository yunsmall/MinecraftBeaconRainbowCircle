# MinecraftBeaconRainbowCircle

一个在Minecraft java版中生成彩虹信标圆的命令行工具。

A command-line tool to generate a rainbow beacon circle in Minecraft java edition.

This program has done multiple language translation, including English and Chinese.

# Usage

Just like other command-line tools, use `minecraft_beacon_circle.exe -h` to see help information.

```
> minecraft_beacon_circle -h
A generator to generate mcfunctions for creating Minecraft rainbow beacon circles
Usage:
  Minecraft Beacon Circle Generator [OPTION...]

  -n, --point_num arg          Number of beacons (default: 300)
  -M, --maximum_distance arg   Maximum distance from the origin (default:
                               150)
  -m, --minimum_distance arg   Minimum distance from the origin (default:
                               100)
  -d, --output_directory arg   Output directory of generated files
                               (default: .)
      --package_namespace arg  Packet namespace (default: my_test)
  -h, --help                   Show Help
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
cmake -Bbuild -DCMAKE_INSTALL_PREFIX=install
cmake --build build
cmake --install build
```

The translation file will be installed in the same directory as the executable.

I strongly recommend to use vcpkg to manage third-party libs.
For this project, run following command to prepare third-party libs

```shell
vcpkg install cxxopts tbb gettext
```

And follow the output of `vcpkg integrate install` to configure project with cmake.

# Requirements

tbb = 2022.0.0
gettext = 0.22.5
cxxopts = 3.3.1

I didn't use very new features, so different versions of these lib should still work.

# Thanks

Get rgb values from project [minecraft-beacon-util](https://github.com/Evolved-Cow-Man/minecraft-beacon-util)