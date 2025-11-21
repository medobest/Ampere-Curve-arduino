// ===================== Utility Functions =====================
void showbgd(int x, int y, const uint8_t *bmp, int w, int h, uint16_t color, uint16_t bg, uint8_t pad = 7) {
  int yy, xx;
  uint8_t first = 1, RVS = pad & 0x80;
  uint16_t pixel;
  pad &= 31;
  uint16_t wid = (w + pad) & ~pad;                //bits per row
  tft.setAddrWindow(x, y, x + w - 1, y + h - 1);  //
  for (yy = 0; yy < h; yy++) {
    uint32_t ofs = (RVS ? (h - yy - 1) : yy) * wid;  //bit offset
    const uint8_t *p = bmp + (ofs >> 3);             //flash address
    uint8_t mask = 0x80 >> (ofs & 7);                //bit mask
    uint8_t c = pgm_read_byte(p++);
    for (xx = 0; xx < w; xx++) {
      if (mask == 0) {
        c = pgm_read_byte(p++);
        mask = 0x80;
      }
      pixel = (c & mask) ? color : bg;
      tft.pushColors(&pixel, 1, first);
      first = 0;
      mask >>= 1;
    }
  }
  tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1);
}

void shbh() {
  // int x = 5, y, w = 128, h = 64;

  // const int SZ = w * h / 8;
  // uint8_t sram[SZ];
  //memcpy_P(sram, Serbious_Logo, SZ);

  tft.fillScreen(TFT_BLACK);
 analogWrite(BLPin, brightness/4);
  delay(200);
  // y = 0;
  tft.drawBitmap((480 - 283) / 2, 160 - (69 / 1.5), Serbious_Logo, 283, 69, TFT_WHITE);
  analogWrite(BLPin, brightness/3);
  delay(200);
    analogWrite(BLPin, brightness/2);
  delay(200);
   analogWrite(BLPin, brightness);

  delay(3000);
  tft.fillScreen(TFT_BLACK);
}
// Check if point is inside a rectangle
bool isInside(int x, int y, int rx, int ry, int rw, int rh) {
  return (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh);
}

// Read touchscreen coordinates
bool Touch_getXY() {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

// Draw dashed line
void drawDashedLine(int x0, int y0, int x1, int y1, uint16_t color, uint8_t dashLength, uint8_t gapLength) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;
  int count = 0;
  int pattern = dashLength + gapLength;
  while (true) {
    if ((count % pattern) < dashLength) {
      tft.drawPixel(x0, y0, color);
    }
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
    count++;
  }
}

// Show float with background
void showmsgXYflt(int x, int y, int sz, float msg, int base, int color, int colorb) {
  tft.setTextColor(color, colorb);
  tft.setTextSize(sz);
  tft.setCursor(x, y);
  tft.print(msg, base);
  tft.print(" ");
}

// Show string with background
void showmsgXYStr(int x, int y, int sz, const char *msg, int color, int colorb) {
  tft.setTextColor(color, colorb);
  tft.setTextSize(sz);
  tft.setCursor(x, y);
  tft.print(msg);
}

// Function to reset the Arduino
void software_reset() {
  asm volatile("jmp 0");
}


// ===================== UI Elements =====================

void DrewdasheBorder() {
  float tpp = 0.0;
  for (int x = 5; x >= 0; x--) {
    showmsgXYflt(0, 135 + (x * 33), 2, tpp, 1, TFT_WHITE, TFT_BLACK);
    tft.drawFastHLine(41, 143 + (x * 33), 10, TFT_WHITE);
    tft.drawFastHLine(46, 160 + (x * 33), 5, TFT_DARKGREY);
    tft.drawFastHLine(44, 152 + (x * 33), 7, TFT_DARKGREY);
    tft.drawFastHLine(44, 168 + (x * 33), 7, TFT_DARKGREY);

    // Use the global variable for the step value
    tpp += graphStepAmps;
  }
  tft.drawRect(51, 142, 462 - 49, 320 - 142, TFT_DARKGREY);
}

