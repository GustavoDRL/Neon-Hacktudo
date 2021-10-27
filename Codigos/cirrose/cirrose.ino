#include <PS4Controller.h>


#define PWMA 27
#define PWMB 26
#define A1  12
#define A2  13
#define B1  25
#define B2  33


#include <Servo_ESP32.h>
static const int brushPin = 14; 
Servo_ESP32 servo;
int angle;

void motors_control(int linear, int angular) {
  int result_R = linear - angular; //ao somar o angular com linear em cada motor conseguimos a ideia de direcao do robo
  int result_L = linear + angular;
  
  if(result_R<15 && result_R >-15) result_R=0; //não envia valores de potencia abaixo de 15, que não são fortes o suficiente para mover o robo
  if(result_L<15 && result_L >-15 ) result_L=0;
  
  motor_A(result_R); //manda para a funcao motor um valor de -255 a 255, o sinal signifca a direcao
  motor_B(result_L);
}

void motor_A(int speedA){  // se o valor for positivo gira para um lado e se for negativo troca o sentido
  if(speedA>0){  
    digitalWrite(A1, 1);
    digitalWrite(A2, 0);
  }else{
    digitalWrite(A1, 0);
    digitalWrite(A2, 1);
  }
  ledcWrite(5,abs( speedA));
}

void motor_B(int speedB){
  if(speedB>0){
    digitalWrite(B1, 1);
    digitalWrite(B2, 0);
  }else{
    digitalWrite(B1, 0);
    digitalWrite(B2, 1);
  }
  ledcWrite(6,abs( speedB));
}


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens


  PS4.begin("3c:8b:5b:8d:b6:e1");

  ledcAttachPin(PWMA,5);
  ledcAttachPin(PWMB,6);


  ledcSetup(5, 80000, 8);
  ledcSetup(6, 80000, 8);
  
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  digitalWrite(A1, 0);
  digitalWrite(A2, 0);
  digitalWrite(B1, 0);
  digitalWrite(B2, 0);

 servo.attach(brushPin);
 int angle = 0;
 servo.write(angle);
 while(PS4.isConnected()!= true){
  delay(20);}

}

void loop() {

  while(PS4.isConnected()) {
  // estou multiplicando por 1.8 para aumentar a velocidade linear, o quao rapido o robo vai ser
  // estou dividindo por 2 o angular, para o robô fazer menos curva e ser mais facil de controlar
  
  //motors_control(linear_speed*multiplicador, angular_speed* multiplicador2); 
    motors_control(PS4.LStickY()*1.8, PS4.RStickX()/2);
  
      
    if (PS4.Cross()) {
           
        angle=40;
        servo.write(angle);
        Serial.println(angle);
        delay(20);
    }

    
    if (PS4.Triangle()){
        angle=angle+2;
        servo.write(angle);
        Serial.println(angle);
        delay(20);
    } 
       
     
  } 
  if(PS4.isConnected()!= true){
  angle = 0;
  servo.write(angle);
  motors_control(0,0);
  Serial.println("Restart");
  setup();
  delay(50);
  }
  
}
