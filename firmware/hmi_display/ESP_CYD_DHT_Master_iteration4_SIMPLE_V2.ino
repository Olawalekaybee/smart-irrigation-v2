// /**************************CrowPanel ESP32 HMI Display Example Code************************
// Version     :	1.1
// Suitable for:	CrowPanel ESP32 HMI Display
// Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
// Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
// Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
// Description	:	The code is currently available based on the course on YouTube, 
// 				        if you have any questions, please refer to the course video: Introduction 
// 				        to ask questions or feedback.
// **************************************************************/


// #include <TFT_eSPI.h>
// /**************************TFT_eSPI************************
// If you don't know how to configure TFT_eSPI library for the display and touch driver
// Please refer to the content of the fifth lesson
// **************************************************************/
// #include <Arduino.h>
// #include <SPI.h>
// #include <XPT2046_Touchscreen.h>

// // The CYD touch uses some non default
// // SPI pins

// #define XPT2046_IRQ 36
// #define XPT2046_MOSI 32
// #define XPT2046_MISO 39
// #define XPT2046_CLK 25
// #define XPT2046_CS 33

// #define I2C_DEV_ADDR 0x55
// #define I2C_SDA 27
// #define I2C_SCL 22


// #define CMD_SENSOR 0x01
// #define CMD_FLAG   0x02
// #define CMD_ALL 0x03

// uint8_t credStatusFlag = 0;
// /**************************LVGL and UI************************
// if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
// if not, please do not include it. It will waste your Flash space.
// **************************************************************/
// #include <lvgl.h>
// // #include <demos/lv_demos.h>
// // #include <examples/lv_examples.h>
// #include "ui.h"
// /**************************LVGL and UI END************************/

// #include "Wire.h"
// #include "time.h"
// #include <OneWire.h>
// // #include <DallasTemperature.h>

// #define I2C_DEV_ADDR 0x55

// // volatile float temp = 0.0;
// // volatile bool temphumidtyReceived = false;

// // int count = 0;
// float threshold1;
// float threshold2;
// bool wifiCredsUpdated = false;


// char wifi_ssid[32] = {};
// char wifi_password[32] = {};

// // const char* savedSSID = get_wifi_ssid();
// // const char* savedPASS = get_wifi_password();

// const char* savedSSID = "your_wifi_name";
// const char* savedPASS = "your_wifi_password";
// /*******************************************************************************
//  * Please define the corresponding macros based on the board you have purchased.
//  * CrowPanel_24 means CrowPanel ESP32 HMI 2.4inch Board
//  * CrowPanel_28 means CrowPanel ESP32 HMI 2.8inch Board
//  * CrowPanel_35 means CrowPanel ESP32 HMI 3.5inch Board
//  ******************************************************************************/
// //#define CrowPanel_24
// #define CrowPanel_28
// //#define CrowPanel_35

// #if defined (CrowPanel_35)
// /*screen resolution*/
// static const uint16_t screenWidth  = 480;
// static const uint16_t screenHeight = 320;
// uint16_t calData[5] = { 353, 3568, 269, 3491, 7  };     /*touch caldata*/

// #elif defined (CrowPanel_24)
// static const uint16_t screenWidth  = 320;
// static const uint16_t screenHeight = 240;
// uint16_t calData[5] = { 557, 3263, 369, 3493, 3  };

// #elif defined (CrowPanel_28)
// static const uint16_t screenWidth  = 320;
// static const uint16_t screenHeight = 240;
// uint16_t calData[5] = { 189, 3416, 359, 3439, 1 };
// #endif

// // SPIClass mySpi = SPIClass(HSPI); // touch does not work with this setting
// SPIClass mySpi = SPIClass(VSPI); // critical to get touch working
// XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
// TFT_eSPI lcd = TFT_eSPI(); /* TFT entity */

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf1[ screenWidth * screenHeight / 13];


// float temp = 0.0;
// float humidity = 0.0;
// bool tempHumReceived = false;
// float tempD = 0.0;
// float tempD2 = 0.0;

