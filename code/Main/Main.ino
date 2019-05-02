#include "content.h"

SemaforoMotorista farolMotorista;
SemaforoPedestre farolPedestre;
TelaOLED tela;
SR04 ultrassom;
SDCard microSD;
MPU6050 acelerometro;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  pinMode(farolMotorista.vermelho, OUTPUT);
  pinMode(farolMotorista.amarelo, OUTPUT);
  pinMode(farolMotorista.verde, OUTPUT);
  pinMode(farolPedestre.vermelho, OUTPUT);
  pinMode(farolPedestre.verde, OUTPUT);
  microSD.iniciar();
  acelerometro.init();
}

void loop() {
  Serial.println(ultrassom.distancia());
  acelerometro.readAccel();
  farolMotorista.alternaLuz(farolMotorista.vermelho, farolMotorista.verde);
  tela.exibe(1, farolMotorista.msgVerde); //primeiro parâmetro informa ajuste de fonte
  delay(1000);
  tela.contagem(((int)farolMotorista.tempoVerde/1000)-1); //substitui a contagem comum de luzVerde
  delay(TEMPO_MUDANCA_MODAL); //trata do espaço entre uma mudança do principal com relacao ao pedestre
  farolPedestre.alternaLuz(farolPedestre.verde, farolPedestre.vermelho);
  //delay(farolMotorista.tempoVerde); //a ideia e substituir esses delays conforme funcoes forem adicionadas
  farolMotorista.alternaLuz(farolMotorista.verde, farolMotorista.amarelo);
  tela.exibe(1, farolMotorista.msgAmarelo);
  delay(farolMotorista.tempoAmarelo);
  farolMotorista.alternaLuz(farolMotorista.amarelo, farolMotorista.vermelho);
  tela.exibe(1, farolMotorista.msgVermelho);
  delay(TEMPO_MUDANCA_MODAL);
  farolPedestre.alternaLuz(farolPedestre.vermelho, farolPedestre.verde);
  delay(farolMotorista.tempoVermelho);  //tinha sido substituido pelo tempo de avaliar pedestre passando
  //for (int i=0; i < 10; i++) {  //voltar depois ao sr04 para dar um jeito nele
  //  ultrassom.distancia();
  //  delay(300);
  //}
  farolPedestre.alternaLuz(farolPedestre.verde, farolPedestre.vermelho);
  farolPedestre.vermelhoPiscante(3000);
}