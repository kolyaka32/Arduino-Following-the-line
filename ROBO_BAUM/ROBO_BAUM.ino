// Макроопределение для подключения датчика линии к пину №2 Arduino
//#define PIN_SENSOR  2
#define LIGHT_PIN_L 10
#define LIGHT_PIN_R 11

#define MOTOR_PIN_L_F 3  // Motor 1 pin - left moving forward
#define MOTOR_PIN_L_B 5  // Motor 2 pin - left moving backward
#define MOTOR_PIN_R_B 6  // Motor 3 pin - right moving backward
#define MOTOR_PIN_R_F 9  // Motor 4 pin - right moving forward

#define NOR_SPEED 100 // Need speed for turning for motors
#define TURN_FRONT 120  // Need speed for motors for turning
#define TURN_BACK 200

//#define DEV 1

bool BlackZone = false;
int counter = 0;

// Turning robot left
void goingLeft(){
  analogWrite(MOTOR_PIN_L_F, 0);
  analogWrite(MOTOR_PIN_L_B, TURN_BACK);
  analogWrite(MOTOR_PIN_R_B, 0);
  analogWrite(MOTOR_PIN_R_F, TURN_FRONT);
};

// Turning robot right
void goingRight(){
  analogWrite(MOTOR_PIN_R_B, TURN_BACK);
  analogWrite(MOTOR_PIN_R_F, 0);
  analogWrite(MOTOR_PIN_L_B, 0);
  analogWrite(MOTOR_PIN_L_F, TURN_FRONT);
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
  pinMode(LIGHT_PIN_L, INPUT);
  pinMode(LIGHT_PIN_R, INPUT);
  
  pinMode(MOTOR_PIN_L_F, OUTPUT);
  pinMode(MOTOR_PIN_R_F, OUTPUT);
  pinMode(MOTOR_PIN_L_B, OUTPUT);
  pinMode(MOTOR_PIN_R_B, OUTPUT);


  delay(1000);
}

// Main part
void loop() {
  // Контролируем датчик с периодичностью около 200мс
  bool lightLeft = !digitalRead(LIGHT_PIN_L);//1-black;0-white
  bool lightRight = !digitalRead(LIGHT_PIN_R);
  if( !(lightLeft) && !(lightRight) ){  // Both light
    BlackZone = false;  // If we get back to all light zone - switchig back to normal mode
    if(counter == 2){
      // Getting on finish
      analogWrite(MOTOR_PIN_L_F, 0);
      analogWrite(MOTOR_PIN_L_B, 0);
      analogWrite(MOTOR_PIN_R_F, 0);
      analogWrite(MOTOR_PIN_R_B, 0);
      while(true){
        delay(10);
      }
    }
    forward();  // Both forward

  }
  else if( lightLeft && lightRight ){//Both black
    // If we get on all right zone - switching to inverce mode
    /*if(!BlackZone){
      counter++;
    }
    else{*/
      BlackZone = true;
    
    BlackZone = true;  
    forward();
  }
  // Rotating
  else if( ((!lightLeft) && lightRight) ){
    // Going left
    if(BlackZone){
      goingRight();
    }
    else{
      goingLeft();
    }
  }
  else if( lightLeft && (!lightRight) ){
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
