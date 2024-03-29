#include <Encoder.h>
#include "Keyboard.h"
#include <U8g2lib.h>
#include <EEPROM.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define ARM_PIN 5
#define ARM_LED_PIN 6
#define BTN_1_PIN 9
#define BTN_2_PIN 10
#define BTN_3_PIN 16
#define BTN_4_PIN 14
#define BTN_5_PIN 15
#define LED_1_PIN 4
#define LED_2_PIN 18
#define LED_3_PIN 19
#define LED_4_PIN 20
#define LED_5_PIN 21
#define BTN_COUNT 5

#define FV(x) (__FlashStringHelper*)(x)

Encoder enc(7, 8);
uint8_t btnPins[] = { BTN_1_PIN, BTN_2_PIN, BTN_3_PIN, BTN_4_PIN, BTN_5_PIN };
uint8_t ledPins[] = { LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN, LED_5_PIN };
uint8_t selectedStratagems[] = { 0, 1, 2, 3, 4 };

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define helldivers_width 128
#define helldivers_height 64
static const unsigned char helldivers_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff,
   0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0xc0, 0xff, 0xff, 0x81, 0x7f, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x15, 0x7f, 0x80, 0xff,
   0xff, 0x00, 0x7f, 0x54, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0x95, 0x3f, 0x00, 0xff, 0x7f, 0x00, 0xfe, 0x54, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0x95, 0x3f, 0x00, 0xff, 0x7f, 0x00, 0xfe, 0x54,
   0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xc5, 0x3f, 0x00, 0xff,
   0x7f, 0x00, 0xfe, 0x51, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
   0xe5, 0x3f, 0x00, 0xff, 0x7f, 0x00, 0xfe, 0x53, 0x55, 0x55, 0x55, 0x55,
   0x55, 0x55, 0x55, 0x55, 0xf1, 0x3f, 0x00, 0xff, 0x7f, 0x00, 0xfe, 0x47,
   0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0xff,
   0x7f, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0x7f, 0x80, 0xff, 0xff, 0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xc0, 0xff, 0xff, 0x81, 0xff, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x3f,
   0xfe, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xfc, 0xff, 0x1f, 0xfc, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x1f, 0xfc, 0xff, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xcf,
   0xf9, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xfb, 0xff, 0xff, 0xef, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff,
   0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00 };

struct StratagemData {
  char category[40];
  char name[40];
  char input[10];
  char keys[10];
};

