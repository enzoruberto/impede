#include <cstdint>
#include <print>
#include <array>
#include <cmath>
#include <random>

#include "Capacitor.hpp"
#include "Units.hpp"

int main(int argc, char* argv[]) {
    std::println("Capacitor Simulation");
    std::println("Usage: impede <input CSV> <output CSV>");
    std::println("Input CSV format: capacitance, voltage_mV");
    // std::println("Input file: {}", argv[1]); // TODO: Check if valid
    // std::println("Output file: {}", argv[2])); // TODO: Check if valid

    // TODO: Read input CSV file

    // Demo:
    constexpr int_fast32_t CAPACITANCE_pF = 1000; // 1000 pF
    Capacitor capacitor(CAPACITANCE_pF);
    std::array<int_fast32_t, 1000> input_mV{};
    // Fill array with a sine wave
    for (int_fast32_t entry : input_mV) {
        // Simulate a sine wave
        entry = static_cast<int_fast32_t>(1000 * std::sin(2 * M_PI * entry / 1000));
        // Add noise
        entry += static_cast<int_fast32_t>(std::rand() % 100 - 50); // TODO: Switch to C++11 random

        std::println("current (nA): {}", capacitor.getCurrent_nA(entry, 1));
    }
}
