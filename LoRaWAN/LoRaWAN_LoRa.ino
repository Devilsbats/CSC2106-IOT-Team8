/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.s
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!

 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in
 * arduino-lmic/project_config/lmic_project_config.h or from your BOARDS.txt.
 *
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>


//
// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values assigned by the TTN console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.
//
#ifdef COMPILE_REGRESSION_TEST
# define FILLMEIN 0
#else
# warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
# define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]={ 0xFD, 0x5B, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
static const u1_t PROGMEM APPKEY[16]={ 0x46, 0x65, 0x12, 0xAD, 0x0A, 0x71, 0x39, 0xFB, 0xD4, 0x15, 0x38, 0xC3, 0x96, 0xF1, 0x40, 0x48 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

static uint8_t btn_activated[1] = { 0x01};
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 5;

int M5ID;
int distance; 
float temperature;
int truncatedtemperature;
bool togglenontemp;
bool commastate;
char str_temp[6];

String incomingData;

// Define the SoftwareSerial object with the RX and TX pins
SoftwareSerial mySerial(0, 2); // RX, TX

// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 7,
  .dio = {2, 5, 6},
};

void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}


#define DHT_PIN 8

// DHT11 or DHT22
#define DHTTYPE DHT22

// Initialize dht
DHT dht(DHT_PIN, DHTTYPE);

int fPort = 1;               // fPort usage: 1=dht11, 2=button, 3=led
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
//-----------------------------

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(artKey[i]);
              }
              Serial.println("");
              Serial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(nwkKey[i]);
              }
              Serial.println();
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
        // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));

            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));

              //------ Added ----------------
              fPort = LMIC.frame[LMIC.dataBeg - 1];
              Serial.print(F("fPort "));
              Serial.println(fPort);
            
             Serial.println();
             //-----------------------------
            }                    
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            break;
        
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

void do_send(osjob_t* j){
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        byte* payload = NULL;
        int payloadSize = 0;

        if(commastate){
            payloadSize = 6;
        } else {
            payloadSize = 6;
        }

        payload = (byte*) malloc(payloadSize * sizeof(byte));
        if (payload == NULL) {
            Serial.println(F("Memory allocation failed"));
            return;
        }

        if(commastate){

            payload[0] = highByte(distance);
            payload[1] = lowByte(distance);
            payload[2] = highByte(M5ID);
            payload[3] = lowByte(M5ID);
            payload[4] = highByte(truncatedtemperature);
            payload[5] = lowByte(truncatedtemperature);

            fPort = 1;
            LMIC_setTxData2(fPort, payload, payloadSize, 0);
            Serial.println(F("Packet queued"));

            // Free the allocated memory
            free(payload);
        } else {

            payload[0] = highByte(distance);
            payload[1] = lowByte(distance);
            payload[2] = highByte(M5ID);
            payload[3] = lowByte(M5ID);
            payload[4] = highByte(truncatedtemperature);
            payload[5] = lowByte(truncatedtemperature);

            fPort = 1;
            LMIC_setTxData2(fPort, payload, payloadSize, 0);
            Serial.println(F("Packet queued"));

            // Free the allocated memory
            free(payload);
        }
    }
}


void setup() {
    Serial.begin(9600);
    mySerial.begin(115200);
    Serial.println(F("Starting"));

    dht.begin();
    
    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Use with Arduino Pro Mini ATmega328P 3.3V 8 MHz
    // Let LMIC compensate for +/- 1% clock error
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100); 

    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}

void loop() {
  if (mySerial.available() > 0) {
      incomingData = mySerial.readStringUntil('\n'); // Read until newline character
      Serial.println(incomingData);
      int separatorIndex1 = incomingData.indexOf(','); // Find the first separator
      int separatorIndex2 = incomingData.indexOf(',', separatorIndex1 + 1); // Find the second separator
      
      // Check if the string contains only one comma
      bool oneComma = separatorIndex1 != -1 && separatorIndex2 == -1;
      
      if (separatorIndex1 != -1 && (separatorIndex2 != -1 || oneComma)) {
          String distanceStr;
          String M5IDStr;
          String temperatureStr;

          if (oneComma) {
              // If there's only one comma, parse accordingly
              distanceStr = incomingData.substring(0, separatorIndex1);
              M5IDStr = incomingData.substring(separatorIndex1 + 1);

              // Convert strings to appropriate data types
              distance = distanceStr.toInt();
              M5ID =  M5IDStr.toInt(); // Assuming the third value is an integer

              commastate = true;
              togglenontemp = true;

          } else {
              // If there are two commas, parse accordingly
              distanceStr = incomingData.substring(0, separatorIndex1);
              M5IDStr = incomingData.substring(separatorIndex1 + 1, separatorIndex2);
              temperatureStr = incomingData.substring(separatorIndex2 + 1);

              // Convert strings to appropriate data types
              distance = distanceStr.toInt();
              temperature = temperatureStr.toFloat();
              truncatedtemperature = (int)(temperature * 100);
              Serial.println(truncatedtemperature);
              M5ID =  M5IDStr.toInt(); // Assuming the third value is an integer

              commastate = false;
              togglenontemp = false;

          }
      }
  }
 os_runloop_once();
}