const StratagemData stratagems[] PROGMEM = {
  { "SUPPLY: BACKPACKS", "LIFT-850 Jump Pack", "\x48\x4B\x4B\x48\x4B", "swwsw" },
  { "SUPPLY: BACKPACKS", "B-1 Supply Pack", "\x48\x49\x48\x4B\x4B\x48", "saswws" },
  { "SUPPLY: BACKPACKS", "AX/LAS-5 \"Guard Dog\" Rover", "\x48\x4B\x49\x4B\x4A\x4A", "swawdd" },
  { "SUPPLY: BACKPACKS", "SH-20 Ballistic Shield Backpack", "\x48\x49\x48\x48\x4B\x49", "sasswa" },
  { "SUPPLY: BACKPACKS", "SH-32 Shield Generator Pack", "\x48\x4B\x49\x4A\x49\x4A", "swadad" },
  { "SUPPLY: BACKPACKS", "AX/AR-23 \"Guard Dog\"", "\x48\x4B\x49\x4B\x4A\x48", "swawds" },
  { "SUPPLY: WEAPONS", "MG-43 Machine Gun", "\x48\x49\x48\x4B\x4A", "saswd" },
  { "SUPPLY: WEAPONS", "APW-1 Anti-Materiel Rifle", "\x48\x49\x4A\x4B\x48", "sadws" },
  { "SUPPLY: WEAPONS", "M-105 Stalwart", "\x48\x49\x48\x4B\x4B\x49", "saswwa" },
  { "SUPPLY: WEAPONS", "EAT-17 Expendable Anti-tank", "\x48\x48\x49\x4B\x4A", "ssawd" },
  { "SUPPLY: WEAPONS", "GR-8 Recoilless Rifle", "\x48\x49\x4A\x4A\x49", "sadda" },
  { "SUPPLY: WEAPONS", "FLAM-40 Flamethrower", "\x48\x49\x4B\x48\x4B", "sawsw" },
  { "SUPPLY: WEAPONS", "AC-8 Autocannon", "\x48\x49\x48\x4B\x4B\x4A", "saswwd" },
  { "SUPPLY: WEAPONS", "RS-422 Railgun", "\x48\x4A\x48\x4B\x49\x4A", "sdswad" },
  { "SUPPLY: WEAPONS", "FAF-14 SPEAR Launcher", "\x48\x48\x4B\x48\x48", "sswss" },
  { "SUPPLY: WEAPONS", "GL-21 Grenade Launcher", "\x48\x49\x4B\x49\x48", "sawas" },
  { "SUPPLY: WEAPONS", "LAS-98 Laser Cannon", "\x48\x49\x48\x4B\x49", "saswa" },
  { "SUPPLY: WEAPONS", "ARC-3 Arc Thrower", "\x48\x4A\x48\x4B\x49\x49", "sdswaa" },
  { "MISSION", "Reinforce", "\x4B\x48\x4A\x49\x4B", "wsdaw" },
  { "MISSION", "SOS Beacon", "\x4B\x48\x4A\x4B", "wsdw" },
  { "MISSION", "Resupply", "\x48\x48\x4B\x4A", "sswd" },
  { "MISSION", "NUX-223 Hellbomb", "\x48\x4B\x49\x48\x4B\x4A\x48\x4B", "swaswdsw" },
  { "MISSION", "SSSD Delivery", "\x48\x48\x48\x4B\x4B", "sssww" },
  { "MISSION", "Seismic Probe", "\x4B\x4B\x49\x4A\x48\x48", "wwadss" },
  { "MISSION", "Upload Data", "\x48\x48\x4B\x4B\x4B", "sswww" },
  { "MISSION", "Eagle Rearm", "\x4B\x4B\x49\x4B\x4A", "wwawd" },
  { "MISSION", "Illumination Flare", "\x4A\x4A\x49\x49", "ddaa" },
  { "DEFENSIVE", "E/MG-101 HMG Emplacement", "\x48\x4B\x49\x4A\x4A\x49", "swadda" },
  { "DEFENSIVE", "FX-12 Shield Generator Relay", "\x48\x48\x49\x4A\x49\x4A", "ssadad" },
  { "DEFENSIVE", "A/ARC-3 Tesla Tower", "\x48\x4B\x4A\x4B\x49\x4A", "swdwad" },
  { "DEFENSIVE", "MD-6 Anti-Personnel Minefield", "\x48\x49\x4B\x4A", "sawd" },
  { "DEFENSIVE", "MD-I4 Incendiary Mines", "\x48\x49\x49\x48", "saas" },
  { "DEFENSIVE", "A/MG-43 Machine Gun Sentry", "\x48\x4B\x4A\x4A\x4B", "swddw" },
  { "DEFENSIVE", "A/G-16 Gatling Sentry", "\x48\x4B\x4A\x49", "swda" },
  { "DEFENSIVE", "A/M-12 Mortar Sentry", "\x48\x4B\x4A\x4A\x48", "swdds" },
  { "DEFENSIVE", "A/AC-8 Autocannon Sentry", "\x48\x4B\x4A\x4B\x49\x4B", "swdwaw" },
  { "DEFENSIVE", "A/MLS-4X Rocket Sentry", "\x48\x4B\x4A\x4A\x49", "swdda" },
  { "DEFENSIVE", "A/M-23 EMS Mortar Sentry", "\x48\x4B\x4A\x48\x4A", "swdsd" },
  { "OFFENSIVE: ORBITAL", "Orbital Gatling Barrage", "\x4A\x48\x49\x4B\x4B", "dsaww" },
  { "OFFENSIVE: ORBITAL", "Orbital Airburst Strike", "\x4A\x4A\x4A", "ddd" },
  { "OFFENSIVE: ORBITAL", "Orbital 120MM HE Barrage", "\x4A\x4A\x48\x49\x4A\x48", "ddsads" },
  { "OFFENSIVE: ORBITAL", "Orbital 380MM HE Barrage", "\x4A\x48\x4B\x4B\x49\x48\x48", "dswwass" },
  { "OFFENSIVE: ORBITAL", "Orbital Walking Barrage", "\x4A\x48\x4A\x48\x4A\x48", "dsdsds" },
  { "OFFENSIVE: ORBITAL", "Orbital Laser", "\x4A\x48\x4B\x4A\x48", "dswds" },
  { "OFFENSIVE: ORBITAL", "Orbital Railcannon Strike", "\x4A\x4B\x48\x48\x4A", "dwssd" },
  { "OFFENSIVE: ORBITAL", "Orbital Precision Strike", "\x4A\x4A\x4B", "ddw" },
  { "OFFENSIVE: ORBITAL", "Orbital Gas Strike", "\x4A\x4A\x48\x4A", "ddsd" },
  { "OFFENSIVE: ORBITAL", "Orbital EMS Strike", "\x4A\x4A\x49\x48", "ddas" },
  { "OFFENSIVE: ORBITAL", "Orbital Smoke Strike", "\x4A\x4A\x48\x4B", "ddsw" },
  { "OFFENSIVE: EAGLE", "Eagle Strafing Run", "\x4B\x4A\x4A", "wdd" },
  { "OFFENSIVE: EAGLE", "Eagle Airstrike", "\x4B\x4A\x48\x4A", "wdsd" },
  { "OFFENSIVE: EAGLE", "Eagle Cluster Bomb", "\x4B\x4A\x48\x48\x4A", "wdssd" },
  { "OFFENSIVE: EAGLE", "Eagle Napalm Airstrike", "\x4B\x4A\x48\x4B", "wdsw" },
  { "OFFENSIVE: EAGLE", "Eagle Smoke Strike", "\x4B\x4A\x4B\x48", "wdws" },
  { "OFFENSIVE: EAGLE", "Eagle 110MM Rocket Pods", "\x4B\x4A\x4B\x49", "wdwa" },
  { "OFFENSIVE: EAGLE", "Eagle 500kg Bomb", "\x4B\x4A\x48\x48\x48", "wdsss" },
};

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < BTN_COUNT; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  Keyboard.begin();

  u8g2.begin();
  drawLogo();

  pinMode(ARM_PIN, INPUT_PULLUP);
  // pinMode(ARM_LED_PIN, OUTPUT);
  digitalWrite(ARM_LED_PIN, LOW);

  EEPROM.get(0, selectedStratagems);
}

