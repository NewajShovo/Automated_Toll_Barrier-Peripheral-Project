#include<Servo.h>
#include <LiquidCrystal.h>
#include "SPI.h"
#include "MFRC522.h"
#define LED 10
#define SS_PIN 53
#define RST_PIN 42
String str[10] = {"C0:FA:1A:83", "B0:BA:60:7E"};
// initialize the library with the numbers of the interface pins

/////Serial.begin(9600);

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
int money[10] = {100, 200};
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

void Less_money(int id) {
  lcd.clear();
  lcd.print("Not Enough Money");
  delay(2000);
  lcd.clear();
  lcd.print("You Have to");
  lcd.setCursor(0, 1);
  lcd.print("  Recharge Money");
  delay(2000);
  
  
 /// Check();
}

void Greetings() {
  lcd.clear();
  lcd.print("Have A");
  lcd.setCursor(0, 1);

  lcd.print("   Safe Journey...");
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
  if (money[cnt] >= 50) {
    Serial.println(strID);
    money[cnt] -= 50;
    Serial.println(money[cnt]);
    Greetings();

  }
  else {
    Less_money(cnt);
     Welcome();
      Check();

  }


  delay(500);
  for (int i = 38; i <= 128; i++)
  {
    servo.write(i);
    delay(45);
  }

  Object();

}


void Object()
{
  delay(600);

  isObstacle = digitalRead(isObstaclePin);

  if (isObstacle == LOW) {
    Welcome();
    for (int i = 128; i >= 38; i--) {
      servo.write(i);
      delay(30);

    }
    Check();
  }

  Object();
}
void Welcome()
{
  lcd.clear();
  lcd.print("Welcome to Padma");
  lcd.setCursor(0, 1);

  lcd.print("   Toll  Plaza  ");

}


void loop() {
  Welcome();

  Check();
  Object();

}




