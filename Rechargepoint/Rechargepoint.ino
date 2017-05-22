#include<Servo.h>
#include <LiquidCrystal.h>
#include "SPI.h"
#include "MFRC522.h"
#define LED 10
#define SS_PIN 53
#define RST_PIN 42
String str[10] = {"C0:FA:1A:83", "B0:BA:60:7E"};
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
int money[10] = {0,0};
int chk = 0;
int cnt = 0;
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
Servo servo;
int angle;
int isObstaclePin = 9;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

void setup() {
 
  lcd.begin(16, 2);
  pinMode(isObstaclePin, INPUT);
  pinMode(LED, OUTPUT);
  servo.attach(8);
  servo.write(38);
  ///Serial.begin(9600);

  ///while (! Serial);


  delay(200);// sets the digital pin as output
  SPI.begin();
  rfid.PCD_Init();

}
void Alarm()
{
  lcd.clear();
  lcd.print("Restricted ");
  lcd.setCursor(0, 1);

  lcd.print("     Vehicle!!");
  Serial.println("I am in Alarm");
  digitalWrite(LED, HIGH);
  delay(4000);
  digitalWrite(LED, LOW);

  Check();

}
void Check()
{
  Serial.println("I am in check");
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(500);
    Check();
  }

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
   if (strID == "B0:BA:60:7E") Alarm();
  for (int i = 0; i < 10; i++) {
    if (strID == str[i]) cnt = i;
  }
 
   delay(1000);
  money[cnt]+=100;
  lcd.clear();
  lcd.print("Recharge   ");
  lcd.setCursor(0, 1);

  lcd.print("     Completed!!");
    delay(2000);
    lcd.clear();
  lcd.print("Thank you...");
  lcd.setCursor(0, 1);
  lcd.print("  For Recharging");
  delay(2000);
   
  ////serial.println("Here");
 Welcome1();

}

void Welcome()
{
  lcd.clear();
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Recharge Point");

}

void Welcome1()
{
  lcd.clear();
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("  Recharge Point");
Check();
}
void loop() {
  Welcome();
  delay(2000);
  Check();

}




