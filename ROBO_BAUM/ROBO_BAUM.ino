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

#define NOR_SPEED 90    // Need speed for turning for motors
#define TURN_FRONT 150   // Need speed for motors for turning
#define TURN_BACK 120    // Need speed

#define DEV 1

bool BlackZone = false;
int counter = 0;
float light = 0;

// Turning robot left
void goingLeft(){
  analogWrite(MOTOR_PIN_L_F, 0);
  analogWrite(MOTOR_PIN_L_B, TURN_BACK);//*abs(light));
  analogWrite(MOTOR_PIN_R_B, 0);
  analogWrite(MOTOR_PIN_R_F, TURN_FRONT);//*abs(light));
};

// Turning robot right
void goingRight(){
  analogWrite(MOTOR_PIN_R_B, TURN_BACK);//*abs(light));
  analogWrite(MOTOR_PIN_R_F, 0);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_L_F, TURN_FRONT);//*abs(light));
};

// Moving forward0
void forward(){
  analogWrite(MOTOR_PIN_L_F, NOR_SPEED);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_R_F, NOR_SPEED);
  analogWrite(MOTOR_PIN_R_B, 0);
};

void setup() {
  // Инициализируем работу с серийным портом для вывода отладочной информации

  // Настраиваем на вход пин, к которому подключен датчик линии.
  // Здесь нет необходимости включать внутреннюю подтяжку пина к VCC, т.к.
  // модуль сам заботится о формировании логической "1" при отсутствии цели

  // Initialasing input pins for light sensors
  pinMode(LIGHT_PIN_R_F, INPUT);
  pinMode(LIGHT_PIN_R_N, INPUT);
  pinMode(LIGHT_PIN_L_N, INPUT);
  pinMode(LIGHT_PIN_L_F, INPUT);
  
  pinMode(MOTOR_PIN_L_F, OUTPUT);
  pinMode(MOTOR_PIN_R_F, OUTPUT);
  pinMode(MOTOR_PIN_L_B, OUTPUT);
  pinMode(MOTOR_PIN_R_B, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  #ifdef DEV
  Serial.begin(9600);
  #endif

  delay(1000);
}

// Main part
void loop() {

  // Getting volatge
  //volt = 9 / analogRead_VCC();
  
  // New control system with prioritets
  int input = 0; // Variable for selecting need turn speed
  // Plus number - left
  // Minus number - right
  
  // Far sensors more important (?)
  input += 3 * digitalRead(LIGHT_PIN_L_F);
  input -= 3 * digitalRead(LIGHT_PIN_R_F);

  // Near sensors
  input += 2 * digitalRead(LIGHT_PIN_L_N);
  input -= 2 * digitalRead(LIGHT_PIN_R_N);

  light = light * 0.5 + input * 0.7;

  #ifdef DEV
  Serial.println(light);
  #endif

  // Selecting need mode
  if( abs(light) < 1.0 ){  // near none turn
    // Getting, if both black
    forward();  // Both forward
    
    int black = 0;
    black += digitalRead(LIGHT_PIN_L_F);
    black += digitalRead(LIGHT_PIN_R_F);
    black += digitalRead(LIGHT_PIN_L_N);
    black += digitalRead(LIGHT_PIN_R_N);

    if(BlackZone && black < 2){
      counter++;
      BlackZone = false;
      digitalWrite(LED_PIN, LOW);
    }
    else if(!BlackZone && black > 3){
      BlackZone = true;
      digitalWrite(LED_PIN, HIGH);
    }
    /*if(counter == 2){
        // Getting on finish
        analogWrite(MOTOR_PIN_L_F, 0);
        analogWrite(MOTOR_PIN_L_B, 0);
        analogWrite(MOTOR_PIN_R_F, 0);
        analogWrite(MOTOR_PIN_R_B, 0);
        // Waiting
        while(true){
          delay(10);
        }
      }*/
  }

  // Rotating
  else if( light < 0 ){
    // Going left
    if(BlackZone){
      goingRight();
    }
    else{
      goingLeft();
    }
  }
  else if( light > 0 ){
    // Going right
    if(BlackZone){
      goingLeft();
    }
    else{
      goingRight();
    }
  }
  //delay(10);
}
