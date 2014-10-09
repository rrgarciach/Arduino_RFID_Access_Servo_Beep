#include <SoftwareSerial.h>
#include <PN532.h>
#include <Servo.h> 


#define SCK 13
#define MOSI 11
#define SS 10
#define MISO 2 // switched from pin 12

PN532 nfc(SCK, MISO, MOSI, SS);
SoftwareSerial bt(4, 5); // RX, TX

Servo myservo;
int pos = 0; 

void setup(void) {
  pinMode(8, OUTPUT);
  myservo.attach(9);
  myservo.write(0);
    Serial.begin(115200);
    Serial.println("Hello!");

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
        Serial.print("Didn't find PN53x board");
        while (1); // halt
    }
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    Serial.print("Supports "); Serial.println(versiondata & 0xFF, HEX);

    // configure board to read RFID tags and cards
    nfc.SAMConfig();
    
    // setup interrupt to detect when a new card reading is comming from the module
    // interrupt uses digital pin 2 (interrupt 0 on Arduino Uno)
    attachInterrupt(0, ISR_INTO, CHANGE);
}


void loop(void) {
}

void btSetup() {
  // Starts AT command state
  bt.write("AT\r\n");
  // Perform synchronous communication
  while( !bt.avaiable() ) {
  }
  Serial.println(bt.read());
  // Sets device name
  bt.write("AT+NAME=DOOR\r\n");
  // Sets device PIN
  bt.write("AT+PIN=\r\n");
}

void checkAccess() {
  uint32_t id;
  // look for MiFare type cards
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
  
  if (id != 0) {
      Serial.print("Read card #"); Serial.println(id);
  }
  
  // checks for hardcoded IDs
  if (id==984486619 || id==3137105877 || id==52531444) {
    Serial.println("Access garanted!!");
    
    myservo.write(180);
    beep(true);
    delay(2000);
    myservo.write(0);
    //myservo.detach();
  }
  else if (id!=0) {
    Serial.println("Acces denied");
    beep(false);
  }
  //delay(1000);
  Serial.println("Stand by...");
}

void beep(boolean access) {
  switch (access) {
    case true:
      digitalWrite(8, HIGH);
      delay(1000);
      digitalWrite(8, LOW);
      break;
    case false:
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
      delay(100);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
      delay(100);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
      break;
  }
}

