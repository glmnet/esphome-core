#ifndef ESPHOME_REMOTE_COOLIX_H
#define ESPHOME_REMOTE_COOLIX_H

#include "esphome/defines.h"

#ifdef USE_REMOTE

#include "esphome/remote/remote_receiver.h"
#include "esphome/remote/remote_transmitter.h"
#include "esphome/climate/climate_state.h"

ESPHOME_NAMESPACE_BEGIN

namespace remote {

#ifdef USE_REMOTE_TRANSMITTER
class COOLIXTransmitter : public ClimateRemoteTransmitter {
 public:
  COOLIXTransmitter(const std::string &name, uint32_t data);

  void to_data(RemoteTransmitData *data) override;

 protected:
  uint32_t data_;
};

void encode_COOLIX(RemoteTransmitData *data, uint32_t COOLIX_data);
#endif

}  // namespace remote

ESPHOME_NAMESPACE_END

#endif  // USE_REMOTE

#endif  // ESPHOME_REMOTE_COOLIX_H
