#ifndef ESPHOME_CLIMATE_CLIMATE_STATE_H
#define ESPHOME_CLIMATE_CLIMATE_STATE_H

#include "esphome/defines.h"

#ifdef USE_CLIMATE

#include "esphome/component.h"
#include "esphome/automation.h"
#include "esphome/esppreferences.h"
//#include "esphome/climate/climate_traits.h"

ESPHOME_NAMESPACE_BEGIN

namespace climate {

enum Mode {
  MODE_OFF = 0,
  MODE_HEAT = 1,
  MODE_COOL = 2,
  MODE_DRY = 3,
  MODE_FAN = 4
};

/// Simple enum to represent the speed of a fan.
enum FanSpeed {
  FAN_SPEED_LOW = 0,     ///< The fan is running on low speed.
  FAN_SPEED_MEDIUM = 1,  ///< The fan is running on medium speed.
  FAN_SPEED_HIGH = 2     ///< The fan is running on high/full speed.
};

template<typename... Ts> class TurnOffAction;
template<typename... Ts> class TurnHeatAction;
template<typename... Ts> class TurnColdAction;
template<typename... Ts> class TurnDryAction;
template<typename... Ts> class TurnFanAction;

#ifdef USE_MQTT_CLIMATE
class MQTTClimateComponent;
#endif

class ClimateState : public Nameable, public Component {
 public:
  /// Construct the climate state with name.
  explicit ClimateState(const std::string &name);

  /// Register a callback that will be called each time the state changes.
  void add_on_state_callback(std::function<void()> &&callback);

  /// Get the traits of this climate (i.e. what features it supports).
  //const ClimateTraits &get_traits() const;
  /// Set the traits of this climate (i.e. what features it supports).
  //void set_traits(const ClimateTraits &traits);

  template<typename... Ts> TurnOffAction<Ts...> *make_turn_off_action();
  template<typename... Ts> TurnHeatAction<Ts...> *make_turn_heat_action();
  template<typename... Ts> TurnColdAction<Ts...> *make_turn_cold_action();
  template<typename... Ts> TurnDryAction<Ts...> *make_turn_dry_action();
  template<typename... Ts> TurnFanAction<Ts...> *make_turn_fan_action();

  /// The current mode of the climate.
  Mode mode{MODE_OFF};
  /// The current fan speed.
  FanSpeed speed{FAN_SPEED_HIGH};
  /// The current target temperature

  class StateCall {
   public:
    explicit StateCall(ClimateState *state);

    ClimateState::StateCall &set_mode(Mode mode);
    ClimateState::StateCall &set_mode(optional<Mode> mode);
    ClimateState::StateCall &set_speed(FanSpeed speed);
    ClimateState::StateCall &set_speed(optional<FanSpeed> speed);
    ClimateState::StateCall &set_speed(const char *speed);

    void perform() const;

   protected:
    ClimateState *const state_;
    optional<Mode> mode_;
    optional<FanSpeed> speed_{};
  };

  ClimateState::StateCall turn_off();
  ClimateState::StateCall turn_heat();
  ClimateState::StateCall turn_cold();
  ClimateState::StateCall turn_dry();
  ClimateState::StateCall turn_fan();
  ClimateState::StateCall make_call();

  void setup() override;
  float get_setup_priority() const override;

#ifdef USE_MQTT_CLIMATE
  MQTTClimateComponent *get_mqtt() const;
  void set_mqtt(MQTTClimateComponent *mqtt);
#endif

 protected:
  uint32_t hash_base() override;

