#include "esphome/defines.h"

#ifdef USE_ARDUINO_PORT_EXPANDER

#include "esphome/io/arduino_port_expander.h"
#include "esphome/log.h"

ESPHOME_NAMESPACE_BEGIN

namespace io {

static const char *TAG = "io.arduinope";

ArduinoPortExpander::ArduinoPortExpander(I2CComponent *parent, uint8_t address) : Component(), I2CDevice(parent, address) {}
ArduinoPortExpanderGPIOInputPin ArduinoPortExpander::make_input_pin(uint8_t pin, uint8_t mode, bool inverted) {
  return {this, pin, mode, inverted};
}
ArduinoPortExpanderGPIOOutputPin ArduinoPortExpander::make_output_pin(uint8_t pin, bool inverted) {
  return {this, pin, ARDUINO_PORT_EXPANDER_OUTPUT, inverted};
}
void ArduinoPortExpander::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ArduinoPortExpander... waiting up to 3 secs");

  /* We cannot setup as usual as arduino boots later than esp8266 
  
    Poll i2c bus for our Arduino for a n seconds instead of failing fast,
    also this is important as pin setup (INPUT_PULLUP, OUTPUT it's done once)
  */
  /**/
  unsigned long start = millis();
  
  while (millis() - start < 3000) // Usually is alive before 200 milliseconds
  {
    if (this->read_bytes(0, const_cast<uint8_t*>(this->read_buffer_), 3, 1)) {
      ESP_LOGCONFIG(TAG, "ArduinoPortExpander found at %d in %d millis", address_, millis() - start);
      return;
    }    
    delay(20);
  }
  ESP_LOGE(TAG, "ArduinoPortExpander NOT found at %d in %d millis", address_, millis() - start);
  this->mark_failed();
}

void ArduinoPortExpander::loop() {
  // reset bit read digital inputs done for this loop
  this->digital_read_ = false;
}
bool ArduinoPortExpander::digital_read(uint8_t pin) {
  if (!this->digital_read_)
  {
    this->read_bytes(0, const_cast<uint8_t*>(this->read_buffer_), 3, 1);
    this->digital_read_ = true;
  }
  uint8_t bit = pin % 8;
  uint8_t value = pin < 8 ? this->read_buffer_[0] :
                    pin < 16 ? this->read_buffer_[1] :
                    this->read_buffer_[2];
  bool ret = value & (1 << bit);
  return ret;
}

const uint8_t CMD_ANALOG_READ        = 0b00000000;
const uint8_t CMD_SETUP_OUTPUT       = 0b00100000;
const uint8_t CMD_SETUP_INPUT_PULLUP = 0b01000000;
const uint8_t CMD_SETUP_INPUT        = 0b01100000;
const uint8_t CMD_WRITE_DIGITAL      = 0b10000000;

const uint8_t DigitalBitFalse = 0b00000000;
const uint8_t DigitalBitTrue = 0b00100000;

void ArduinoPortExpander::digital_write(uint8_t pin, bool value) {
  this->write_byte_(CMD_WRITE_DIGITAL | pin | (value ? DigitalBitTrue : DigitalBitFalse));
}

void ArduinoPortExpander::pin_mode(uint8_t pin, uint8_t mode) {
  ESP_LOGD(TAG, "Set digital pin %d mode %d", pin, mode);
  switch (mode) {
    case ARDUINO_PORT_EXPANDER_INPUT:
      this->write_byte_(pin | CMD_SETUP_INPUT);
      break;
    case ARDUINO_PORT_EXPANDER_INPUT_PULLUP:
      this->write_byte_(pin | CMD_SETUP_INPUT_PULLUP);
      break;
    case ARDUINO_PORT_EXPANDER_OUTPUT:
      this->write_byte_(pin | CMD_SETUP_OUTPUT);
      break;
    default:
      break;
  }
}
float ArduinoPortExpander::get_setup_priority() const { return setup_priority::HARDWARE; }

bool ArduinoPortExpander::write_byte_(uint8_t byte_) {
  if (this->is_failed())
    return false;
  return this->write_bytes(byte_, NULL, 0);
}

ArduinoPortExpanderGPIOInputPin::ArduinoPortExpanderGPIOInputPin(ArduinoPortExpander *parent, uint8_t pin, uint8_t mode, bool inverted)
    : GPIOInputPin(pin, mode, inverted), parent_(parent) {}
GPIOPin *ArduinoPortExpanderGPIOInputPin::copy() const { return new ArduinoPortExpanderGPIOInputPin(*this); }
void ArduinoPortExpanderGPIOInputPin::setup() { this->pin_mode(this->mode_); }
void ArduinoPortExpanderGPIOInputPin::pin_mode(uint8_t mode) { this->parent_->pin_mode(this->pin_, mode); }
bool ArduinoPortExpanderGPIOInputPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void ArduinoPortExpanderGPIOInputPin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}
ArduinoPortExpanderGPIOOutputPin::ArduinoPortExpanderGPIOOutputPin(ArduinoPortExpander *parent, uint8_t pin, uint8_t mode, bool inverted)
    : GPIOOutputPin(pin, mode, inverted), parent_(parent) {}
GPIOPin *ArduinoPortExpanderGPIOOutputPin::copy() const { return new ArduinoPortExpanderGPIOOutputPin(*this); }
void ArduinoPortExpanderGPIOOutputPin::setup() { this->pin_mode(this->mode_); }
void ArduinoPortExpanderGPIOOutputPin::pin_mode(uint8_t mode) { this->parent_->pin_mode(this->pin_, mode); }
bool ArduinoPortExpanderGPIOOutputPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void ArduinoPortExpanderGPIOOutputPin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}
}  // namespace io

ESPHOME_NAMESPACE_END

#endif  // USE_ARDUINO_PORT_EXPANDER
