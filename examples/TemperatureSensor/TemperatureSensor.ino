#include <SPI.h>
#include <Ethernet.h>
#include <Device.h>

uint8_t mac[6] = {0x74,0x69,0x69,0x2D,0x30,0x35};

EthernetClient client;

/* Nodes */
Node tempOutside("out", "Sensor Outside", "sensor");
Node tempInside("in", "Sensor Inside", "sensor");

void setup() {
  delay(500);
  Device::setFirmware("TemperatureSensor", "1.0.0");
  Device::setName("Temp1");

  tempOutside.exposeProperty("temperature").setName("Temperature Outside").setDatatype(Datatype::FLOAT).setUnit("°C");
  tempOutside.exposeProperty("humidity").setName("Humidity Outside").setDatatype(Datatype::INTEGER).setUnit("%");

  tempInside.exposeProperty("temperature").setName("Temperature Inside").setDatatype(Datatype::FLOAT).setUnit("°C");
  tempInside.exposeProperty("humidity").setName("Humidity Inside").setDatatype(Datatype::INTEGER).setUnit("%");

  if(Ethernet.begin(mac) == 0) {
    // Ethernet connection failed
    for(;;);
  }

  Device::setup(mac, Ethernet.localIP(), client);
}

uint32_t lastMillis = 0;

/* Dummy values */
double tempOut = 6.4;
double tempIn = 22.1;

int humidityOut = 30;
int humidityIn = 50;

void readTemperatures() {
  
  if(tempOut <= 10) {
    tempOut += 0.1;
  } else {
    tempOut = 6.4;
  }
  if(tempIn <= 25) {
    tempIn += 0.2;
  } else {
    tempIn = 22.1;
  }
  if(humidityOut <= 45) {
    humidityOut += 2;
  } else {
    humidityOut = 30;
  }
  if(humidityIn <= 60) {
    humidityIn += 1;
  } else {
    humidityIn = 50;
  }
}

void loop() {
  Device::loop();
  if(millis() - lastMillis >= 10000) { // Every 10 seconds
    readTemperatures();
    tempOutside.sendProperty("temperature", String(tempOut).c_str());
    tempOutside.sendProperty("humidity", String(humidityOut).c_str());
    tempInside.sendProperty("temperature", String(tempIn).c_str());
    tempInside.sendProperty("humidity", String(humidityIn).c_str());
    lastMillis = millis();
  }
}