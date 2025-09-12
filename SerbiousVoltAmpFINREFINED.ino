#include <Adafruit_GFX.h>   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library
#include <ACS712XX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>

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
#define TFT_ORANGE      0xFDA0    
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

const float vfixer = 39.57528957528958;  // Voltage calibration factor

bool EnGraph = true;
float t1 = 0.0;  // Voltage
float t2 = 0.0;  // Current

// Graph drawing bounds
int bt = 52;
int et = 462;
int cur2[410];
int curX = 0;
int curP = 0;
bool att = true;
// ===================== Button Layout =====================
int buttons[2][4] = {
  { 15, 67, 205, 58 },  // Current button
  { 255, 67, 205, 58 }  // Voltage button
};



// ===================== Arduino Core =====================
String res;

void setup() {
  fillcur2();

  Serial.begin(115200);
  tft.begin(tft.readID());
  tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);
  DeviceMainLine();

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

void loop() {
  unsigned long currentMillis2 = millis();
  if ((currentMillis2 - previousMillis2) >= interval2) {
    previousMillis2 = currentMillis2;
    previousMillis += interval2;

    // Read sensors
    t1 = analogRead(Vpin) / vfixer;
    t2 = ACS712.getDC();
    if (t2 <= 0.05) t2 = 0.0;

    Serial.print(t1);
    Serial.print("        ");
    Serial.print(cc);
    Serial.print("        ");
    Serial.println(t2);

    // Update display every 1s
    if (previousMillis >= interval) {
      showmsgXYflt(75, 80, 4, t2, 2, TFT_RED, TFT_LIGHTGREY);
      showmsgXYflt(315, 80, 4, t1, 2, TFT_BLUE, TFT_LIGHTGREY);
      previousMillis = 0;
    }

    // Graph control
    EnGraph = (t2 > 0.05);

    if (EnGraph) {
      int mappedCurrent = map(t2 * 100, 0, 200, 307, 142);

      if (cx == bt) {
        vx = t1;
        ix = mappedCurrent;
      }

     if (att){
      tft.drawLine(cx, ix, cx + 1, mappedCurrent, cc);
      //tft.drawPixel(cx,ix, CGREEN);
      } 

      if (cx >= et) {
        switch (cc) {
          case CGREEN:
            cc = TFT_BLUE;
            break;
          case TFT_BLUE:
            cc = CGREEN;
            break;
        }
        cx = bt;

        curX++;
        if (curX == 5 && curP < 410) {
          cur2[curP] = ix;
          curX = 0;
          curP++;
        
        }




      } else {
        cx++;
        vx = t1;
        ix = mappedCurrent;
      }
    } else {
      cx = bt;
    }
  }

  // Handle touch buttons
  if (Touch_getXY()) {
    for (int i = 0; i < 2; i++) {
      if (isInside(pixel_x, pixel_y, buttons[i][0], buttons[i][1], buttons[i][2], buttons[i][3])) {
        if (i == 0) button0();
        if (i == 1) button1();
      }
    }
  }

  /*
    if (Serial.available()) {
      res = Serial.readStringUntil("|");
      res.remove(res.length() - 3, 5);
      int f = res.indexOf(":");
      if (f == -1) {
        if (res == "UPD") {
         PBImage();
        }
      } else {

      }
    }

*/
}
