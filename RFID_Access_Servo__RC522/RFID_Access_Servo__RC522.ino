#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
SoftwareSerial bt(4, 5); // RX, TX
int outputPin = 8;

void setup() {
  Serial.begin(115200);	// Initialize serial communications with the PC
  SPI.begin();		// Init SPI bus
  mfrc522.PCD_Init();	// Init MFRC522 card
  Serial.println("RC522 ready.");
  pinMode(outputPin, OUTPUT);  
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent() ) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }
  checkAccess();
}

void checkAccess() {  
  // Dump UID
  String cardID = "";
  Serial.print("Read card #");
  for(byte i = 0; i < mfrc522.uid.size; i++) {
    //cardKey += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    cardID += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(cardID);
  
  Serial.println();
  if (cardID == "bafc6fd5") {
    Serial.println("Access!!!");
    
    //myservo.write(180);
    beep(true);
    delay(2000);
    //myservo.write(0);
  }
  beep(false);
}

void beep(boolean access) {
  switch (access) {
    case true:
      digitalWrite(outputPin, HIGH);
      delay(1000);
      digitalWrite(outputPin, LOW);
      break;
    case false:
      digitalWrite(outputPin, HIGH);
      delay(100);
      digitalWrite(outputPin, LOW);
      delay(100);
      digitalWrite(outputPin, HIGH);
      delay(100);
      digitalWrite(outputPin, LOW);
      delay(100);
      digitalWrite(outputPin, HIGH);
      delay(100);
      digitalWrite(outputPin, LOW);
      break;
  }
}
