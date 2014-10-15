#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
SoftwareSerial bt(5, 4); // RX, TX
int outputPin = 8;
String btStringBuffer;
String allowedIDs[] = {
  "bafc6fd5", //karla
  "9aed7bdb", //gochi
  "5af14bdb", //rene
  "a1881db", //ruy
  "4a6d42db", //spiro
  "4aba80db", //gerardo
  "7ad160db", //JC
  "baf831db", //Abraham
  "2aac65dc", //Jey-El
};

void setup() {
  Serial.begin(115200);	// Initialize serial communications with the PC
  bt.begin(9600);
  //btSetup();
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
  if( bt.available() ) {
    while( bt.available() ) {
      btStringBuffer += (char)bt.read();
    }
  }
  Serial.println(btStringBuffer);
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
  
  // Send ID request to Internet
  bt.println(cardID);
  
  // Wait for response
  while( ! bt.available() ) {}
  
  if( bt.available() ) {
    while( bt.available() ) {
      btStringBuffer += (char)bt.read();
    }
  }
    // "bafc6fd5"
  if (btStringBuffer == "true") {
    Serial.println("Access!!!");
    bt.println("Access!!!");
    
    //myservo.write(180);
    beep(true);
    delay(2000);
    //myservo.write(0);
  } else {
    Serial.println("Access NOT ALLOWED.");
    bt.println("Access NOT ALLOWED.");
    //beep(false);
  }
  btStringBuffer= "";
  bt.flush();
}

void btSetup() {
  // Starts AT command state
  bt.println("AT");
  // Perform synchronous communication
  while( bt.available() ) {
    Serial.println(bt.read());
  }
  // Sets device name
  bt.println("AT+NAME=DOOR-729");
  while( bt.available() ) {
    Serial.println(bt.read());
  }
  // Sets device PIN
  bt.println("AT+PSWD=2274");
  while( bt.available() ) {
    Serial.println(bt.read());
  }
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
