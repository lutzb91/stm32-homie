#include <Arduino.h>
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>
#include "device.h"
#include "node.h"

uint8_t mac[6] = {0x74,0x69,0x69,0x2D,0x30,0x31};

Adafruit_SSD1306 display;

Node n1("trafo", "Trafo", "Electric");
Node n2("led", "LED Streifen", "Light");//, true, 10);
Node n3("mode", "Modus", "control");

void setup()   {
  delay(500);

  Device::setFirmware("TestFirmwareSTM32", "1.0.0");
  Device::setName("Kellerlicht");

  n1.exposeProperty("status", "Status", true, false, "", Node::Datatype::BOOLEAN);
  n1.exposeProperty("voltage", "Voltage", false, true, "V", Node::Datatype::FLOAT);
  n3.exposeProperty("mode", "Modus");

  Device::setup(mac);

  pinMode(PC13, OUTPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Clear the buffer.
  display.clearDisplay();

  

  // oberer Teil 16px
/*  display.clearDisplay();
  display.print("IP: ");
  ip.printTo(display);
  display.println();*/

  

  delay(1000);

  boolean on = true;

  for(int y = 0; y<6; y++) {
    for(int x = 0; x < 15; x++) {
      if(x == 0 || x == 14 || y == 0 || y == 5) {
        if(on) {
          display.fillCircle(x*6+3+x, 19+y*6+y, 3, WHITE);
        } else {
          display.drawCircle(x*6+3+x, 19+y*6+y, 3, WHITE);
        }
        on = !on;
      }
    }
  }
  display.display();

}


void loop() {
  Device::loop();
}