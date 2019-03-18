#ifndef ESPHOME_CLIMATE_MQTT_CLIMATE_COMPONENT_H
#define ESPHOME_CLIMATE_MQTT_CLIMATE_COMPONENT_H

#include "esphome/defines.h"

#ifdef USE_MQTT_CLIMATE

#include "esphome/mqtt/mqtt_component.h"
#include "esphome/climate/climate_state.h"

ESPHOME_NAMESPACE_BEGIN

namespace climate {

class MQTTClimateComponent : public mqtt::MQTTComponent {
 public:
  explicit MQTTClimateComponent(ClimateState *state);

  /// Set a custom oscillation command topic. Defaults to "<base>/oscillation/command".
  void set_custom_oscillation_command_topic(const std::string &topic);
  /// Set a custom oscillation state topic. Defaults to "<base>/oscillation/state".
  void set_custom_oscillation_state_topic(const std::string &topic);
  /// Set a custom speed command topic. Defaults to "<base>/speed/command".
  void set_custom_speed_command_topic(const std::string &topic);
  /// Set a custom speed state topic. Defaults to "<base>/speed/state".
  void set_custom_speed_state_topic(const std::string &topic);

  void send_discovery(JsonObject &root, mqtt::SendDiscoveryConfig &config) override;

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Setup the climate subscriptions and discovery.
  void setup() override;
  /// Send the full current state to MQTT.
  bool send_initial_state() override;
  bool publish_state();
  /// 'climate' component type for discovery.
  std::string component_type() const override;

  const std::string get_oscillation_command_topic() const;
  const std::string get_oscillation_state_topic() const;
  const std::string get_speed_command_topic() const;
  const std::string get_speed_state_topic() const;

  ClimateState *get_state() const;

  bool is_internal() override;

 protected:
  std::string friendly_name() const override;

  std::string custom_oscillation_command_topic_;
  std::string custom_oscillation_state_topic_;
  std::string custom_speed_command_topic_;
  std::string custom_speed_state_topic_;
  ClimateState *state_;
};

}  // namespace climate

ESPHOME_NAMESPACE_END

#endif  // USE_MQTT_CLIMATE

#endif  // ESPHOME_CLIMATE_MQTT_CLIMATE_COMPONENT_H
