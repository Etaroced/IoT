#define BLYNK_TEMPLATE_ID "TMPL6hhoBumur"
#define BLYNK_TEMPLATE_NAME "Assignment"

#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define SS_PIN D4
#define RST_PIN D3

#define LR_LED D1
#define BR_LED D2

int LR = 0;
int BR = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);

char auth[] ="gUyOLLuvvtpVddcL_AmFfSlfSxj5Dfx-";
const char* ssid = "Redmi K50"; 
const char* password = "03052002"; 

#include <Servo.h> 

Servo myServo;
SimpleTimer timer;

int fflag = 0; 

WidgetTerminal terminal(V2);
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  Blynk.begin(auth, ssid, password);

  pinMode(LR_LED, OUTPUT);
  pinMode(BR_LED, OUTPUT);

  SPI.begin();      // Initiate  SPI bus
  myServo.attach(D0); //servo pin D0
  myServo.write(0); //servo start position
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
  timer.setInterval(1000L, iot_rfid);
}

void loop() 
{
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}

void iot_rfid(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if( (content.substring(1) == "23 57 ED 0F")  && (fflag == 0))
  {
    Serial.println("Authorized access");
    Blynk.virtualWrite(V2, "USER, Authorized access" );
    Serial.println();
    delay(500);
    myServo.write(180);
    delay(4000);
    myServo.write(0);
  }
         
  else{
   Serial.println("Access denied");
   Blynk.virtualWrite(V2, "Id card is incorrect, access denied" );
  }
}
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
   fflag = param.asInt(); // assigning incoming value from pin V3 to a variable
}

BLYNK_WRITE(V3)
{
  int value = param.asInt();
  digitalWrite(LR_LED, value);
}

BLYNK_WRITE(V4)
{
  int value = param.asInt();
  digitalWrite(BR_LED, value);
}