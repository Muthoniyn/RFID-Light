#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

uint8_t blue[4] = {0x69, 0x45, 0x01, 0x9E};
uint8_t card[4] = {0xC3, 0x12, 0x6E, 0x31};
uint8_t led = 6;
unsigned long lastOpenMS = 0;

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  pinMode(led, OUTPUT);
}

void loop() {

  if (millis() - lastOpenMS > 3000) {
    digitalWrite(led, LOW);
    //lastOpenMS = millis();
  }
  
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  if (memcmp(card, mfrc522.uid.uidByte, mfrc522.uid.size) == 0) {
    Serial.println("Card");
    digitalWrite(led, HIGH);
    lastOpenMS = millis();
  }

  if (memcmp(blue, mfrc522.uid.uidByte, mfrc522.uid.size) == 0) {
    Serial.println("FOB");
    digitalWrite(led, LOW);
  }

}
