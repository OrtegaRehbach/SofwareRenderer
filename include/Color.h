#pragma once

#include <iostream>
#include <cstdint> // Include for Uint8 definition

using Uint8 = uint8_t; // Type alias for unsigned 8-bit integer

struct Color {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;

    Color() : red(255), green(255), blue(255), alpha(255) {}

    Color(int r, int g, int b, int a = 255) {
        red =   static_cast<Uint8>((r > 255) ? 255 : ((r < 0) ? 0 : r));
        green = static_cast<Uint8>((g > 255) ? 255 : ((g < 0) ? 0 : g));
        blue =  static_cast<Uint8>((b > 255) ? 255 : ((b < 0) ? 0 : b));
        alpha = static_cast<Uint8>((a > 255) ? 255 : ((a < 0) ? 0 : a));
    }

    Color(float r, float g, float b, float a = 1.0f)
        : red(static_cast<Uint8>(r * 255.0f)),
          green(static_cast<Uint8>(g * 255.0f)),
          blue(static_cast<Uint8>(b * 255.0f)),
          alpha(static_cast<Uint8>(a * 255.0f)) {
    
        red = (red > 255) ? 255 : ((red < 0) ? 0 : red);
        green = (green > 255) ? 255 : ((green < 0) ? 0 : green);
        blue = (blue > 255) ? 255 : ((blue < 0) ? 0 : blue);
        alpha = (alpha > 255) ? 255 : ((alpha < 0) ? 0 : alpha);
    }

    Color operator+(const Color& other) const {
        int r = static_cast<int>(red) + static_cast<int>(other.red);
        int g = static_cast<int>(green) + static_cast<int>(other.green);
        int b = static_cast<int>(blue) + static_cast<int>(other.blue);
        int a = static_cast<int>(alpha) + static_cast<int>(other.alpha);

        r = (r > 255) ? 255 : ((r < 0) ? 0 : r);
        g = (g > 255) ? 255 : ((g < 0) ? 0 : g);
        b = (b > 255) ? 255 : ((b < 0) ? 0 : b);
        a = (a > 255) ? 255 : ((a < 0) ? 0 : a);

        return Color(r, g, b, a);
    }

    Color operator*(float scalar) const {
        Uint8 r = static_cast<Uint8>(std::max(0.0f, std::min(red * scalar, 255.0f)));
        Uint8 g = static_cast<Uint8>(std::max(0.0f, std::min(green * scalar, 255.0f)));
        Uint8 b = static_cast<Uint8>(std::max(0.0f, std::min(blue * scalar, 255.0f)));
        Uint8 a = 255;
        return Color(r, g, b, a);
    }

    Color operator*(Color other) const {
        Uint8 r = static_cast<Uint8>((red * other.red) / 255);
        Uint8 g = static_cast<Uint8>((green * other.green) / 255);
        Uint8 b = static_cast<Uint8>((blue * other.blue) / 255);
        Uint8 a = static_cast<Uint8>((alpha * other.alpha) / 255);

        return Color(r, g, b, a);
    }

    bool operator==(const Color& other) const {
        return red == other.red && green == other.green && blue == other.blue && alpha == other.alpha;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Color& color) {
        os << "Color(" 
        << static_cast<int>(color.red) << ", "
        << static_cast<int>(color.green) << ", "
        << static_cast<int>(color.blue) << ", "
        << static_cast<int>(color.alpha) << ")";
        return os;
    }
};