char stratagem[] = {'w', 's', 'd', 'a', 'w'};
long stratagemIndex  = 0;
int selectedButton = -1;
bool lastWasArmed = false;
int lastStratagem = 0;

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(ARM_PIN) == LOW) {
    armedLoop(!lastWasArmed);
    lastWasArmed = true;
  } else {
    unarmedLoop(lastWasArmed);
    lastWasArmed = false;
  }
}

void armedLoop(bool justSwitched) {
  // digitalWrite(ARM_LED_PIN, HIGH);
  if (justSwitched) {
    Serial.println("Armed");
    drawLogo();
    EEPROM.put(0, selectedStratagems);
    delay(50);
  }

  for (int i = 0; i < BTN_COUNT; i++) {
    digitalWrite(ledPins[i], LOW);

    if (digitalRead(btnPins[i]) == LOW) {
      digitalWrite(ledPins[i], HIGH);
      drawStratagem(selectedStratagems[i]);
      inputStratagem(selectedStratagems[i]);
      delay(500);

      drawLogo();
    }
  }
}

void unarmedLoop(bool justSwitched) {
  // digitalWrite(ARM_LED_PIN, LOW);

  for (int i = 0; i < BTN_COUNT; i++) {
    digitalWrite(ledPins[i], i == selectedButton ? HIGH : LOW);
    if (digitalRead(btnPins[i]) == LOW) {
      selectedButton = i;
      enc.write(4 * selectedStratagems[selectedButton]);
    }
  }

  if (selectedButton < 0) return;

  selectedStratagems[selectedButton] = stratagemId();
  if (selectedStratagems[selectedButton] != lastStratagem || justSwitched) {
    lastStratagem = selectedStratagems[selectedButton];
    drawStratagem(selectedStratagems[selectedButton]);
  }
}

int stratagemCount() {
  return sizeof(stratagems) / sizeof(StratagemData);
}

int stratagemId() {
  int count = stratagemCount();
  long pos = enc.read() / 4;
  if (pos >= 0) {
    return (int) (pos % count);
  }

  return (int) ((pos + 1) % count) + count - 1;
}

void drawStratagem(int i) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenthinnerguys_tf);
  u8g2.drawBox(0, 0, 128, 19);
  u8g2.setDrawColor(0);
  u8g2.setCursor(4, 14);
  u8g2.print(FV(stratagems[i].category));
  u8g2.setDrawColor(1);
  u8g2.setCursor(0, 35);
  u8g2.print(FV(stratagems[i].name));
  u8g2.setFont(u8g2_font_open_iconic_arrow_2x_t);
  u8g2.setCursor(0, 57);
  u8g2.print(FV(stratagems[i].input));
  u8g2.drawBox(2 + (128 - 16 - 2 - 2) * i / (stratagemCount() - 1), 62, 16, 2);
  u8g2.sendBuffer();
}

void drawLogo() {
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, helldivers_width, helldivers_height, helldivers_bits);
  u8g2.sendBuffer();
}

void inputStratagem(int i) {
  Serial.println("Sending stratagem");

  Keyboard.press(KEY_LEFT_CTRL);
  delay(100);
  int keysLength = strlen_P(stratagems[i].keys);
  for (byte k = 0; k < keysLength; k++) {
    char myChar = pgm_read_byte_near(stratagems[i].keys + k);
    Keyboard.press(myChar);
    delay(50);
    Keyboard.release(myChar);
    delay(50);
  }
  Keyboard.release(KEY_LEFT_CTRL);
}
