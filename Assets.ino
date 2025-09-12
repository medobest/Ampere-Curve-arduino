// ===================== Utility Functions =====================

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

// ===================== UI Elements =====================

void DrewdasheBorder() {
  float tpp = 0.0;
  for (int x = 5; x >= 0; x--) {
    drawDashedLine(52, 142 + (x * 33), 462, 142 + (x * 33), TFT_LIGHTGREY, 4, 4);
    drawDashedLine(52 + (x * 82), 142, 52 + (x * 82), 307, TFT_LIGHTGREY, 4, 4);
    showmsgXYflt(0, 135 + (x * 33), 2, tpp, 2, TFT_WHITE, TFT_BLACK);
    tpp += 0.40;
  }
}

void DeviceMainLine() {
  tft.setFont(&FreeSans9pt7b);
  tft.fillRoundRect(1, 1, 478, 36, 18, CGREEN);
  tft.fillRoundRect(45, 4, 25, 28, 2, TFT_WHITE);
  showmsgXYStr(45, 29, 2, "S", CGREEN, TFT_WHITE);
  // showmsgXYStr(45, 29, 2, "erbious Ampere Curve", WHITE, CGreen);
  tft.setTextColor(TFT_WHITE, CGREEN);
  tft.println("erbious Ampere Curve");
  tft.setTextSize(1);
  int cx = tft.getCursorX();
  int cy = tft.getCursorY();
  tft.setCursor(cx, cy - 15);
  tft.print("                 Current                                     Voltage");
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
    cur2[i] = 307;
  }
}
// Reset graph TFT_LIGHTGREY
void button0() {
  fillcur2();
  tft.fillRect(52, 142, 410, 165, TFT_BLACK);
  DrewdasheBorder();
  cc = CGREEN;
  cx = 52;
  att = true;
}

// Debug button
void button1() {
  if (att) {
    att = false;
    tft.fillRect(52, 142, 410, 165, TFT_BLACK);
    DrewdasheBorder();
    int rr = 0;
    for (int i = 0; i < 410; i++) {
      if (i == 0) { rr = cur2[i]; }
      tft.drawLine(52 + i, rr, 53 + i, cur2[i], CGREEN);
      rr = cur2[i];
      
    }
  } else {
    tft.fillRect(52, 142, 410, 165, TFT_BLACK);
    DrewdasheBorder();
    cc = CGREEN;
    cx = 52;
    att = true;
    
  }

  //    tft.drawPixel(52+i, cur2[i], CGREEN);
}

/*
String resp;
void PBImage() {
  Serial.println("===img Begin===");
  for (int x = 0; x <= 319; x++) {
    resp = "";
    for (int y = 0; y <= 479; y++) {
      resp = resp + tft.readPixel(x, y) + ",";
    }
    Serial.println(resp);
  }
  Serial.println("===img END===");
}
*/