// int count = 0;
// bool wifi_status = false;


// void requestAllData() {
//   Wire.beginTransmission(I2C_DEV_ADDR);
//   Wire.write(CMD_ALL); // new combined command
//   Wire.endTransmission();

//   delay(5); // allow slave to prep

//   const uint8_t totalBytes = 1 + 16; // 1 flag + 4 floats
//   if (Wire.requestFrom(I2C_DEV_ADDR, totalBytes) == totalBytes) {
//     credStatusFlag = Wire.read(); // first byte is flag
//     byte buffer[16];
//     Wire.readBytes(buffer, 16);
//     memcpy(&temp,     &buffer[0],  sizeof(float));
//     memcpy(&humidity, &buffer[4],  sizeof(float));
//     memcpy(&tempD,    &buffer[8],  sizeof(float));
//     memcpy(&tempD2,   &buffer[12], sizeof(float));
//     tempHumReceived = true;
    
//     // if (credStatusFlag == 1){
//     //   lv_label_set_text(ui_Label5, "Wifi ON");
//     // }
//     // else{
//     //   lv_label_set_text(ui_Label5, "Wifi OFF");
//     // }

//     Serial.printf("Received flag: %d | Temp: %.2f°C, TempD: %.2f°C, TempD2: %.2f°C, Hum: %.2f%%\n",
//       credStatusFlag, temp, tempD, tempD2, humidity);
//   } else {
//     Serial.println("No combined data received!");
//   }
// }


// void sendWiFiCredentials() {
//   Wire.beginTransmission(0x55);
  
//   uint8_t ssidLen = strlen(wifi_ssid);
//   uint8_t passLen = strlen(wifi_password);
  
//   // Send lengths first
//   Wire.write(ssidLen);
//   Wire.write(passLen);
  
//   // Send actual strings
//   Wire.write((uint8_t*)wifi_ssid, ssidLen);
//   Wire.write((uint8_t*)wifi_password, passLen);
  
//   uint8_t floatBytes1[4];
//   memcpy(floatBytes1, &threshold1, sizeof(float));  // copy bytes safely 
//   Wire.write(floatBytes1, sizeof(float));  // send 4 bytes
//   Serial.print("Master sends threshold1: ");
//   Serial.println(threshold1, 4);

  
//   uint8_t floatBytes2[4];
//   memcpy(floatBytes2, &threshold2, sizeof(float));  // copy bytes safely
  
//   Wire.write(floatBytes2, sizeof(float));  // send 4 bytes
//   Serial.print("Master sends threshold1: ");
//   Serial.println(threshold2, 4);

  

//   Wire.endTransmission();
//  // Serial.println(threshold);
//   delay(10);
//   Serial.println("WiFi credentials sent to slave");
// }


// //_______________________
// /* display flash */
// void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
// {
//   uint32_t w = ( area->x2 - area->x1 + 1 );
//   uint32_t h = ( area->y2 - area->y1 + 1 );

//   lcd.startWrite();
//   lcd.setAddrWindow( area->x1, area->y1, w, h );
//   lcd.pushColors( ( uint16_t * )&color_p->full, w * h, true );
//   lcd.endWrite();

//   lv_disp_flush_ready( disp );
// }

// uint16_t touchX, touchY;
// // /*touch read*/
// void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
// {

//  //bool touched = lcd.getTouch( &touchX, &touchY, 600);
//   bool touched = (ts.tirqTouched() && ts.touched()); // this is the version needed for XPT2046 touchscreen
//   if ( !touched )
//   {
//     data->state = LV_INDEV_STATE_REL;
//   }
//   else
//   {
//      // the following three lines are specific for using the XPT2046 touchscreen
//     TS_Point p = ts.getPoint();
//     touchX = map(p.x, 200, 3700, 1, screenWidth);  /* Touchscreen X calibration */
//     touchY = map(p.y, 240, 3800, 1, screenHeight); /* Touchscreen X calibration */
//     data->state = LV_INDEV_STATE_PR;

