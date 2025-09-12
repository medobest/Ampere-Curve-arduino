#include <Adafruit_GFX.h>     // Core graphics library
#include <MCUFRIEND_kbv.h>    // Hardware-specific library
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
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define CYAN    0x07FF
#define WHITE   0xFFFF
#define GREY    0x8410
#define CGREEN  ((100 & 0x1F) << 11) | ((153 & 0x3F) << 5) | (1 & 0x1F)

// ===================== Timing =====================
unsigned long previousMillis2 = 0;
const long interval2 = 10;
long previousMillis = 0;
const long interval = 1000;

// ===================== Sensor Pins =====================
const int Vpin = A6;
const int Ipin = A5;
ACS712XX ACS712(ACS712_05B, Ipin); //here change your sensor model( 5A , 20A , 30A)

// ===================== Variables =====================
int vx, ix;
int cx = 52;
uint16_t cc = CGREEN;

const float vfixer = 39.57528957528958; // Voltage calibration factor

bool EnGraph = true;
float t1 = 0.0;  // Voltage
float t2 = 0.0;  // Current

// Graph drawing bounds
int bt = 52;
int et = 462;

// ===================== Button Layout =====================
int buttons[2][4] = {
  {15, 67, 205, 58},             // Current button
  {255, 67, 205, 58}             // Voltage button
};



// ===================== Arduino Core =====================

void setup() {
  Serial.begin(115200);
  tft.begin(tft.readID());
  tft.setRotation(3);

  tft.fillScreen(BLACK);
  DeviceMainLine();

  // Initial readings
  t1 = analogRead(Vpin) / vfixer;
  t2 = ACS712.getDC();
  if (t2 <= 0.05) t2 = 0.0;

  showmsgXYflt(75, 80, 4, t2, 2, WHITE, GREY);
  showmsgXYflt(315, 80, 4, t1, 2, WHITE, GREY);
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
      showmsgXYflt(75, 80, 4, t2, 2, RED, GREY);
      showmsgXYflt(315, 80, 4, t1, 2, BLUE, GREY);
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

      tft.drawLine(cx, ix, cx + 1, mappedCurrent, cc);

      if (cx >= et) {
           switch (cc) {
          case CGREEN:
            cc = BLUE;
            break;
          case BLUE:
            cc = CGREEN;
            break;
        }
        cx = bt;
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
}
