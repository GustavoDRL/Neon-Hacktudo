# Neon-Hacktudo

## Descrição

Repositório com o código dos robôs de combate da equipe de robótica da UFABC Project Neon

## Instruções
### Windows
Como primeiro requisito a instalação temos o driver CH340, que pode ser instalado seguindo as instruções presentes em https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all . Em seguida é necessário adicionar a ESP32 a IDE do Arduíno (devido a um problema de compatibilidade com as bibliotecas utilizadas é necessário que seja instalada a versão 1.0.4 ou inferior ) como pode ser visto em https://www.fernandok.com/2018/09/instalando-esp32-no-arduino-ide-metodo.html. Por ultimo as bibliotecas arduino-esp32 e esp32-ps3 que estão dentro da pasta bibliotecas(https://www.robocore.net/tutoriais/adicionando-bibliotecas-na-ide-arduino.html).
### Linux

O primeiro passo é adicion a ESP32 a IDE do Arduíno (devido a um problema de compatibilidade com as bibliotecas utilizadas e necessário que seja instalada a versão 1.0.4 ou inferior ) como pode ser visto em https://www.fernandok.com/2018/09/instalando-esp32-no-arduino-ide-metodo.html. Por ultimo as bibliotecas arduino-esp32 e esp32-ps3 que estão dentro da pasta bibliotecas (https://www.robocore.net/tutoriais/adicionando-bibliotecas-na-ide-arduino.html). 

Para garantir que tudo esteja funcionando corretamente mais adiante sera importante que caso o comando para utilizar python seja python3 instalar o pacote python-is-python3 (seu funcionamento e explicado em https://newbedev.com/python-is-python3-package-in-ubuntu-20-04-what-is-it-and-what-does-it-actually-do).

sudo apt install python-is-python3

## Problemas

### Conectar controles diferentes em uma mesma ESP

#### Soluções
 Limpesa de memoria da ESP-32, provavelmente o problema ocorria devido a informacoes contidas na memorio flash. Para realizar esse processo utilizamos a ferramenta esptool ( https://github.com/espressif/esptool ).

##### Windows
Apos instalar o esptool basta rodar o seguinte comando:

python3 -m esptool --chip ESP32 --port <Porta_da_ESP> erase_flash	

##### Linux 
Apos instalar o esptool basta rodar o seguinte comando:

esptool.py --chip ESP32 --port <Caminho_da_porta> erase_flash 

## Considerações
Fork da biblioteca esp32-ps3 com melhorias na desconexão  
https://github.com/RomanLut/esp32-ps3/tree/1.0.0_with_reconnect_fixed
## Fontes

arduino-esp32 : https://github.com/espressif/arduino-esp32

esp32-ps3: https://github.com/jvpernis/esp32-ps3