//     /*set location*/
//     data->point.x = touchX;
//     data->point.y = touchY;

//     Serial.print( "Data x " );
//     Serial.println( touchX );

//     Serial.print( "Data y " );
//     Serial.println( touchY );
//   }
// }

// void setup()
// { 
//   delay(100);
//   Serial.begin( 115200 ); /*serial init */
//   Wire.setPins(I2C_SDA, I2C_SCL);
//   Wire.begin(I2C_SDA, I2C_SCL);
//   // Wire.onReceive(onReceive);
//   // Wire.onRequest(onRequest);
 
//  Serial.println("I2C Slave ready on address 0x55...");
//   //Port_D
//   pinMode(25, OUTPUT);
//   digitalWrite(25, LOW);

//   //LCD init
//   lcd.begin();          
//   lcd.setRotation(1); 
//   lcd.fillScreen(TFT_BLACK);
//   lcd.setTouch(calData);
//   delay(100);
//   //background light pin
//   pinMode(27, OUTPUT);
//   digitalWrite(27, HIGH);

//   //lvgl init
//   lv_init();

//   mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
//   ts.begin(mySpi);                                                  /* Touchscreen init */
//   ts.setRotation(1);                                                /* Landscape orientation */

//   lcd.begin();        /* TFT init */
//   lcd.setRotation(1); // Landscape orientation  1 =  CYC usb on right, 2 for vertical
  
//   lv_disp_draw_buf_init( &draw_buf, buf1, NULL, screenWidth * screenHeight / 13 );

//   /* Initialize the display */
//   static lv_disp_drv_t disp_drv;
//   lv_disp_drv_init( &disp_drv );
//   /* Change the following line to your display resolution */
//   disp_drv.hor_res = screenWidth;
//   disp_drv.ver_res = screenHeight;
//   disp_drv.flush_cb = my_disp_flush;
//   disp_drv.draw_buf = &draw_buf;
//   lv_disp_drv_register( &disp_drv );

//   /* Initialize the (dummy) input device driver */
//   static lv_indev_drv_t indev_drv;
//   lv_indev_drv_init( &indev_drv );
//   indev_drv.type = LV_INDEV_TYPE_POINTER;
//   indev_drv.read_cb = my_touchpad_read;
//   lv_indev_drv_register( &indev_drv );

//   lcd.fillScreen(TFT_BLACK);

// //lv_arc_set_range(ui_Arc1, 0, 100);

//   //lv_demo_widgets();    // LVGL demo
//   ui_init();


//   lv_timer_t * timer = lv_timer_create(my_timer, 1000,  NULL);

//   Serial.println( "Setup done" );
// }

// void my_timer(lv_timer_t * timer) {
//   static uint32_t lastUpdate = 0;
//   if (millis() - lastUpdate > 1000) { // Only update once per second
//     requestAllData();

//     if (wifiCredsUpdated) {
//       sendWiFiCredentials();
//       // request_wifi_status();
//       wifiCredsUpdated = false;   // Reset flag after sending
//     }

//     if (tempHumReceived) {
//     char temp1[32];
//     char hum[32];
//     char temp2[32];
//     char temp3[32];
    
//     sprintf(temp1, "T: %.2f C", temp);
//     sprintf(hum, "H: %.2f %%",  humidity);
//     sprintf(temp2, "T: %.2f C",  tempD);  //dallas temp1
//     sprintf(temp3, "T: %.2f C",  tempD2);  //dallas temp2 

// //   // Update arc gauge value
//     lv_arc_set_value(ui_Arc1, (int16_t)temp); // Cast to int since arc uses integers
   
//     lv_arc_set_value(ui_Arc2, (int16_t)humidity); // Cast to int since arc uses integers

//     lv_arc_set_value(ui_Arc3, (int16_t)tempD); // Cast to int since arc uses integers

