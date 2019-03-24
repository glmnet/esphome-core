#ifndef ESPHOME_CORE_IO_ARDUINO_PORT_EXPANDER_H
#define ESPHOME_CORE_IO_ARDUINO_PORT_EXPANDER_H

#include "esphome/defines.h"

#ifdef USE_ARDUINO_PORT_EXPANDER

#include "esphome/component.h"
#include "esphome/i2c_component.h"
#include "esphome/esphal.h"

ESPHOME_NAMESPACE_BEGIN

/// Modes for ARDUINO_PORT_EXPANDER pins
enum ArduinoPortExpanderGPIOMode {
  ARDUINO_PORT_EXPANDER_INPUT = INPUT,                // 0x00
  ARDUINO_PORT_EXPANDER_INPUT_PULLUP = INPUT_PULLUP,  // 0x02
  ARDUINO_PORT_EXPANDER_OUTPUT = OUTPUT,              // 0x01
};

namespace io {

class ArduinoPortExpanderGPIOInputPin;
class ArduinoPortExpanderGPIOOutputPin;

class ArduinoPortExpander : public Component, public I2CDevice {
 public:
  ArduinoPortExpander(I2CComponent *parent, uint8_t address);

  ArduinoPortExpanderGPIOInputPin make_input_pin(uint8_t pin, uint8_t mode = ARDUINO_PORT_EXPANDER_INPUT, bool inverted = false);

  ArduinoPortExpanderGPIOOutputPin make_output_pin(uint8_t pin, bool inverted = false);

  void setup() override;
  void loop() override;

  bool digital_read(uint8_t pin);
  void digital_write(uint8_t pin, bool value);
  void pin_mode(uint8_t pin, uint8_t mode);

  float get_setup_priority() const override;

 protected:
  bool digital_read_{false};
  uint8_t read_buffer_[3]{0,0,0};
  bool write_byte_(uint8_t byte_);
};

class ArduinoPortExpanderGPIOInputPin : public GPIOInputPin {
 public:
  ArduinoPortExpanderGPIOInputPin(ArduinoPortExpander *parent, uint8_t pin, uint8_t mode, bool inverted = false);

  GPIOPin *copy() const override;

  void setup() override;
  void pin_mode(uint8_t mode) override;
  bool digital_read() override;
  void digital_write(bool value) override;

 protected:
  ArduinoPortExpander *parent_;
};

class ArduinoPortExpanderGPIOOutputPin : public GPIOOutputPin {
 public:
  ArduinoPortExpanderGPIOOutputPin(ArduinoPortExpander *parent, uint8_t pin, uint8_t mode, bool inverted = false);

  GPIOPin *copy() const override;

  void setup() override;
  void pin_mode(uint8_t mode) override;
  bool digital_read() override;
  void digital_write(bool value) override;

 protected:
  ArduinoPortExpander *parent_;
};

}  // namespace io

ESPHOME_NAMESPACE_END

#endif  // USE_ARDUINO_PORT_EXPANDER

#endif  // ESPHOME_CORE_IO_ARDUINO_PORT_EXPANDER_H
