#define trigPin1 9
#define echoPin1 8

#define trigPin2 7
#define echoPin2 10

#define Buzzer 6 
#define Led 5   
#define Vibrator 4 

#define ButtonPin 3       
#define LightSensorPin A0 
const int lightThreshold = 787; 

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
    // Vertical sensor pins
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(Buzzer, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(Vibrator, OUTPUT); 
  
pinMode(ButtonPin, INPUT_PULLUP); 
}

void loop() {
  long duration1, distance1;
  long duration2, distance2; 
  
 // ---------------------- HORIZONTAL OBSTACLE -------------------------
  
  digitalWrite(trigPin1, LOW);
  delay(2);
  digitalWrite(trigPin1, HIGH);
  delay(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;

  // Handle invalid measurements
  if (duration1 == 0 || distance1 > 300) { 
    distance1 = 300;
  }

  Serial.print(" HORIZONTAL Distance: ");
  Serial.println(distance1);
  
  // ------------------------ VERTICAL ALTITUDE -------------------------
  
  digitalWrite(trigPin2, LOW);
  delay(2);
  digitalWrite(trigPin2, HIGH);
  delay(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;

  if (distance2 > 300) {
    distance2 = 0 ;
  }

  Serial.print("Vertical Altitude: ");
  Serial.print(distance2);
  Serial.println(" cm");

    // ------------------------ ACTIONS ---------------------------------

  // Reset outputs
  digitalWrite(Buzzer, LOW);
  digitalWrite(Led, LOW);
  digitalWrite(Vibrator, LOW);

  // Step 1: H(20–40 cm) V(5–10 cm)
  if (distance1 <= 40 && distance1 > 20 || distance2 >= 5 && distance2 <= 10) {
    digitalWrite(Led, HIGH);
    tone(Buzzer, 1000);       // 1 kHz beep
    digitalWrite(Vibrator, HIGH); 
    delay(500);               
    noTone(Buzzer);
    digitalWrite(Vibrator, LOW);
    delay(500);
  }

  // Step 2: H(10–20 cm) V(10–20 cm)
  if (distance1 <= 20 && distance1 > 10 || distance2 <= 20 && distance2 > 10) {
    digitalWrite(Led, HIGH); 
    tone(Buzzer, 1000);       // 1 kHz beep
    digitalWrite(Vibrator, HIGH); 
    delay(200);               
    noTone(Buzzer);
    digitalWrite(Vibrator, LOW);
    delay(200);
  }

  // Step 3: H(<10 cm) V(>20 cm)
  if (distance1 <= 10 ||  distance2 > 20) {
    digitalWrite(Led, HIGH); 
    tone(Buzzer, 2000);       // Continuous 2 kHz beep
    digitalWrite(Vibrator, HIGH); 
  }

   // ------------------------ LIGHT SENSOR & BUTTON ---------------------
  int lightLevel = analogRead(LightSensorPin); 
  bool buttonPressed = digitalRead(ButtonPin) == LOW; 

  if (buttonPressed) {
    Serial.println("Button Pressed!");

    if (lightLevel > lightThreshold) {
      Serial.println("Light detected above threshold!");      
      Serial.println(lightLevel);

      // Vibrate if light is above threshold
      digitalWrite(Vibrator, HIGH);
      delay(500);
      digitalWrite(Vibrator, LOW);
      delay(200);
    } else {
      Serial.println("Light below threshold.");
      Serial.println(lightLevel);
      delay(200);


    }
  }
  
  
}
