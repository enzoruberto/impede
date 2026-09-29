//
// Created on 9/29/2026.
//

#pragma once

#include "UNITS.hpp"

class Capacitor {
public:
    explicit Capacitor(const int_fast32_t capacitance_pF) : _capacitance_pF(capacitance_pF) {};

    /**
     * @brief Calculates the current through the capacitor based on the voltage and time since last sample
     * @param currentVoltage_mV
     * @param deltaTime_uS
     * @return Current through the capacitor in nanoamps
     * @see https://duckduckgo.com/?q=capaciotr%20equations&ia=images&iax=images&iai=https%3A%2F%2Fwww.cyberphysics.co.uk%2FQ%26A%2FKS5%2Felectricity%2Fcapacitors%2Fequations.png
     * @see https://duckduckgo.com/?q=capaciotr%20current&iar=images&iai=https%3A%2F%2Fimage3.slideserve.com%2F6092107%2Frelationship-between-capacitor-voltage-and-current-l.jpg
    */
    int_fast32_t getCurrent_nA(const int_fast32_t currentVoltage_mV, const uint_fast32_t deltaTime_uS) {
        // dV = V2 - V1
        const int_fast32_t deltaVoltage_mV = currentVoltage_mV - _previousVoltage_mV;
        // dC = dV * C
        // mV * pF = 10^-3 V * 10^-12 F = 10^-15 C = 10^-12 mF = 10^-9 uF = 10^-6 nC = 10^-3 pC = 10^0 fC
        // charge_fC += (deltaVoltage_mV * capacitance_pF);
        _previousVoltage_mV = currentVoltage_mV;

        // I = C * dV/dt
        // 10^-12 F * 10^-3 V / (10^-6 s) = 10^-15 / 10^-6 A = 10^-9 A = 10^-6 mA = 10^3 uA = 10^0 nA
        return _capacitance_pF * deltaVoltage_mV / deltaTime_uS;
    }

private:
    const int_fast32_t _capacitance_pF;
    uint_fast32_t _previousVoltage_mV = 0;
    // uint_fast32_t _charge_fC = 0;
};