//        // Update arc gauge value
//     lv_arc_set_value(ui_Arc4, (int16_t)tempD2); // Cast to int since arc uses integers

        
// // Update arc's center label
//     lv_label_set_text(ui_Label15, temp1);
//     lv_label_set_text(ui_Label16, hum);
//     lv_label_set_text(ui_Label18, temp3);
//     lv_label_set_text(ui_Label17, temp2);
    
    
  
//       tempHumReceived = false;
//   }
//     lastUpdate = millis();
//   }
// }

// void loop()
// {

//   static uint32_t lastGoodData = 0;
  
//   lv_timer_handler();
  
//   if (tempHumReceived) {
//     Serial.printf("New temperature received: %.2f °C\n", temp);
//    // lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_1, temp);
//     tempHumReceived = false;
   
//     lastGoodData = millis();
//   }
//   else if (millis() - lastGoodData > 2000) { // If no data for 2 seconds
//     Serial.println("No I2C data - attempting recovery...");
//     //recoverI2C();
//     lastGoodData = millis(); // Reset timer
//   }
  
//   delay(1);
// }

#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <lvgl.h>
#include "ui.h"


lv_obj_t * ui_LabelTitle;
lv_obj_t * ui_LabelPot1;
lv_obj_t * ui_LabelRelay1;
lv_obj_t * ui_LabelRelay2;


// ================= I2C =================
#define SLAVE_ADDR 0x08

// ================= TOUCH =================
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// ================= DISPLAY CONFIG =================
#define CrowPanel_28

#if defined (CrowPanel_28)
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = { 189, 3416, 359, 3439, 1 };
#endif

// ================= DISPLAY =================
SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI lcd = TFT_eSPI();

// ================= LVGL =================
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenWidth * screenHeight / 13];

// ================= DATA =================
int16_t s = 0;
int16_t L = 0;
int16_t pot,relay1, relay2 = 0;
float t = 0.0;
float h = 0.0;

volatile bool tempHumReceived = false;
uint8_t buffer[18];

// ================= I2C RECEIVE =================
void receiveEvent(int bytes)
{
  Serial.print("Bytes received: ");
  Serial.println(bytes);

  if (bytes != 18) {
    while (Wire.available()) Wire.read();
    Serial.println("Invalid packet");
    return;
  }

  for (int i = 0; i < 18; i++) {
    buffer[i] = Wire.read();
  }

  memcpy(&s, buffer, 2);
  memcpy(&L, buffer + 2, 2);
  memcpy(&t, buffer + 4, 4);
  memcpy(&h, buffer + 8, 4);
  memcpy(&pot, buffer + 12, 2);
  memcpy(&relay1, buffer + 14, 2);
  memcpy(&relay2, buffer + 16, 2);

  Serial.println("---- RECEIVED ----");
  Serial.print("S: "); Serial.println(s);
  Serial.print("L: "); Serial.println(L);
  Serial.print("T: "); Serial.println(t);
  Serial.print("H: "); Serial.println(h);
  Serial.print("Int E: "); Serial.println(pot);

  tempHumReceived = true;
}

// ================= DISPLAY FLUSH =================
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

// ================= TOUCH =================
uint16_t touchX, touchY;

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  bool touched = (ts.tirqTouched() && ts.touched());

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
    return;
  }

  TS_Point p = ts.getPoint();

  touchX = map(p.x, 200, 3700, 1, screenWidth);
  touchY = map(p.y, 240, 3800, 1, screenHeight);

  data->state = LV_INDEV_STATE_PR;
  data->point.x = touchX;
  data->point.y = touchY;
}

