#ifdef ARDUINO_ESP32_GATEWAY_F
  #pragma message "Compling for Olimex ESP32 Gateway Rev F"
  #define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
  #define ETH_PHY_POWER 5
#elif ARDUINO_ESP32_POE_ISO
  #pragma message "Compling for Olimex ESP32 POE ISO"
  #define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
  #define ETH_PHY_POWER 12
#elif ARDUINO_WESP32
  #pragma message "Compling for Silicognition wESP32 POE"
#else
  #error "Board not supported."
#endif

#ifndef UNIVERSES
  #error "Please define UNIVERSES to the number of universes required for this board."
#endif

#if UNIVERSES >= 1
  #ifndef UPIN1
	#error "Please define UPIN1 to the GPIO pin # for the first output universe."
  #endif
#endif
#if UNIVERSES >= 2
  #ifndef UPIN2
	#error "Please define UPIN2 to the GPIO pin # for the second output universe."
  #endif
#endif
#if UNIVERSES >= 3
  #ifndef UPIN3
	#error "Please define UPIN3 to the GPIO pin # for the third output universe."
  #endif
#endif
#if UNIVERSES >= 4
  #ifndef UPIN4
	#error "Please define UPIN4 to the GPIO pin # for the fourth output universe."
  #endif
#endif

#include <ETH.h>
#include <ESPAsyncE131.h>
#include <FastLED.h>

static bool eth_connected = false;

ESPAsyncE131 e131(UNIVERSES);

// 300 channels per strip maximum, 1 strip per universe
#define NUM_LEDS_PER_STRIP   100
#define NUM_STRIPS           UNIVERSES
#define NUM_PIXELS           (NUM_LEDS_PER_STRIP * NUM_STRIPS)

CRGB leds[NUM_LEDS_PER_STRIP*NUM_STRIPS];

void WiFiEvent (WiFiEvent_t event);

void setup (void)
{
  // set up serial port
  Serial.begin (115200);
  Serial.printf ("Hello, world\n\r");

  // set up LEDs
#if UNIVERSES >= 1
  FastLED.addLeds<WS2812B,UPIN1,RGB>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
#endif
#if UNIVERSES >= 2
  FastLED.addLeds<WS2812B,UPIN2,RGB>(leds, 1*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
#endif
#if UNIVERSES >= 3
  FastLED.addLeds<WS2812B,UPIN3,RGB>(leds, 2*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
#endif
#if UNIVERSES >= 4
  FastLED.addLeds<WS2812B,UPIN4,RGB>(leds, 3*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
#endif

  // set for dim initial level on every third channel
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    leds[pixel] = CRGB (16, 0, 0);
  }
  FastLED.show();

  // set up ethernet
  WiFi.onEvent(WiFiEvent);
#ifdef ARDUINO_ESP32_GATEWAY_F
  ETH.begin();
#elif ARDUINO_ESP32_POE_ISO
  ETH.begin();
#elif ARDUINO_WESP32
  ETH.begin(0, -1, 16, 17, ETH_PHY_RTL8201);
#endif

  // set up E1.31
  if (e131.begin(E131_UNICAST)) {
    Serial.println(F("Listening for data..."));
  } else {
    Serial.println(F("*** e131.begin failed ***"));
  }
}


void loop (void)
{
  bool sendit = false;
  
  while (!e131.isEmpty()) {

	// pull packet from ring buffer
    e131_packet_t packet;
    e131.pull (&packet);     
    
    Serial.printf ("%u/%u/%u/%u\n\r",
      htons(packet.universe),                 // universe
      htons(packet.property_value_count) - 1, // number of channels
      e131.stats.num_packets,                 // packet counter
      e131.stats.packet_errors);              // packet error counter
    
	// get number of channels and check bounds
    int channels = htons (packet.property_value_count) - 1;
    if (channels > 300) {
      channels = 300;
    }
    
	// get universe and check bounds
    int universe = htons (packet.universe);
	if ((universe < 1) || (universe > UNIVERSES)) {
	  continue;
    }

    uint8_t *ledptr;
    
    switch (universe) {
      case 1:
        ledptr = (uint8_t *)&leds[0];
        for (int i = 0; i < channels; i++) {
          *ledptr++ = packet.property_values[1+i];
        }
        break;
      case 2:
        ledptr = (uint8_t *)&leds[100];
        for (int i = 0; i < channels; i++) {
          *ledptr++ = packet.property_values[1+i];
        }
        break;
      case 3:
        ledptr = (uint8_t *)&leds[200];
        for (int i = 0; i < channels; i++) {
          *ledptr++ = packet.property_values[1+i];
        }
        break;
      case 4:
        ledptr = (uint8_t *)&leds[300];
        for (int i = 0; i < channels; i++) {
          *ledptr++ = packet.property_values[1+i];
        }
        break;
    }

	// send after receiving last universe
    if (universe == UNIVERSES) {
      sendit = true;
    }
  }

  if (sendit) {
    FastLED.show();
  }
}


void WiFiEvent (WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    
    switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}
