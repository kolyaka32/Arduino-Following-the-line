//Light sensors
#define LIGHT_PIN_L_F 8  // left far from center sensor
#define LIGHT_PIN_L_N 12  // left near center sensor
#define LIGHT_PIN_R_N 11  // right near center sensor
#define LIGHT_PIN_R_F 10  // right far from center sensor

#define MOTOR_PIN_L_F 3  // Motor 1 pin - left moving forward
#define MOTOR_PIN_L_B 5  // Motor 2 pin - left moving backward
#define MOTOR_PIN_R_B 6  // Motor 3 pin - right moving backward
#define MOTOR_PIN_R_F 9  // Motor 4 pin - right moving forward

#define LED_PIN 13

#define NOR_SPEED 140    // Need speed for turning for motors
#define TURN_FRONT 200   // Need speed for motors for turning
#define TURN_BACK 180    // Need speed

#define FORWARD_BORDER 1.5  // Border value to move forward

// PID proportions
#define PID_P 1.5  // Prev: 1
#define PID_I 0.2  // Prev: 0.1
#define PID_D 3.0  // Prev: 2.0
#define INTEGRAL_MUL 0.95
#define MAX_INTEGRAL 10

#define turnTime 1000000  // Timer, before last timer
#define finishCounter 10

// PID variables
int input = 0;
int oldInput = 0;
float integral = 0;
float output = 0;

#define DEV 1

bool BlackZone = false;
int blackCounter = 0;
int timerStart = 0;

// Turning robot left
void goingLeft(){
  analogWrite(MOTOR_PIN_L_F, 0);
  analogWrite(MOTOR_PIN_L_B, TURN_BACK);// * fabs(output)));
  analogWrite(MOTOR_PIN_R_B, 0);
  analogWrite(MOTOR_PIN_R_F, TURN_FRONT);// * fabs(output)));
};

// Turning robot right
void goingRight(){
  analogWrite(MOTOR_PIN_R_B, TURN_BACK);// * fabs(output)));
  analogWrite(MOTOR_PIN_R_F, 0);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_L_F, TURN_FRONT);// * fabs(output)));
};

// Moving forward0
void forward(){
  analogWrite(MOTOR_PIN_L_F, NOR_SPEED);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_R_B, 0);
  analogWrite(MOTOR_PIN_R_F, NOR_SPEED);
};

// All stopping and waiting function
void stopping(){
  // Getting on finish
  analogWrite(MOTOR_PIN_L_F, 0);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_R_F, 0);
  analogWrite(MOTOR_PIN_R_B, 0);
  // Waiting
  while(true){
    delay(10);
  }
}

void setup() {
  // Initialasing input pins for light sensors
  pinMode(LIGHT_PIN_R_F, INPUT);
  pinMode(LIGHT_PIN_R_N, INPUT);
  pinMode(LIGHT_PIN_L_N, INPUT);
  pinMode(LIGHT_PIN_L_F, INPUT);

  // Initialasing motor output pins
  pinMode(MOTOR_PIN_L_F, OUTPUT);
  pinMode(MOTOR_PIN_R_F, OUTPUT);
  pinMode(MOTOR_PIN_L_B, OUTPUT);
  pinMode(MOTOR_PIN_R_B, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  #ifdef DEV
  Serial.begin(9600);
  #endif

  // Starting delay
  delay(1000);

  // Getting start timer for last turn
  timerStart = millis();
}

// Main part
void loop() {
  // New control system with prioritets
  input = 0; // Variable for selecting need turn speed
  // Plus number - left
  // Minus number - right
  
  // Far sensors more important (?)
  input += 2 * digitalRead(LIGHT_PIN_L_F);  // Prev: 3 * 
  input -= 2 * digitalRead(LIGHT_PIN_R_F);  // Prev: 3 * 

  // Near sensors
  input += 2 * digitalRead(LIGHT_PIN_L_N);  // Prev: 2 * 
  input -= 2 * digitalRead(LIGHT_PIN_R_N);  // Prev: 2 * 

  // PID regulator
  integral = integral * INTEGRAL_MUL + input;
  if(integral > MAX_INTEGRAL){
    integral = MAX_INTEGRAL;
  }
  else if(integral < -MAX_INTEGRAL){
    integral = -MAX_INTEGRAL;
  }

  float output = input * PID_P + integral * PID_I + (input - oldInput) * PID_D;
  oldInput = input;

  #ifdef DEV
  Serial.print(input);
  Serial.print(" ");
  Serial.println(output);
  #endif


  // Selecting need mode
  if( abs(output) < FORWARD_BORDER ){  // near none turn
    // Getting, if both black
    forward();  // Both forward
    
    int black = 0;
    black += digitalRead(LIGHT_PIN_L_F);
    black += digitalRead(LIGHT_PIN_R_F);
    black += digitalRead(LIGHT_PIN_L_N);
    black += digitalRead(LIGHT_PIN_R_N);

    if(BlackZone && black < 3){
      blackCounter++;
      BlackZone = false;
      digitalWrite(LED_PIN, LOW);
    }
    else if(!BlackZone && black > 3){
      BlackZone = true;
      integral = 0.0;
      digitalWrite(LED_PIN, HIGH);
    }
    if(blackCounter == finishCounter){
        stopping();
    }
  }

  // Turning
  else if( output < 0 ){
    // Going left
    if(BlackZone){
      goingRight();
    }
    else{
      goingLeft();
    }
  }
  else if( output > 0 ){
    // Going right
    if(BlackZone){
      goingLeft();
    }
    else{
      goingRight();
    }
  }
  // Checking on final turn as cheat
  if(millis() - timerStart > turnTime){
    stopping();
  }
  //delay(10);
}