void updatestate(int stc) {
  tft.setTextSize(2);
  if (stc == 0) {
    tft.setCursor(220, 44);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print(F("Live"));
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  } else if (stc == 1) {
    tft.setCursor(220, 44);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(F("LOG "));
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  } else if (stc == 2) {
    tft.setCursor(220, 44);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(F("Live"));
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
}
void DeviceMainLine(bool flager) {
  tft.setFont(&FreeSans9pt7b);
  if (flager) {
    tft.fillRoundRect(1, 1, 478, 36, 18, CGREEN);
    tft.fillRoundRect(45, 4, 25, 28, 2, TFT_WHITE);
    showmsgXYStr(45, 29, 2, "S", CGREEN, TFT_WHITE);
    // showmsgXYStr(45, 29, 2, "erbious Ampere Curve", WHITE, CGreen);
    tft.setTextColor(TFT_WHITE, CGREEN);
    tft.println(F("erbious Ampere Curve"));
    tft.setTextSize(1);

    int csx = tft.getCursorX();
    int csy = tft.getCursorY();

    tft.setCursor(csx, csy - 15);
    tft.print(F("                 Current                                     Voltage"));
  }


  tft.fillRoundRect(15, 67, 205, 125 - 67, 10, TFT_LIGHTGREY);
  tft.fillRoundRect(15 + 205 + 35, 67, 205, 125 - 67, 10, TFT_LIGHTGREY);
  tft.setTextSize(2);

  tft.fillRoundRect(31, 73, 35, 47, 2, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, CGREEN);
  tft.setCursor(36, 73 + 34);
  tft.println("A");
  tft.fillRoundRect(16 + 15 + 205 + 35, 73, 35, 47, 2, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, CGREEN);
  tft.setCursor(16 + 15 + 205 + 35 + 5, 73 + 34);
  tft.println("V");
  tft.setFont();

  DrewdasheBorder();
}

void fillcur2() {
  for (int i = 0; i < 410; i++) {
    cur2[i] = 254;  //307-254  (307, 142)
  }
}

// This is the dialog box function
void showResetDialog() {
  // --- Define Dialog Geometry ---
  int boxX = 120, boxW = 240;
  int boxH = 160;
  int boxY = (320 - boxH) / 2;  // Center Y

  int btnW = 200, btnH = 45;
  int btn1X = boxX + (boxW - btnW) / 2;  // 140
  int btn2X = btn1X;

  int btn1Y = boxY + 25;
  int btn2Y = btn1Y + btnH + 20;

  // --- Draw Dialog Box ---
  tft.fillRoundRect(boxX, boxY, boxW, boxH, 10, TFT_DARKGREY);
  tft.drawRoundRect(boxX, boxY, boxW, boxH, 10, TFT_WHITE);

  // --- Draw Buttons ---
  // Button 1: RESET
  tft.fillRoundRect(btn1X, btn1Y, btnW, btnH, 8, TFT_GREEN);
  showmsgXYStr(btn1X + 70, btn1Y + 14, 2, "RESET", TFT_WHITE, TFT_GREEN);

  // Button 2: RESTART
  tft.fillRoundRect(btn2X, btn2Y, btnW, btnH, 8, TFT_RED);
  showmsgXYStr(btn2X + 58, btn2Y + 14, 2, "RESTART", TFT_WHITE, TFT_RED);

  // --- Wait for Touch (Blocking Loop) ---
  while (true) {
    if (Touch_getXY()) {

      // Check for RESET button press
      if (isInside(pixel_x, pixel_y, btn1X, btn1Y, btnW, btnH)) {
        break;  // Exit the while loop to reset
      }

      // Check for RESTART button press
      if (isInside(pixel_x, pixel_y, btn2X, btn2Y, btnW, btnH)) {
        software_reset();  // Restart the Arduino
      }
    }
    // delay(50); // Small delay to prevent CPU hog
  }

  // --- User pressed RESET ---

  // No partial cleanup needed here. The full screen redraw below will fix it.

  // Call the graph reset, which will clear the rest of the dialog
  performGraphReset();

  // Unpause the main loop
  isPaused = false;
}

// This function now performs a full UI redraw to ensure a clean screen
void performGraphReset() {
  // 1. Redraw the ENTIRE UI to clear any remaining dialog residue
  //tft.fillScreen(TFT_BLACK);
  tft.fillRect(1, 66, 480, 320 - 66, TFT_BLACK);
  DeviceMainLine(false);

  // 2. Reset the variables
  fillcur2();

  // Reset graph scale variables to 1.0A
  graphMaxAmps = 1.0;
  graphStepAmps = 0.2;

  // Restore graph state
  cc = CGREEN;
  cx = 52;
  curP = 0;
  att = true;
  updatestate(0);
}

// MODIFIED: button0 now pauses and calls the dialog
void button0() {
  isPaused = true;
  showResetDialog();
}

// Debug button
void button1() {
  if (att) {
    att = false;
    tft.fillRect(52, 142, 410, 165, TFT_BLACK);
    DrewdasheBorder();
    int rr = 0;
    for (int i = 0; i < 410; i++) {
      if (i == 0) { rr = cur2[i] + 53; }                       ////////////////////////+53
      tft.drawLine(52 + i, rr, 53 + i, cur2[i] + 53, CGREEN);  //////////////////////////+53
      rr = cur2[i] + 53;                                       ////////////////////////////+53
    }
    updatestate(1);
  } else {
    tft.fillRect(52, 142, 410, 165, TFT_BLACK);
    DrewdasheBorder();
    cc = CGREEN;
    cx = 52;
    att = true;
    updatestate(0);
  }
}