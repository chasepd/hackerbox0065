#include <MatrixHardware_ESP32_V0.h>    
#include <SmartMatrix.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define COLOR_DEPTH 24                  // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)

const char* ssid     = "SSID GOES HERE";
const char* password = "WIFI PASSWORD GOES HERE";

String bitcoin_url = "http://[ADDRESS OF PYTHON SERVER]/bitcoin";
String ethereum_url = "http://[ADDRESS OF PYTHON SERVER]/ethereum";
String litecoin_url = "http://[ADDRESS OF PYTHON SERVER]/litecoin";
String monero_url = "http://[ADDRESS OF PYTHON SERVER]/monero";

HTTPClient http;

const uint16_t kMatrixWidth = 64;       // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 32;      // Set to the height of your display
const uint8_t kRefreshDepth = 36;       // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;   // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);        // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);

volatile char bitcoin[20];
volatile char ethereum[20];
volatile char litecoin[20];
volatile char monero[20];

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer1, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer2, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer3, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer4, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

const int defaultBrightness = (35*255)/100;     // dim: 35% brightness

void setup() {
  Serial.begin(57600);
  delay(1000);
  randomSeed(analogRead(0));
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // setup matrix
  matrix.addLayer(&indexedLayer1); 
  matrix.addLayer(&indexedLayer2);
  matrix.addLayer(&indexedLayer3);
  matrix.addLayer(&indexedLayer4);
  matrix.begin();

  matrix.setBrightness(defaultBrightness);  
}

void loop() {
  // clear screen before writing new text
  indexedLayer1.fillScreen(0);
  indexedLayer2.fillScreen(0);
  indexedLayer3.fillScreen(0);
  indexedLayer4.fillScreen(0);

  http.begin(bitcoin_url.c_str());
  int httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    String response = http.getString();
    for (int iterate = 0; iterate < (sizeof(response.c_str())/sizeof(response.c_str()[0])) + 1; iterate++){
      bitcoin[iterate] = response.c_str()[iterate];
    }
    Serial.print("Bitcoin price acquired:");
    Serial.println((char *)bitcoin);
  }
  else {
    Serial.print("Error code while getting bitcoin price: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  http.begin(ethereum_url.c_str());
  httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    String response = http.getString();
    for (int iterate = 0; iterate < (sizeof(response.c_str())/sizeof(response.c_str()[0])) + 3; iterate++){
      ethereum[iterate] = response.c_str()[iterate];
    }
    Serial.print("Ethereum price acquired:");
    Serial.println((char *)ethereum);
  }
  else {
    Serial.print("Error code while getting eth price: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  http.begin(litecoin_url.c_str());
  httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    String response = http.getString();
    for (int iterate = 0; iterate < (sizeof(response.c_str())/sizeof(response.c_str()[0])) + 3; iterate++){
      litecoin[iterate] = response.c_str()[iterate];
    }
    Serial.print("Litecoin price acquired:");
    Serial.println((char *)litecoin);
  }
  else {
    Serial.print("Error code while getting ltc price: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  http.begin(monero_url.c_str());
  httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    String response = http.getString();
    for (int iterate = 0; iterate < (sizeof(response.c_str())/sizeof(response.c_str()[0])) + 3; iterate++){
      monero[iterate] = response.c_str()[iterate];
    }
    Serial.print("Monero price acquired:");
    Serial.println((char *)monero);
  }
  else {
    Serial.print("Error code while getting ltc price: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  
  char  * txtBuffer = new char[20];
  sprintf(txtBuffer, "BTC: %s", bitcoin);
  indexedLayer1.setFont(font5x7);
  indexedLayer1.setIndexedColor(1,{0xfc, 0x05, 0xbe});
  indexedLayer1.drawString(0, 1, 1, txtBuffer);
  indexedLayer1.swapBuffers();
  
  txtBuffer = new char[20];
  sprintf(txtBuffer, "ETH: %s", ethereum);
  indexedLayer2.setFont(font5x7);
  indexedLayer2.setIndexedColor(1,{0x00, 0xf7, 0xca});
  indexedLayer2.drawString(0, 9, 1, txtBuffer);
  indexedLayer2.swapBuffers();
  
  txtBuffer = new char[20];
  sprintf(txtBuffer, "LTC: %s", litecoin);
  indexedLayer3.setFont(font5x7);
  indexedLayer3.setIndexedColor(1,{0xe8, 0xe8, 0xe8});
  indexedLayer3.drawString(0, 17, 1, txtBuffer); 
  indexedLayer3.swapBuffers();

  txtBuffer = new char[20];
  sprintf(txtBuffer, "XMR: %s", monero);
  indexedLayer4.setFont(font5x7);
  indexedLayer4.setIndexedColor(1,{0xfc, 0x05, 0x91});
  indexedLayer4.drawString(0, 25, 1, txtBuffer); 
  indexedLayer4.swapBuffers();

  delay(10000);
}
