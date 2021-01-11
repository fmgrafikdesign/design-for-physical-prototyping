
const float floatingAverageDuration = 0.1; // in minutes
const int calculationsPerMinute = 240;

const int ledPins[2] = { 2 };
const int numberOfMeasureSpots = 1;
const int floatingAverageArrayLength = floatingAverageDuration * calculationsPerMinute;
int resistancePins[numberOfMeasureSpots] = { A0 } ;
int motorPins[numberOfMeasureSpots] = { 4 };
int floatingAverage[numberOfMeasureSpots][floatingAverageArrayLength] = {{}};
int total[numberOfMeasureSpots];
int readIndex = 0;
bool firstLoop = true;
bool ledsOn = false;
const long del = (1000L * 60) / calculationsPerMinute;


int threshold = 930;


void setup() {
  // initialize serial communications at 9600 bps:

  Serial.begin(9600);

  for (int i = 0; i < sizeof(ledPins); i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i = 0; i < numberOfMeasureSpots; i++) {
    pinMode(resistancePins[i], INPUT);
    pinMode(motorPins[i], OUTPUT);
  }
}

void loop() {

  for (int i = 0; i < numberOfMeasureSpots; i++) {
    int resistance = analogRead(resistancePins[i]);
    total[i] = total[i] - floatingAverage[i][readIndex];

    floatingAverage[i][readIndex] = resistance;

    total[i] = total[i] + floatingAverage[i][readIndex];

    int average = 0;
    if (!firstLoop) {
      average = total[i] / floatingAverageArrayLength;
    } else {
      average = total[i] / (readIndex + 1);
    }

    DebugLog(resistancePins[i], average, resistance);

    if (average < threshold) {
      ledsOn = true;
      digitalWrite(motorPins[i], HIGH);
    } else {
      digitalWrite(motorPins[i], LOW);
    }
  }

  // Turn LEDs on if a patch is over threshold
  if (ledsOn) {
    for (int j = 0; j < sizeof(ledPins); j++) {
      digitalWrite(ledPins[j], HIGH);
    }
  } else {
    for (int j = 0; j < sizeof(ledPins); j++) {
      digitalWrite(ledPins[j], LOW);
    }
  }

  readIndex++;
  if (readIndex >= floatingAverageArrayLength) {
    readIndex = 0;
    firstLoop = false;
  }

  ledsOn = false;

  delay(del);
}

void DebugLog(int pin, int average, int resistance) {
    Serial.print("Average for resistance pin ");
    Serial.print(pin);
    Serial.print(" is ");
    Serial.print(average);
    Serial.print(". Last reading: ");
    Serial.println(resistance);
}
