#include <UIPEthernet.h>
#include <Device.h>

uint8_t mac[6] = {0x74,0x69,0x69,0x2D,0x30,0x32};

EthernetClient client;

/* Nodes */
Node led13("led", "LED PC13", "led");

void ledOnHandler(const char *data) {

    bool on = strcmp(data, "true") == 0;

    if(on) {
        digitalWrite(PC13, LOW);
    } else {
        digitalWrite(PC13, HIGH);
    }
}

void setup() {
    delay(500);
    pinMode(PC13, OUTPUT);
    
    Device::setFirmware("LedOnOff", "1.0.0");
    Device::setName("Led");

    led13.exposeProperty("on").setName("On").setDatatype(Datatype::BOOLEAN).settableHandler(ledOnHandler);

    if(Ethernet.begin(mac) == 0) {
        // Ethernet connection failed
        for(;;);
    }

    Device::setup(mac, Ethernet.localIP(), client);

    // Send Property after setup
    led13.sendProperty("on", "true");
    digitalWrite(PC13, LOW);
}

void loop() {
    Device::loop();
}