// ================= UI UPDATE =================
void updateUI()
{
  char buf1[32], buf2[32], buf3[32], buf4[32],buf5[32], buf6[32], buf7[32];

  snprintf(buf1, sizeof(buf1), "T: %.2f C", t);
  snprintf(buf2, sizeof(buf2), "H: %.2f %%", h);
  snprintf(buf3, sizeof(buf3), "S: %d", s);
  snprintf(buf4, sizeof(buf4), "L: %d", L);
  snprintf(buf5, sizeof(buf5), "pot: %d", pot);
  snprintf(buf6, sizeof(buf6), "RELAY1: %d", relay1);
  snprintf(buf7, sizeof(buf7), "RELAY2: %d", relay2);

  

  lv_label_set_text(ui_Label15, buf1);
  lv_label_set_text(ui_Label16, buf2);
  lv_label_set_text(ui_Label17, buf3);
  lv_label_set_text(ui_Label18, buf4);
  lv_label_set_text(ui_LabelPot1, buf5);

  // Optional arcs
  lv_arc_set_value(ui_Arc1, (int)t);
  lv_arc_set_value(ui_Arc2, (int)h);
  lv_arc_set_value(ui_Arc3, s);
  lv_arc_set_value(ui_Arc4, L);

   lv_label_set_text(ui_LabelRelay1, (s > pot) ? "Relay1: ON" : "Relay1: OFF");
  lv_label_set_text(ui_LabelRelay2, (L > pot) ? "Relay2: ON" : "Relay2: OFF");

    lv_label_set_text(ui_LabelTitle, "SYSTEM DASHBOARD");
}

// void updateUI()
// {
//   char buf[32];

//   snprintf(buf, sizeof(buf), "Pot1: %d", s);
//   lv_label_set_text(ui_LabelPot1, buf);

//   snprintf(buf, sizeof(buf), "Pot2: %d", L);
//   lv_label_set_text(ui_LabelPot2, buf);

//   lv_label_set_text(ui_LabelRelay1, (s > 50) ? "Relay1: ON" : "Relay1: OFF");
//   lv_label_set_text(ui_LabelRelay2, (L > 50) ? "Relay2: ON" : "Relay2: OFF");

//   lv_label_set_text(ui_LabelTitle, "SYSTEM DASHBOARD");
// }

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);

  // I2C SLAVE INIT
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  // BACKLIGHT (CRITICAL)
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  // LCD INIT
  lcd.begin();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  lcd.setTouch(calData);

  // LVGL INIT
  lv_init();

  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screenWidth * screenHeight / 13);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();
  
  // ================= CUSTOM UI LABELS =================

  // Title (center)
  ui_LabelTitle = lv_label_create(ui_Screen3);
  lv_label_set_text(ui_LabelTitle, "SYSTEM DASHBOARD");
  lv_obj_align(ui_LabelTitle, LV_ALIGN_TOP_MID, 0, 5);

  // Pot labels
  ui_LabelPot1 = lv_label_create(ui_Screen3);
  lv_label_set_text(ui_LabelPot1, "Pot: 0");
  lv_obj_align(ui_LabelPot1, LV_ALIGN_LEFT_MID, 3, 50);

  // Relay labels
  ui_LabelRelay1 = lv_label_create(ui_Screen3);
  lv_label_set_text(ui_LabelRelay1, "Relay1: OFF");
  lv_obj_align(ui_LabelRelay1, LV_ALIGN_LEFT_MID, 2, 3);

  
  ui_LabelRelay2 = lv_label_create(ui_Screen3);
  lv_label_set_text(ui_LabelRelay2, "Relay2: OFF");
  lv_obj_align(ui_LabelRelay2, LV_ALIGN_RIGHT_MID, 0,0);

  Serial.println("CYD SLAVE READY");
}

// ================= LOOP =================
void loop()
{
  static uint32_t lastGoodData = 0;

  lv_timer_handler();

  if (tempHumReceived) {

    Serial.printf("Update UI -> S:%d L:%d T:%.2f H:%.2f\n", s, L, t, h);

    updateUI();

    tempHumReceived = false;
    lastGoodData = millis();
  }
  else if (millis() - lastGoodData > 2000) {
    Serial.println("No I2C data...");
    lastGoodData = millis();
  }

  delay(5);
}
