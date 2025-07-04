/*
    参考自https://www.programmingalgorithms.com/algorithm/hsl-to-rgb/cpp/
*/
#include "Color.h"


static float HueToRGB(float v1, float v2, float vH) {
    if (vH < 0)
        vH += 1;

    if (vH > 1)
        vH -= 1;

    if ((6 * vH) < 1)
        return (v1 + (v2 - v1) * 6 * vH);

    if ((2 * vH) < 1)
        return v2;

    if ((3 * vH) < 2)
        return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

    return v1;
}

RGBColor HSLtoRGB(const HSLColor &hsl) {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    if (hsl.saturation == 0) {
        r = g = b = (unsigned char) (hsl.luminance * 255);
    } else {
        float v1, v2;
        float hue = (float) hsl.hue / 360;

        v2 = (hsl.luminance < 0.5)
                 ? (hsl.luminance * (1 + hsl.saturation))
                 : ((hsl.luminance + hsl.saturation) - (hsl.luminance * hsl.saturation));
        v1 = 2 * hsl.luminance - v2;

        r = (unsigned char) (255 * HueToRGB(v1, v2, hue + (1.0f / 3)));
        g = (unsigned char) (255 * HueToRGB(v1, v2, hue));
        b = (unsigned char) (255 * HueToRGB(v1, v2, hue - (1.0f / 3)));
    }

    return RGBColor(r, g, b);
}
