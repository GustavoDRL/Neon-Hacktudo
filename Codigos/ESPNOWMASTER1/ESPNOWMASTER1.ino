 //Libs do espnow e wifi
#include <esp_now.h>
#include <WiFi.h>

//Canal usado para conexão
#define CHANNEL 1

//Pinos que iremos ler (digitalRead) e enviar para os Slaves
//É importante que o código fonte dos Slaves tenha este mesmo array com os mesmos gpios
//na mesma ordem
uint8_t gpios[] = {2, 3}; //inserir as portas

//No setup iremos calcular a quantidade de pinos e colocar nesta variável,
//assim não precisamos trocar aqui toda vez que mudarmos a quantidade de pinos,
//tudo é calculado no setup
int gpioCount;

//Mac Address dos slaves para os quais iremos enviar a leitura
//Se quiser enviar para todos os Slaves utilize apenas o endereço de broadcast {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}.
//Se quiser enviar para ESPs específicos coloque o Mac Address (obtido através da função WiFi.macAddress())
uint8_t macSlaves[6] = {0x34,0xB4,0x72,0x7F,0xED,0x24}; //inserir mac address
  //Se for enviar para ESPs específicos, coloque cada endereço separado por vírgula 34:B4:72:7F:ED:24
  // {0x24, 0x0A, 0xC4, 0x0E, 0x3F, 0xD1}]

esp_now_peer_info_t slave;


void setup() {
  Serial.begin(115200);

  //Cálculo do tamanho do array de gpios que serão lidos com o digitalRead
  //sizeof(gpios) retorna a quantidade de bytes que o array gpios aponta
  //Sabemos que todos os elementos do array são do tipo uint8_t
  //sizeof(uint8_t) retorna a quantidade de bytes que o tipo uint8_t possui
  //Sendo assim para saber quantos elementos o array possui
  //fazemos a divisão entre a quantidade total de bytes do array e quantos
  //bytes cada elemento possui
  gpioCount = sizeof(gpios)/sizeof(uint8_t);

  //Colocamos o ESP em modo station
  WiFi.mode(WIFI_STA);

  //Mostramos no Monitor Serial o Mac Address deste ESP quando em modo station
  Serial.print("Mac Address in Station: "); 
  Serial.println(WiFi.macAddress());

  //Chama a função que inicializa o ESPNow
  InitESPNow();

  int slavesCount = 1; //apenas 1 slave sendo utilizado

  
  slave.channel = CHANNEL;
  //0 para não usar criptografia ou 1 para usar
  slave.encrypt = 0;
  memcpy(slave.peer_addr, macSlaves, sizeof(macSlaves));
  esp_now_add_peer(&slave);
  esp_now_register_send_cb(OnDataSent);
   //Para cada pino que está no array gpios
  for(int i=0; i<gpioCount; i++){
    //Colocamos em modo de leitura
    pinMode(gpios[i], INPUT);
  }
    //Chama a função send
  send();
}

void InitESPNow() {
  //Se a inicialização foi bem sucedida
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  //Se houve erro na inicialização
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

//Função que irá fazer a leitura dos pinos
//que estão no array gpios e enviar os valores
//lidos para os outros ESPs
void send(){
  //Array que irá armazenar os valores lidos
  uint8_t values[gpioCount];

  //Para cada pino
  for(int i=0; i<gpioCount; i++){
    //Lê o estado do pino e armazena no array
    values[i] = digitalRead(gpios[i]);
  }
  esp_err_t result = esp_now_send(macSlaves, (uint8_t*) &values, sizeof(values));
  Serial.print("Send Status: ");
    //Se o envio foi bem sucedido
  if (result == ESP_OK) {
    Serial.println("Success");
  }
  //Se aconteceu algum erro no envio
  else {
    Serial.println("Error");
  }
}

//Função que serve de callback para nos avisar
//sobre a situação do envio que fizemos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  //Copiamos o Mac Address destino para uma string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", //verificar a função snprintf
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Mostramos o Mac Address que foi destino da mensagem
  Serial.print("Sent to: "); 
  Serial.println(macStr);
  //Mostramos se o status do envio foi bem sucedido ou não
  Serial.print("Status: "); 
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  //Enviamos novamente os dados
  send();
}

void loop(){
}