  //FanTraits traits_{};
  CallbackManager<void()> state_callback_{};
  ESPPreferenceObject rtc_;
#ifdef USE_MQTT_CLIMATE
  MQTTClimateComponent *mqtt_{nullptr};
#endif
};

template<typename... Ts> class TurnOffAction : public Action<Ts...> {
 public:
  explicit TurnOffAction(ClimateState *state);

  void play(Ts... x) override;

 protected:
  ClimateState *state_;
};

template<typename... Ts> class TurnHeatAction : public Action<Ts...> {
 public:
  explicit TurnHeatAction(ClimateState *state);

  template<typename V> void set_mode(V value) { this->mode_ = value; }
  template<typename V> void set_speed(V value) { this->speed_ = value; }

  void play(Ts... x) override;

 protected:
  ClimateState *state_;
  TemplatableValue<Mode, Ts...> mode_;
  TemplatableValue<FanSpeed, Ts...> speed_;
};
template<typename... Ts> class TurnColdAction : public Action<Ts...> {
 public:
  explicit TurnColdAction(ClimateState *state);

  template<typename V> void set_mode(V value) { this->mode_ = value; }
  template<typename V> void set_speed(V value) { this->speed_ = value; }

  void play(Ts... x) override;

 protected:
  ClimateState *state_;
  TemplatableValue<Mode, Ts...> mode_;
  TemplatableValue<FanSpeed, Ts...> speed_;
};
template<typename... Ts> class TurnDryAction : public Action<Ts...> {
 public:
  explicit TurnDryAction(ClimateState *state);

  template<typename V> void set_mode(V value) { this->mode_ = value; }
  template<typename V> void set_speed(V value) { this->speed_ = value; }

  void play(Ts... x) override;

 protected:
  ClimateState *state_;
  TemplatableValue<Mode, Ts...> mode_;
  TemplatableValue<FanSpeed, Ts...> speed_;
};
template<typename... Ts> class TurnFanAction : public Action<Ts...> {
 public:
  explicit TurnFanAction(ClimateState *state);

  template<typename V> void set_mode(V value) { this->mode_ = value; }
  template<typename V> void set_speed(V value) { this->speed_ = value; }

  void play(Ts... x) override;

 protected:
  ClimateState *state_;
  TemplatableValue<Mode, Ts...> mode_;
  TemplatableValue<FanSpeed, Ts...> speed_;
};

template<typename... Ts> TurnOffAction<Ts...>::TurnOffAction(ClimateState *state) : state_(state) {}
template<typename... Ts> void TurnOffAction<Ts...>::play(Ts... x) {
  this->state_->turn_off().perform();
  this->play_next(x...);
}

template<typename... Ts> TurnHeatAction<Ts...>::TurnHeatAction(ClimateState *state) : state_(state) {}
template<typename... Ts> void TurnHeatAction<Ts...>::play(Ts... x) {
  auto call = this->state_->turn_heat();
  if (this->speed_.has_value()) {
    call.set_speed(this->speed_.value(x...));
  }
  call.perform();
  this->play_next(x...);
}
template<typename... Ts> TurnColdAction<Ts...>::TurnColdAction(ClimateState *state) : state_(state) {}
template<typename... Ts> void TurnColdAction<Ts...>::play(Ts... x) {
  auto call = this->state_->turn_cold();
  if (this->speed_.has_value()) {
    call.set_speed(this->speed_.value(x...));
  }
  call.perform();
  this->play_next(x...);
}
template<typename... Ts> TurnDryAction<Ts...>::TurnDryAction(ClimateState *state) : state_(state) {}
template<typename... Ts> void TurnDryAction<Ts...>::play(Ts... x) {
  auto call = this->state_->turn_dry();
  if (this->speed_.has_value()) {
    call.set_speed(this->speed_.value(x...));
  }
  call.perform();
  this->play_next(x...);
}
template<typename... Ts> TurnFanAction<Ts...>::TurnFanAction(ClimateState *state) : state_(state) {}
template<typename... Ts> void TurnFanAction<Ts...>::play(Ts... x) {
  auto call = this->state_->turn_fan();
  if (this->speed_.has_value()) {
    call.set_speed(this->speed_.value(x...));
  }
  call.perform();
  this->play_next(x...);
}

template<typename... Ts> TurnOffAction<Ts...> *ClimateState::make_turn_off_action() { return new TurnOffAction<Ts...>(this); }
template<typename... Ts> TurnHeatAction<Ts...> *ClimateState::make_turn_heat_action() { return new TurnHeatAction<Ts...>(this); }
template<typename... Ts> TurnColdAction<Ts...> *ClimateState::make_turn_cold_action() { return new TurnColdAction<Ts...>(this); }
template<typename... Ts> TurnDryAction<Ts...> *ClimateState::make_turn_dry_action() { return new TurnDryAction<Ts...>(this); }
template<typename... Ts> TurnFanAction<Ts...> *ClimateState::make_turn_fan_action() { return new TurnFanAction<Ts...>(this); }

}  // namespace climate

ESPHOME_NAMESPACE_END

#include "esphome/climate/mqtt_climate_component.h"

#endif  // USE_CLIMATE

#endif  // ESPHOME_CLIMATE_CLIMATE_STATE_H
