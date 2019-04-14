//Carrega a biblioteca do sensor ultrassonico
#include "Ultrasonic.h"
#include "HardwareSerial.h"

#define INTERVALO_LEITURA 250 //(ms)

//conexão dos pinos para o sensor ultrasonico
#define PIN_TRIGGER   4
#define PIN_ECHO      5

HardwareSerial UART(0);

//variável responsável 
unsigned int distancia = 0;

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO); //out, in

void setup(){
    UART.begin(115200);
    UART.println("Setup...");
}

void loop(){
    verificarDistancia();
    delay(INTERVALO_LEITURA);
}

void verificarDistancia(){
    //recupera a distância atual lida pelo sensor
    distancia = getDistance();
    UART.print("Distancia: ");
    UART.println(distancia);
}

int getDistance(){
    //faz a leitura das informacoes do sensor (em cm)
    int distanciaCM;
    long microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);

    //return distanciaCM;
    return distanciaCM;
}
/*
Exemplo tirado de: https://www.fernandok.com/2017/12/sensor-ultrassonico-com-esp32.html

*/
