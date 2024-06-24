#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

void loop() {
  if (getFingerprintEnroll() == FINGERPRINT_OK) {
    Serial.println("Template captured and sent to computer.");
  } else {
    Serial.println("Failed to capture template.");
  }
  delay(5000); // Wait 5 seconds before trying again
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.println("Waiting for valid finger to enroll...");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) {
      delay(100);
    } else if (p != FINGERPRINT_OK) {
      Serial.println("Error capturing image");
      return p;
    }
  }

  Serial.println("Image captured, processing...");
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Error processing image");
    return p;
  }

  Serial.println("Image converted, creating template...");
  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Error creating template");
    return p;
  }

  Serial.println("Template created, sending to computer...");
  sendFingerprintTemplate();

  return FINGERPRINT_OK;
}

void sendFingerprintTemplate() {
  // Read the template data from the fingerprint module
  uint8_t templateBuffer[512]; // Adjust size based on your module's template size
  uint16_t templateLength;

  if (finger.getModel() != FINGERPRINT_OK) {
    Serial.println("Error reading template");
    return;
  }

  finger.getFingerprintTemplate(templateBuffer, &templateLength);

  // Send the template data to the computer over serial
  for (int i = 0; i < templateLength; i++) {
    Serial.write(templateBuffer[i]);
  }
  Serial.println(); // To mark the end of the template data
}
