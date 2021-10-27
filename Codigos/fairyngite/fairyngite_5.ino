  #include <PS4Controller.h>
  
  #define PWMA 27
  #define PWMB 26
  #define A1  13
  #define A2  12
  #define B1  25
  #define B2  33
  
  
  #include <Servo_ESP32.h>
  static const int brushPin = 14; 
  Servo_ESP32 servo;
  int angle;
  int inv = 1; //permite inverter a pilotagem conforme o lado do robo que esta para cima
  void motors_control(int linear, int angular) {
    int result_R = linear - angular; //ao somar o angular com linear em cada motor conseguimos a ideia de direcao do robo
    int result_L = linear + angular;
    
    if(result_R<15 && result_R >-15) result_R=0; //não envia valores de potencia abaixo de 15, que não são fortes o suficiente para mover o robo
    if(result_L<15 && result_L >-15 ) result_L=0;
    if(result_R >255 ) result_R= 254; //nao permite valores superiores a 255 ou inferiores a -255
    if(result_R<-255 ) result_R=-254;
    if(result_L >255 ) result_L= 254;
    if(result_L<-255 ) result_L=-254;  
    
      
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
    ledcWrite(5,abs(speedA));

  }
  
  void motor_B(int speedB){
    if(speedB>0){
      digitalWrite(B1, 1);
      digitalWrite(B2, 0);
    }else{
      digitalWrite(B1, 0);
      digitalWrite(B2, 1);
    }
    ledcWrite(6,abs(speedB));
    Serial.println("speedB");
    Serial.println(speedB);
    delay(100);
  }
  
  
  void setup(void) {
    Serial.begin(115200);
    while (!Serial)
      delay(10); // will pause Zero, Leonardo, etc until serial console opens
  
  
    PS4.begin("00:e0:4c:65:4d:0f");
    Serial.println("Ready.");
    
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
      if(PS4.LStickY()<-15 || PS4.LStickY()>15){
        float multiplic_curva = map(PS4.LStickY(), -124, 125, 1, 2.3);
        motors_control(1.8*inv*PS4.LStickY(), (-1)*PS4.RStickX()/multiplic_curva);
      }else { // Controle sobre valores pequenos que bugam a funcao map
        motors_control(1.8*inv*PS4.LStickY(), (-1)*PS4.RStickX());
      }
      
        
      if (PS4.Cross()) { 
          angle=40;
          servo.write(angle);
          Serial.println(angle);
          delay(20);
      }
  
       if(PS4.R3()){
          inv = -1;
          delay(100);
       }
        if(PS4.L3()){
          inv = 1;
          delay(100);
       }
       if(PS4.R1()){
          servo.detach();
          delay(100);
          servo.attach(brushPin);
          angle = 40;
          servo.write(angle);
          Serial.println(angle);
  
       }else if(PS4.R2()){
          if(PS4.R2Value()>15){
            angle = map(PS4.R2Value(), 0, 255, 20, 140);
          }else{
            angle = 40;
          }
                  
          servo.write(angle);
          Serial.println(angle);
       }   
    } 
    if(PS4.isConnected()!= true){
    angle = 40;
    servo.write(angle);
    motors_control(0,0);
    Serial.println("Restart");
    setup();
    delay(20);
    }
    
  }
