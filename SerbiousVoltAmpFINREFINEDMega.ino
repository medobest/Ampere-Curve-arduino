#include <Adafruit_GFX.h>   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library
#include <ACS712XX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include "bitmap_mono.h"
//#include <"Assets.ino">
// ===================== TFT & Touch Setup =====================
MCUFRIEND_kbv tft;
// TouchScreen wiring
#define YP A2  // must be analog
#define XM A3  // must be analog
#define YM 8   // digital OK
#define XP 9   // digital OK

#define MINPRESSURE 200
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// Touch calibration
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;
int pixel_x, pixel_y;
// ===================== Colors =====================
#define CGREEN ((100 & 0x1F) << 11) | ((153 & 0x3F) << 5) | (1 & 0x1F)
/********************** Color Index *******************
#define TFT_BLACK       0x0000      
#define TFT_NAVY        0x000F      
#define TFT_DARKGREEN   0x03E0      
#define TFT_DARKCYAN    0x03EF      
#define TFT_MAROON      0x7800      
#define TFT_PURPLE      0x780F      
#define TFT_OLIVE       0x7BE0      
#define TFT_LIGHTGREY   0xC618 
    
#define TFT_DARKGREY    0x7BEF      
#define TFT_BLUE        0x001F     
#define TFT_GREEN       0x07E0   
#define TFT_CYAN        0x07FF    
#define TFT_RED         0xF800    
#define TFT_MAGENTA     0xF81F    
#define TFT_YELLOW      0xFFE0   
#define TFT_WHITE       0xFFFF    
#define 
TFT_ORANGE      0xFDA0    
#define TFT_GREENYELLOW 0xB7E0     
#define TFT_PINK        0xFC9F
********************************************************/

// ===================== Timing =====================
unsigned long previousMillis2 = 0;
const long interval2 = 10;
long previousMillis = 0;
const long interval = 1000;
// ===================== Sensor Pins =====================
const int Vpin = A6;
const int Ipin = A5;
ACS712XX ACS712(ACS712_05B, Ipin);
// ===================== Variables =====================
int vx, ix;
int cx = 52;
uint16_t cc = CGREEN;

const float vfixer = 39.57528957528958;
// Voltage calibration factor

bool EnGraph = true;
float t1 = 0.0;  // Voltage
float t2 = 0.0;
// Current

// Graph drawing bounds
int bt = 52;
int et = 462;
uint8_t cur2[410];
int curX = 0;
int curP = 0;
bool att = true;

// MODIFIED: Set initial graph max to 1.0A
float graphMaxAmps = 1.0;   // The current max Amps for the Y-axis
float graphStepAmps = 0.2;  // The value of each grid step (1.0A / 5 intervals)

// NEW: Add a pause flag for the dialog
bool isPaused = false;

// ===================== Button Layout =====================
int buttons[2][4] = {
  { 15, 67, 205, 58 },  // Current button
  { 255, 67, 205, 58 }  // Voltage button
};
// ===================== Arduino Core =====================
//String res;
void DeviceMainLine(bool flager = true);

int BLPin = 10;         // the PWM pin the LED is attached to
int brightness = 255;  // how bright the LED is



void setup() {
  pinMode(BLPin, OUTPUT);
  analogWrite(BLPin, 0);
  fillcur2();

  // Serial.begin(115200);
  tft.begin(tft.readID());
  Serial.println(tft.readID(), HEX);
  tft.setRotation(3);
  // Serial.print("shield model :");
  // Serial.println(tft.readID());
  shbh();
  //tft.fillScreen(TFT_BLACK);
  DeviceMainLine();
  updatestate(0);
  // Initial readings
  t1 = analogRead(Vpin) / vfixer;
  t2 = ACS712.getDC();
  if (t2 <= 0.05) t2 = 0.0;

  showmsgXYflt(75, 80, 4, t2, 2, TFT_WHITE, TFT_LIGHTGREY);
  showmsgXYflt(315, 80, 4, t1, 2, TFT_WHITE, TFT_LIGHTGREY);

  /*
  Serial.print("Lane:");
  Serial.println( ACS712.autoCalibrate());
  */
}
int poii = 201;

