#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_CABEZA 6
#define PIN_COLA 7

int R_color = 0;
int G_color = 0;
int B_color = 75;
int wait = 50;

BridgeServer server;

Adafruit_NeoPixel strip_CABEZA = Adafruit_NeoPixel(12, PIN_CABEZA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_COLA = Adafruit_NeoPixel(12, PIN_COLA, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  strip_CABEZA.begin();
  strip_CABEZA.show(); // Initialize all pixels to 'off'
  strip_CABEZA.setBrightness(60);

  strip_COLA.begin();
  strip_COLA.show(); // Initialize all pixels to 'off'
  strip_COLA.setBrightness(60);
  
}

void loop() {

  theaterChase(strip_CABEZA.Color(R_color, G_color, B_color), &wait);

}

void theaterChase(uint32_t c, int *wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    //En cada ciclo compruebo que funcione
    BridgeClient client = server.accept();
    if (client) {
      String command = client.readStringUntil('/');
      if (command == "R") {
        R_color = client.parseInt();
        Serial.println(R_color);
      }
      if (command == "G") {
        G_color = client.parseInt();
        Serial.println(G_color);
      }
      if (command == "B") {
        B_color = client.parseInt();
        Serial.println(B_color);
      }
      if (command == "wait") {
        *wait = client.parseInt();
        Serial.print("Nueva espera: ");
        Serial.println(*wait);
      }
      client.stop();
      break;
    }
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip_CABEZA.numPixels(); i = i + 3) {
        strip_CABEZA.setPixelColor(i + q, c);  //turn every third pixel on
        strip_COLA.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip_CABEZA.show();
      strip_COLA.show();
      Serial.println(*wait);
      delay(*wait);
      for (int i = 0; i < strip_CABEZA.numPixels(); i = i + 3) {
        strip_CABEZA.setPixelColor((i + q), 0);      //turn every third pixel off
        strip_COLA.setPixelColor((i + q), 0);
      }
    }
  }
}
