#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>
#include <Fonts/Picopixel.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
#define MSWITCH 15
#define WIFI_TIMEOUT_MS 20000


boolean up = false;
boolean down = false;
int increment = 0;
int selection = 0;
int nextBlink;
int top = 0;
int encPressed = 0;
int lastpos = 0;
String net_ssid = "";
String net_pass = "";
boolean network = false;
String mac = WiFi.macAddress();
String screen = "boot";
int networkAmount = 0;
int selectNumber = 0;
double timer = 0;
const char letters[] PROGMEM = "abcdefghijklmnopqrstuvwxyz0123456789";
const char capitalLetters[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
String * networksAvailable;
boolean blin = true;
String typer = "";


//ArrayList<String> pass = new ArrayList<String>();

// Encoder + Button
EncoderButton eb1(35, 32, 18);
void onEb1Clicked(EncoderButton& eb) {
  Serial.print("eb1 clickCount: ");
  Serial.println(eb.clickCount());
  encPressed = eb.clickCount();
}
void onEb1Encoder(EncoderButton& eb) {
  increment = eb.increment();
  if (selection + increment <= top && selection + increment >= 0) {
    selection += increment;
  }
}
void setTop(int newTop) {
  top = newTop;
}
void connectToWifi(String net_ssid, String net_pass) {
  Serial.print("Connecting to WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.begin(net_ssid.c_str(), net_pass.c_str());
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed!");
  }
}




// OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Bitmaps
const unsigned char backButton [] PROGMEM = {
  0x7f, 0xf8, 0x80, 0x04, 0x8c, 0x14, 0x98, 0x34, 0xbf, 0xf4, 0xbf, 0xe4, 0x98, 0x04, 0x8c, 0x04,
  0x80, 0x04, 0x7f, 0xf8
};
const unsigned char refreshButton [] PROGMEM = {
  0x00, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x20, 0x00, 0x40, 0x47, 0xfe, 0x20, 0x4f, 0xff, 0x20, 0x4f,
  0xff, 0x20, 0x4e, 0x07, 0x20, 0x40, 0x0f, 0xa0, 0x44, 0x07, 0x20, 0x4e, 0x02, 0x20, 0x5f, 0x00,
  0x20, 0x4e, 0x07, 0x20, 0x4f, 0xff, 0x20, 0x4f, 0xff, 0x20, 0x47, 0xfe, 0x20, 0x20, 0x00, 0x40,
  0x1f, 0xff, 0x80, 0x00, 0x00, 0x00
};
const unsigned char nextArrow [] PROGMEM = {
  0x7f, 0xf8, 0x80, 0x04, 0x80, 0x44, 0x80, 0x64, 0xbf, 0xf4, 0xbf, 0xf4, 0x80, 0x64, 0x80, 0x44,
  0x80, 0x04, 0x7f, 0xf8
};


// Main
void setup() {
  pinMode(MSWITCH, INPUT);
  while (network == false) {
    while (screen == "boot") {
      Serial.begin(9600);
      delay(500);
      eb1.setClickHandler(onEb1Clicked);
      eb1.setEncoderHandler(onEb1Encoder);

      if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
      }
      display.clearDisplay();
      screen = "wifi_scan";
    }
    while (screen == "wifi_scan") {
      display.setTextSize(3);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(40, 23);
      display.println("MOF");
      display.setTextSize(1);
      display.setCursor(4, 56);
      display.println(mac);
      display.display();
      delay(1500);
      display.clearDisplay();
      display.display();

      // WIFI SCAN
      display.clearDisplay();
      display.setTextSize(1.5);
      display.setCursor(26, 24);
      display.println("SEARCHING FOR");
      display.setCursor(15, 34);
      display.println("NEARBY NETWORKS...");
      display.display();
      networkAmount = WiFi.scanNetworks();
      display.clearDisplay();

      if (networkAmount == 0) {
        display.clearDisplay();
        display.setCursor(13, 28);
        display.println("No networks found.");
        display.display();
        delay(2000);
        display.clearDisplay();
        display.setCursor(30, 28);
        display.println("rescanning...");
        display.display();
        delay(2000);
        while (networkAmount == 0) {
          networkAmount = WiFi.scanNetworks();
          delay(500);
        }
      }

      //        networksAvailable = new String[networkAmount];
      Serial.println("setting length");
      networksAvailable = new String[networkAmount];
      Serial.println("set length");
      for (int i = 0; i < networkAmount; i++) {
        networksAvailable[i] = WiFi.SSID(i);
        Serial.println(networksAvailable[i]);
      }
      screen = "wifi_selection";
    }

    while (screen == "wifi_selection") {
      setTop(networkAmount - 1);
      eb1.update();
      display.clearDisplay();

      display.setCursor(0, 2);
      display.setTextSize(1);
      display.println("Choose a network");
      display.drawLine(0, 12, 128, 12, 1);

      display.setCursor(7, 15);
      if (selection - 1 >= 0) {
        if (networksAvailable[selection - 1].length() > 18) {
          display.println("" + networksAvailable[selection - 1].substring(0, 16) + "...");
        }
        else {
          display.println("" + networksAvailable[selection - 1]);
        }
      }
      display.setCursor(0, 25);
      if (selection < networkAmount) {
        if (networksAvailable[selection].length() > 18) {
          display.println(">" + networksAvailable[selection].substring(0, 16) + "...");
        }
        else {
          display.println(">" + networksAvailable[selection]);
        }
      }
      display.setCursor(7, 35);
      if (selection + 1 < networkAmount) {
        if (networksAvailable[selection + 1].length() > 18) {
          display.println("" + networksAvailable[selection + 1].substring(0, 16) + "...");
        }
        else {
          display.println("" + networksAvailable[selection + 1]);
        }
      }
      display.setCursor(7, 45);
      if (selection + 2 < networkAmount) {
        if (networksAvailable[selection + 2].length() > 18) {
          display.println("" + networksAvailable[selection + 2].substring(0, 16) + "...");
        }
        else {
          display.println("" + networksAvailable[selection + 2]);
        }
      }
      display.setCursor(7, 55);
      if (selection + 3 < networkAmount) {
        if (networksAvailable[selection + 3].length() > 18) {
          display.println("" + networksAvailable[selection + 3].substring(0, 16) + "...");
        }
        else {
          display.println("" + networksAvailable[selection + 3]);
        }
      }
      display.display();
      if (encPressed == 1) {
        encPressed = 0;
        if (selection < networkAmount) {
          net_ssid = networksAvailable[selection];
          screen = "wifi_password";
        }
      }
    }

    while (screen == "wifi_password") {
      setTop(35);
      eb1.update();
      display.clearDisplay();
      if (millis() > timer) {
        Serial.println("millis pased timer");
        timer = millis() + 500;
        if (blin == true) {
          blin = false;
        }
        else {
          blin = true;
        }
      }
      display.setCursor(0, 2);
      display.setFont();
      display.setTextSize(1);
      display.println("Enter Password:");
      display.drawLine(0, 12, 128, 12, 1);
      display.drawRect(5, 22, 117, 15, 1);
      display.drawRoundRect(54, 42, 21, 22, 3, 1);
      display.setFont(&FreeSans9pt7b);
      if (digitalRead(MSWITCH) == LOW) {
        // If selecting I:
        if (selection == 5 || selection == 8 || selection == 9 || selection == 11 || selection == 19) {
          display.setCursor(61, 57);
        }
        else {
          display.setCursor(59, 57);
        }
        display.println(letters[selection]);
      }
      else if (digitalRead(MSWITCH) == HIGH) {
        // If showing W:
        if (selection == 22) {
          display.setCursor(56, 58);
        }
        else {
          display.setCursor(57, 58);
        }
        display.println(capitalLetters[selection]);
      }
      display.setFont();

      // Draw Back
      display.drawBitmap(1, 53, backButton, 14, 10, WHITE);
      display.setFont(&Picopixel);
      display.setCursor(17, 57);
      display.println("Triple");
      display.setCursor(17, 63);
      display.println("click");

      // Draw Next
      display.drawBitmap(113, 53, nextArrow, 14, 10, WHITE);
      display.setCursor(88, 57);
      display.println("Double");
      display.setCursor(94, 63);
      display.println("click");


      display.setFont();
      display.setCursor(9, 26);
      if (blin == true) {
        display.println(typer + "_");
      }
      else {
        display.println(typer);
      }

      if (encPressed == 3) {
        typer = "";
        encPressed == 0;
        net_ssid = "";
        screen = "wifi_selection";
      }


      if (encPressed == 1) {
        encPressed = 0;
        blin = true;
        if (digitalRead(MSWITCH) == HIGH) {
          typer += capitalLetters[selection];
        }
        else if (digitalRead(MSWITCH) == LOW) {
          typer += letters[selection];
        }
      }

      if (encPressed == 2) {
        encPressed = 0;
        net_pass = typer;
        display.drawRoundRect(7, 9, 109, 47, 4, 0);
        display.drawRoundRect(8, 10, 107, 45, 4, 1);
        display.fillRoundRect(9, 11, 105, 43, 3, 0);
        display.setFont(&FreeSans9pt7b);
        display.setCursor(11, 35);
        display.println("connecting...");
        display.display();
        connectToWifi(net_ssid, net_pass);
        if (WiFi.status() != WL_CONNECTED) {
          display.fillRoundRect(9, 11, 105, 43, 3, 0);
          display.setCursor(25, 35);
          display.println("Failed");
          display.display();
          delay(2000);
        }
        else {
          if (WiFi.RSSI() < -80) {
            display.fillRoundRect(9, 11, 105, 43, 3, 0);
            display.setCursor(23, 35);
            display.println("too weak");
            display.display();
            delay(2000);
          }
          else {
            display.fillRoundRect(9, 11, 105, 43, 3, 0);
            display.setCursor(9, 28);
            display.println("connected to");
            display.setCursor(11, 43);
            display.println(net_ssid);
            screen = "main";
            network = "true";
            display.display();
            delay(2000);
            screen = "main";
          }
        }
      }
      display.setFont();
      display.display();
      increment = 0;
    }

    while (screen == "") {

    }

  }
}

int add1(int top) {
  if (selectNumber >= top)  {
    selectNumber = 0;
  }
  else  {
    selectNumber ++;
  }
  Serial.println(selectNumber);
  return selectNumber;
}

int subtract1(int top) {
  if (selectNumber <= 0)  {
    selectNumber = top;
  }
  else  {
    selectNumber --;
  }
  Serial.println(selectNumber);
  return selectNumber;
}




String networkPrompt(int networksAmount, String networksAvailable[]) {
  while (net_ssid == "") {
    networkSP(networksAmount, networksAvailable);
  }
}






void networkSP(int networksAmount, String networksAvailable[]) {

}




void loop() {
  display.clearDisplay();
  display.display();
  delay(1000);
}
