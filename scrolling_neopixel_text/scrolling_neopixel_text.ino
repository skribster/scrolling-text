#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 8

String ledText = "Hello world";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP    + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(204, 0, 204), matrix.Color(204, 204, 0), matrix.Color(0, 255, 255),
  matrix.Color(255, 10, 127), matrix.Color(127, 0, 255), matrix.Color(0, 255, 0),
  matrix.Color(255, 99, 255)};

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}

int x = matrix.width();
int pass = 0;


// void displayMessage(char[] message) {
//   matrix.print(message);
// }


void updateLEDText() {
  // Bluetooth request is formatted as:
  // text,r,g,b\n

  if (Serial.available() > 0) {
    String incomingData = Serial.readStringUntil('\n'); 
    incomingData.trim(); // Remove whitespace

    int firstComma = incomingData.indexOf(',');
    if (firstComma == -1) return; // Invalid format

    int secondComma = incomingData.indexOf(',', firstComma + 1);
    if (secondComma == -1) return; // Invalid format

    int thirdComma = incomingData.indexOf(',', secondComma + 1);
    if (thirdComma == -1) return; // Invalid format

    // Extract substrings based on comma positions
    String textPart = incomingData.substring(0, firstComma);
    String rPart = incomingData.substring(firstComma + 1, secondComma);
    String gPart = incomingData.substring(secondComma + 1, thirdComma);
    String bPart = incomingData.substring(thirdComma + 1);

    // Update global variables and LED matrix
    ledText = textPart;
    matrix.setTextColor(matrix.Color(rPart.toInt(), gPart.toInt(), bPart.toInt()));
    pass = 0;
  }
}


void loop() {

  matrix.fillScreen(0);    //Turn off all the LEDs
  matrix.setCursor(x, 0);
  matrix.print(ledText);

  // matrix.setTextColor(colors[pass]);
  if( --x < -110 ) {
    updateLEDText();
    x = matrix.width();

    if(++pass >= 8) pass = 0;
  }

  matrix.show();
  delay(33);
}
