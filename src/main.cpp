#include <UIPEthernet.h>
#include <device.h>

uint8_t mac[6] = {0x74,0x69,0x69,0x2D,0x30,0x32};

EthernetClient client;

/* Nodes */
Node led13("led", "LED PC13", "led");

bool ledOn = true;

void ledOnHandler(const char *on) {
    if(ledOn) {
        digitalWrite(PC13, LOW);
    } else {
        digitalWrite(PC13, HIGH);
    }
    ledOn = !ledOn;
}

void setup() {
    delay(500);
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);
    Device::setFirmware("LedOnOff", "1.0.0");
    Device::setName("Led");

    led13.exposeProperty("on").setName("On").setDatatype(Datatype::BOOLEAN).settableHandler(ledOnHandler);

    if(Ethernet.begin(mac) == 0) {
        // Ethernet connection failed
        for(;;);
    }

    Device::setup(mac, Ethernet.localIP(), client);

    
}

void loop() {
    Device::loop();
}