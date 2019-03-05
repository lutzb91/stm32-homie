#include <SPI.h>
#include <Ethernet.h>
#include <Device.h>

uint8_t mac[6] = {0x74,0x69,0x69,0x2D,0x30,0x35};

EthernetClient client;

void eventHandler(const Event& event) {
  switch(event.type) {
    case EventType::EVENT_MQTT_CONNECTED:
      // Do things on Mqtt Connected
      break;
    case EventType::EVENT_MQTT_CONNECT_FAILED:
      // Do things on Mqtt Connect failed
      break;
    case EventType::EVENT_MQTT_DISCONNECTED:
      // Do things on Mqtt Disonnected
      break;
  }
}

void setup() {
  delay(500);
  Device::setFirmware("Events", "1.0.0");
  Device::setName("Event");

  if(Ethernet.begin(mac) == 0) {
    // Ethernet connection failed
    for(;;);
  }

  Device::onEvent(eventHandler);

  Device::setup(mac, Ethernet.localIP(), client);
}
void loop() {
  Device::loop();
}