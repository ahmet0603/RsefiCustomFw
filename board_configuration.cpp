/**
 * @file board_configuration.cpp
 * Mazduino Mini 6CH v1.4 (STM32F407VGT6)
 */

#include "pch.h"

#include "board_overrides.h"

Gpio getCommsLedPin() { return Gpio::Unassigned; }
Gpio getRunningLedPin() { return Gpio::Unassigned; }
Gpio getWarningLedPin() { return Gpio::Unassigned; }

static void setupEtb() {
    // TLE9201 - PWM + DIR + DIS
    // ETB1
    engineConfiguration->etbIo[0].controlPin = Gpio::A8;      // PWM
    engineConfiguration->etbIo[0].directionPin1 = Gpio::B8;   // DIR
    engineConfiguration->etbIo[0].disablePin = Gpio::B9;      // DIS

    // ETB2 (new in v1.4)
    engineConfiguration->etbIo[1].controlPin = Gpio::D14;     // PWM
    engineConfiguration->etbIo[1].directionPin1 = Gpio::D12;  // DIR
    engineConfiguration->etbIo[1].disablePin = Gpio::D11;     // DIS

    engineConfiguration->etb_use_two_wires = false;
}

static void customBoardDefaultConfiguration() {
    //config->communityCommsLedPin = Gpio::B7;

    // Trigger inputs
    engineConfiguration->triggerInputPins[0] = Gpio::D3; // CKP
    engineConfiguration->camInputs[0] = Gpio::D4;        // CMP

    // Analog sensors
    // PA3 (EFI_ADC_3) is reserved for knock via ADC3 - do not assign here
    engineConfiguration->map.sensor.hwChannel = EFI_ADC_10; // PC0
    engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;      // PA2
    engineConfiguration->afr.hwChannel = EFI_ADC_0;         // PA0 (O2)
    engineConfiguration->clt.adcChannel = EFI_ADC_4;        // PA4
    engineConfiguration->iat.adcChannel = EFI_ADC_5;        // PA5
    engineConfiguration->vbattAdcChannel = EFI_ADC_7;       // PA7
    // Spare analog inputs, not assigned here (assign in TunerStudio if needed):
    // PB0 = EFI_ADC_8, PB1 = EFI_ADC_9, PA1 = EFI_ADC_1, PC1 = EFI_ADC_11

    engineConfiguration->analogInputDividerCoefficient = (470.0f + 910.0f) / 910.0f; // 470R high side/910R low side
    engineConfiguration->vbattDividerCoeff = (82.0f + 10.0f) / 10.0f; // 82k high side/10k low side
    engineConfiguration->adcVcc = 3.3f;

    engineConfiguration->clt.config.bias_resistor = 2700;
    engineConfiguration->iat.config.bias_resistor = 2700;

    // Injectors - 6 channel
    engineConfiguration->injectionPins[0] = Gpio::D8;
    engineConfiguration->injectionPins[1] = Gpio::B15;
    engineConfiguration->injectionPins[2] = Gpio::B14;
    engineConfiguration->injectionPins[3] = Gpio::B13;
    engineConfiguration->injectionPins[4] = Gpio::D9;
    engineConfiguration->injectionPins[5] = Gpio::E8;

    // Ignition - 6 channel
    engineConfiguration->ignitionPins[0] = Gpio::E15;
    engineConfiguration->ignitionPins[1] = Gpio::E14;
    engineConfiguration->ignitionPins[2] = Gpio::D13;
    engineConfiguration->ignitionPins[3] = Gpio::E5;
    engineConfiguration->ignitionPins[4] = Gpio::E2;
    engineConfiguration->ignitionPins[5] = Gpio::E3;

    // Aux outputs
    engineConfiguration->fuelPumpPin = Gpio::C8;
    engineConfiguration->mainRelayPin = Gpio::C5;
    engineConfiguration->idle.solenoidPin = Gpio::D10;   // Idle 1
    engineConfiguration->secondSolenoidPin = Gpio::E9;   // Idle 2
    engineConfiguration->fanPin = Gpio::A15;
    engineConfiguration->tachOutputPin = Gpio::C9;
    engineConfiguration->acRelayPin = Gpio::C7;
    // High Side output (PD15, 12V switching) is not assigned here:
    // assign it to a function in TunerStudio if needed.

    // VVT and boost control (new in v1.4)
    engineConfiguration->vvtPins[0] = Gpio::B6;
    engineConfiguration->boostControlPin = Gpio::B4;

    // Switch / speed inputs
    engineConfiguration->acSwitch = Gpio::B5;
    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::D7;
    engineConfiguration->clutchDownPin = Gpio::E12;
    engineConfiguration->clutchDownPinMode = PI_INVERTED_PULLUP;
    engineConfiguration->launchActivatePin = Gpio::E13;

    // NOTE: the v1.3C stepper idle pins (PD12/PD14/PD15) are removed:
    // on v1.4 they are used by ETB2 and the High Side output.

    // CAN
    engineConfiguration->canTxPin = Gpio::D1;
    engineConfiguration->canRxPin = Gpio::D0;

    // Secondary TS channel (Bluetooth) on USART3. rusEFI defaults these to
    // PC10/PC11, which this board uses for the SD card on SPI3, so the default
    // would collide. PB10/PB11 are free on mini6ch (mazduino-lite and -core use
    // them for the LPS25 baro; mini6ch has no baro fitted).
    // Wiring: PB10 -> HC-05 RXD, PB11 <- HC-05 TXD.
    engineConfiguration->binarySerialTxPin = Gpio::B10;
    engineConfiguration->binarySerialRxPin = Gpio::B11;

    // SD card - SPI3
    engineConfiguration->is_enabled_spi_3 = true;
    engineConfiguration->spi3mosiPin = Gpio::C12;
    engineConfiguration->spi3misoPin = Gpio::C11;
    engineConfiguration->spi3sckPin = Gpio::C10;
    engineConfiguration->isSdCardEnabled = true;
    engineConfiguration->sdCardSpiDevice = SPI_DEVICE_3;
    engineConfiguration->sdCardCsPin = Gpio::D2;

    // Electronic throttle - TLE9201
    setupEtb();
}

void setup_custom_board_overrides() {
    custom_board_DefaultConfiguration = customBoardDefaultConfiguration;
}
