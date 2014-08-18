int gainPin = 3;
int sensorPin = A0;

void write(int value) {
  analogWrite(gainPin, value);
  Serial.print(value);
  Serial.print("\n");
}

void setup() {
  pinMode(gainPin, OUTPUT);
  Serial.begin(9600);
  write(255);
}

typedef unsigned long levelType;
const levelType averageScale = 200;

unsigned long tickAverageLevel() {
  static levelType average = 0;
  
  levelType current = analogRead(sensorPin);
  
  const levelType newProportion = 1;
  const levelType oldProportion = averageScale - newProportion;
  
  average = (average * oldProportion) / averageScale + current * newProportion;
  
  return average;
}

levelType goalLevel = 2 * averageScale;
int outputLevel;
const int outputLevelShift = 6;
const int outputLevelMax = 255 << outputLevelShift;

int iteration;
void loop() {
  levelType average = tickAverageLevel();
  
  if(average > goalLevel) {
    outputLevel--;
    if(outputLevel < 0)
      outputLevel = 0;
  } else if(average < goalLevel) {
    outputLevel++;
    if(outputLevel > outputLevelMax)
      outputLevel = outputLevelMax;
  }
  int writeLevel = outputLevel >> outputLevelShift;
  analogWrite(gainPin, writeLevel);
  
  if(iteration++ > 1000) {
    iteration = 0;
    Serial.print(average);
    Serial.print(" ");
    Serial.println(writeLevel);
  }
}
