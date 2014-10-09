#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
SoftwareSerial bt(4, 5); // RX, TX

void setup() {
  Serial.begin(115200);	// Initialize serial communications with the PC
  SPI.begin();		// Init SPI bus
  mfrc522.PCD_Init();	// Init MFRC522 card
  Serial.println("Scan a MIFARE Classic PICC to demonstrate Value Blocks.");
  //attachInterrupt(0, checkAccess, FALLING);
  //attachInterrupt(1, checkAccess, FALLING);
  pinMode(2, OUTPUT);  
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
  // Now a card is selected. The UID and SAK is in mfrc522.uid.
/*
  // Dump UID
  String cardID = "";
  Serial.print("Card UID:");
  for(byte i = 0; i < mfrc522.uid.size; i++) {
    //cardKey += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    cardID += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(cardID);
  Serial.println( checkAccess(cardID) );
  
  Serial.println();

  // Dump PICC type
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("PICC type: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  if (	piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    &&	piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&	piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("This sample only works with MIFARE Classic cards.");
    return;
  }
  
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for(byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // In this sample we use the second sector (ie block 4-7).
  byte sector		= 1;
  byte valueBlockA	= 5;
  byte valueBlockB	= 6;
  byte trailerBlock	= 7;
	
  // Authenticate using key A.
  Serial.println("Authenticating using key A...");
  byte status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // We need a sector trailer that defines blocks 5 and 6 as Value Blocks and enables key B.
  byte trailerBuffer[] = { 255,255,255,255,255,255,  0,0,0,  0,  255,255,255,255,255,255}; // Keep default keys.
  // g1=6 => Set block 5 as value block. Must use Key B towrite & increment, A or B can be used for derement.
  // g2=6 => Same thing for block 6.
  // g3=3 => Key B must be used to modify the Sector Trailer. Key B becomes valid. 
  mfrc522.MIFARE_SetAccessBits(&trailerBuffer[6], 0, 6, 6, 3);

  // Now we read the sector trailer and see if it is like we want it to be.
  Serial.println("Reading sector trailer...");
  byte buffer[18];
  byte size = sizeof(buffer);
  status = mfrc522.MIFARE_Read(trailerBlock, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Read() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  if (buffer[6] != trailerBuffer[6]
  && buffer[7] != trailerBuffer[7]
  && buffer[8] != trailerBuffer[8]) {
    Serial.println("Writing new sector trailer...");
    status = mfrc522.MIFARE_Write(trailerBlock, trailerBuffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("MIFARE_Write() failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }

  // Authenticate using key B.
  Serial.println("Authenticating again using key B...");
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Value blocks has a 32 bit signed value stored three times and an 8 bit address stored 4 times.
  // Make sure blocks valueBlockA and valueBlockB has that format.
  formatBlock(valueBlockA);
  formatBlock(valueBlockB);

  // Add 1 to the value of valueBlockA and store the result in valueBlockA.
  Serial.print("Adding 1 to value of block "); Serial.println(valueBlockA);
  status = mfrc522.MIFARE_Increment(valueBlockA, 1);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Increment() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Transfer(valueBlockA);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Transfer() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Dump the result
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
*/
/*
void formatBlock(byte blockAddr) {
  Serial.print("Reading block "); Serial.println(blockAddr);
  byte buffer[18];
  byte size = sizeof(buffer);
  byte status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Read() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  if ( (buffer[0] == (byte)~buffer[4]) 
  && (buffer[1] == (byte)~buffer[5]) 
  && (buffer[2] == (byte)~buffer[6]) 
  && (buffer[3] == (byte)~buffer[7]) 
  && (buffer[0] == buffer[8]) 
  && (buffer[1] == buffer[9]) 
  && (buffer[2] == buffer[10]) 
  && (buffer[3] == buffer[11]) 
  && (buffer[12] == (byte)~buffer[13]) 
  && (buffer[12] ==        buffer[14]) 
  && (buffer[12] == (byte)~buffer[15])) {
    Serial.println("Block has correct Block Value format.");
  }
  else {
    Serial.println("Writing new value block...");
    byte valueBlock[] = { 0,0,0,0,  255,255,255,255,  0,0,0,0,   blockAddr,~blockAddr,blockAddr,~blockAddr };
    status = mfrc522.MIFARE_Write(blockAddr, valueBlock, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("MIFARE_Write() failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
  }
} // End formatBlock()
*/
void checkAccess() {
  // Dump UID
  String cardID = "";
  Serial.print("Card UID:");
  for(byte i = 0; i < mfrc522.uid.size; i++) {
    //cardKey += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    cardID += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(cardID);
  
  Serial.println();
  if (cardID == "bafc6fd5") {
    Serial.println("Access!!!");
    digitalWrite(2,HIGH);
    delay(2000);
    digitalWrite(2,LOW);
    //return true;
  }
  //return false;
}
