#include <iostream>
#include <filesystem>
#include <fstream>
#include <complex>
#include <cmath>

constexpr auto WIDTH = 1080;
constexpr auto HEIGHT = 640;
constexpr auto FILE_SIZE = WIDTH * HEIGHT;
constexpr auto RAD = 50;
constexpr auto SCALE_FACTOR = 0.004;

void writeToFile(std::array<int, FILE_SIZE> &data) {
    const std::filesystem::path path = "./out.ppm";
    std::ofstream out{path};
    out << "P6\n" << WIDTH << " " << HEIGHT << " " << "255\n";
    for (int i = 0; i < FILE_SIZE; ++i) {
        const char pixel[3] = {
                static_cast<const char>(data[i] >> (8 * 2) & 0xFF),
                static_cast<const char>(data[i] >> (8 * 1) & 0xFF),
                static_cast<const char>(data[i] >> (8 * 0) & 0xFF)
        };
        out.write(pixel, 3);
    }
    out.close();
}


[[maybe_unused]] void fillCircle(std::array<int, FILE_SIZE> &data) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int dx = HEIGHT / 2 - i;
            int dy = WIDTH / 2 - j;
            if (dx*dx + dy*dy <= RAD*RAD) {
                data[i* WIDTH + j] = 0xff0000;
            } else {
                data[i* WIDTH + j] = 0x0000ff;
            }
        }
    }
}

int isInMandelbrot(std::complex<double> c) {
    std::complex z(0.0, 0.0);
    for (int i = 0; i < 255; ++i) {
        z = z*z + c;
        if (abs(z) >= 2) return i;
    }
    return 0;
}

void fillMandelbrot(std::array<int, FILE_SIZE> &data) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            double dx = (720 - j) * SCALE_FACTOR;
            double dy = (320 - i) * SCALE_FACTOR;
            std::complex<double> number = std::complex(dx, dy);
            const unsigned char intensity = isInMandelbrot(-number);
            auto n = (double)intensity;
            double a = 0.1;
            // Stolen from https://github.com/OneLoneCoder/olcPixelGameEngine/blob/5764bbab19efd52b9dfa4144390c9f47edfe7dc3/Videos/OneLoneCoder_PGE_Mandelbrot.cpp#L543
            data[i* WIDTH + j] = (int((0.5 * sin(a * n) + 0.5) * 255) << (8 * 2)) +
                                 (int((0.5 * sin(a * n + 1) + 0.5f) * 255) << (8 * 1)) +
                                 (int((0.5 * sin(a * n + 2) + 0.5f) * 255) << (8 * 0));
        }
    }
}
int main() {
    std::array<int, FILE_SIZE> pixels{};
    fillMandelbrot(pixels);
    writeToFile(pixels);
    return 0;
}