void loop() {
  unsigned long currentMillis2 = millis();

  // NEW: Only run the main logic if NOT paused
  if ((currentMillis2 - previousMillis2) >= interval2 && !isPaused) {
    previousMillis2 = currentMillis2;
    previousMillis += interval2;

    // Read sensors
    t1 = analogRead(Vpin) / vfixer;
    t2 = ACS712.getDC();
    if (t2 <= 0.009) t2 = 0.0;
    /*
    Serial.print(t1);
    Serial.print("        ");
    Serial.print(cc);
    Serial.print("        ");
    Serial.println(t2);
*/
    // MODIFIED: Check if graph scale needs to be updated (starts from 1.0A)
    float newMax = graphMaxAmps;
    if (t2 > 1.0 && graphMaxAmps == 1.0) {
      newMax = 2.0;  // Set next max to 2.0A
    } else if (t2 > 2.0 && graphMaxAmps == 2.0) {
      newMax = 3.0;  // Set next max to 3.0A
    } else if (t2 > 3.0 && graphMaxAmps == 3.0) {
      newMax = 4.0;  // Set next max to 4.0A
    } else if (t2 > 4.0 && graphMaxAmps == 4.0) {
      newMax = 5.0;  // Set next max to 5.0A (max for ACS712-05B)
    }

    // If the max has changed, redraw the graph
    if (newMax != graphMaxAmps) {
      graphMaxAmps = newMax;
      graphStepAmps = graphMaxAmps / 5.0;  // 5 intervals on the graph

      // Reset the graph
      fillcur2();
      tft.fillRect(52, 142, 410, 165, TFT_BLACK);
      DrewdasheBorder();
      cx = bt;
      curP = 0;
    }


    // Update display every 1s
    if (previousMillis >= interval) {
      showmsgXYflt(75, 80, 4, t2, 2, TFT_RED, TFT_LIGHTGREY);
      showmsgXYflt(315, 80, 4, t1, 2, TFT_BLUE, TFT_LIGHTGREY);
      previousMillis = 0;
    }

    // Graph control
    if (t2 > 0.06) {
      EnGraph = true;
    } else if (t2 < 0.04) {
      EnGraph = false;
    }

    if (EnGraph) {
      // MODIFIED: Use graphMaxAmps in map()
      int mappedCurrent = map(t2 * 100, 0, (long)(graphMaxAmps * 100), 307, 142);

      //  mappedCurrent = map(random(poii-5, poii), 0, 200, 307, 142);
      if (cx == bt) {
        vx = t1;
        ix = mappedCurrent;
      }

      if (att) {
        //tft.drawFastVLine(cx, 143, 320-144, TFT_WHITE);
        tft.drawFastVLine(cx, 143, 320 - 144, TFT_BLACK);

        tft.drawLine(cx, ix, cx + 1, mappedCurrent, cc);

        //tft.drawFastVLine(cx, mappedCurrent, 2, cc);
        //tft.drawPixel(cx,ix, CGREEN);
      }

      if (cx >= et) {
        switch (cc) {
          case CGREEN:
            cc = TFT_ORANGE;
            break;
          case TFT_ORANGE:
            cc = CGREEN;
            break;
        }
        cx = bt;

        curX++;
        if (curX == 5 && curP < 410) {
          cur2[curP] = ix-53;////////////////////////////////////////-53 added
          curX = 0;
          curP++;
        }

        poii -= 10;

      } else {
        cx++;
        vx = t1;
        ix = mappedCurrent;
      }

      //updatestate(0);
      tft.fillCircle(237, 95, 10, TFT_RED);
    } else {
      // enabling next line will make it continue from begin(with overdrew) ,, cleaning last path needed.
      //cx = bt;
      //updatestate(2);
      tft.fillCircle(237, 95, 10, TFT_WHITE);
    }
  }

  // Handle touch buttons
  // NEW: Only check for main buttons if NOT paused
  if (Touch_getXY() && !isPaused) {
    for (int i = 0; i < 2; i++) {
      if (isInside(pixel_x, pixel_y, buttons[i][0], buttons[i][1], buttons[i][2], buttons[i][3])) {
        if (i == 0) button0();
        if (i == 1) button1();
      }
    }
  }
}