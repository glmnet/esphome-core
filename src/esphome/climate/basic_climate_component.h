#ifndef ESPHOME_CLIMATE_BASIC_CLIMATE_COMPONENT_H
#define ESPHOME_CLIMATE_BASIC_CLIMATE_COMPONENT_H

#include "esphome/defines.h"

#ifdef USE_CLIMATE

#include "esphome/component.h"
#include "esphome/helpers.h"
#include "esphome/automation.h"
#include "esphome/climate/climate_state.h"
#include "esphome/remote/remote_transmitter.h"

ESPHOME_NAMESPACE_BEGIN

namespace climate {

/// Simple fan helper that pushes the states to BinaryOutput/FloatOutput devices.
class BasicClimateComponent : public Component {
 public:
  /** Create a fan that supports binary state operation (ON/OFF). Can't be mixed with set_speed.
   *
   * @param output The binary output where all binary commands should land.
   */
  //void set_binary(output::BinaryOutput *output);

  /** Create a fan that supports speed operation (OFF/LOW/MEDIUM/HIGH SPEED). Can't be mixed with set_binary.
   *
   * @param output The FloatOutput where all speed/state commands should land.
   * @param low_speed The speed that should be sent to the output if the fan is in LOW speed mode.
   * @param medium_speed The speed that should be sent to the output if the fan is in MEDIUM speed mode.
   * @param high_speed The speed that should be sent to the output if the fan is in HIGH speed mode.
   */
  // void set_speed(output::FloatOutput *output, float low_speed = 0.33, float medium_speed = 0.66,
  //                float high_speed = 1.0);

  /** Create a climate that supports ClimateRemoteTransmitter
   *  
   */
  void set_climate_remote(remote::ClimateRemoteTransmitter *remote);

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  ClimateState *get_state() const;
  void set_state(ClimateState *state);
  void setup() override;
  void dump_config() override;
  void loop() override;

  float get_setup_priority() const override;

 protected:
  ClimateState *state_{nullptr};
  remote::ClimateRemoteTransmitter *climate_remote_{nullptr};
  
  bool next_update_{true};
};

}  // namespace climate

ESPHOME_NAMESPACE_END

#include "esphome/climate/mqtt_climate_component.h"

#endif  // USE_CLIMATE

#endif  // ESPHOME_CLIMATE_BASIC_CLIMATE_COMPONENT_H