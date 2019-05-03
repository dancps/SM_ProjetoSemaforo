#include "content.h"

SemaforoMotorista farolMotorista;
SemaforoPedestre farolPedestre;
TelaOLED tela;
SR04 ultrassom;
SDCard microSD;
MPU6050 acelerometro;
CloudService ts;
unsigned long tempoInicial, lastMillis;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  configuraFarois();
  microSD.iniciar();
  tempoInicial = millis();
  lastMillis = millis();
  Serial.print("Tempo de init microSD: ");
  Serial.println(millis() - tempoInicial);
  acelerometro.init();
  acelerometro.read();
  ts.init(acelerometro.AcX_norm,acelerometro.AcY_norm,acelerometro.AcZ_norm,acelerometro.Tmp_norm,acelerometro.GyX_norm,acelerometro.GyY_norm,acelerometro.GyZ_norm);
}

void loop() {
  Serial.println(ultrassom.distancia());  //PL REACTIVATE
  farolMotorista.alternaLuz(farolMotorista.vermelho, farolMotorista.verde);
  tela.exibe(1, farolMotorista.msgVerde); //primeiro parâmetro informa ajuste de fonte
  delay(1000);  //esse delay é colocado aqui para excluir um certo segundo antes da contagem
  tela.contagem(((int)farolMotorista.tempoVerde/1000)-1); //substitui a contagem comum de luzVerde
  delay(TEMPO_MUDANCA_MODAL); //trata do espaço entre uma mudança do principal com relacao ao pedestre
  farolPedestre.alternaLuz(farolPedestre.verde, farolPedestre.vermelho);
  //delay(farolMotorista.tempoVerde); //a ideia e substituir esses delays conforme funcoes forem adicionadas
  farolMotorista.alternaLuz(farolMotorista.verde, farolMotorista.amarelo);
  tela.exibe(1, farolMotorista.msgAmarelo);
  delay(farolMotorista.tempoAmarelo); //
  farolMotorista.alternaLuz(farolMotorista.amarelo, farolMotorista.vermelho);
  tela.exibe(1, farolMotorista.msgVermelho);
  delay(TEMPO_MUDANCA_MODAL); //
  farolPedestre.alternaLuz(farolPedestre.vermelho, farolPedestre.verde);
  long tempoInicial2 = millis(); //comeca contagem de demora de analise para descontar no resto de tempo
  analiseTrafego(); //gera um coeficiente que pode adicionar até 100% do tempo padrao
  long tempoFinal = millis();
  int desconto = tempoFinal-tempoInicial2;
  delay(farolPedestre.tempoVermelho+(farolPedestre.tempoVermelho*farolPedestre.coeficienteFluxo)-desconto); //até 2x o original
  //é tempo original + de 0 a 1x o tempo original, e o desconto da análise


  farolPedestre.alternaLuz(farolPedestre.verde, farolPedestre.vermelho);
  farolPedestre.vermelhoPiscante(3000);
  
  //int DELAY_TOTAL = 1000 + TEMPO_MUDANCA_MODAL + farolMotorista.tempoAmarelo + TEMPO_MUDANCA_MODAL + (farolPedestre.tempoVermelho+(farolPedestre.tempoVermelho*farolPedestre.coeficienteFluxo)-desconto);

  //Lê valores do acelerometro e atualiza suas variaveis
  acelerometro.read();
  acelerometro.print();
  //Atualiza valores no Thingspeak a cada 20 segundos
  if((millis() - lastMillis)/20000>0) ts.sendValues(acelerometro.AcX_norm,acelerometro.AcY_norm,acelerometro.AcZ_norm,acelerometro.Tmp_norm,acelerometro.GyX_norm,acelerometro.GyY_norm,acelerometro.GyZ_norm);
  lastMillis = millis();

  //Trecho de escrita do microSD
  String info;
  info += acelerometro.AcX_norm;
  info += ";";
  info += acelerometro.AcY_norm;
  info += ";";
  info += acelerometro.AcZ_norm;
  info += ";";
  info += acelerometro.Tmp_norm;
  info += ";";
  info += acelerometro.GyX_norm;
  info += ";";
  info += acelerometro.GyY_norm;
  info += ";";
  info += acelerometro.GyZ_norm;
  info += ";";
  info.concat("\n"); //concatena um caractere de escape correspondente a uma quebra de linha
  char entradaLog[sizeof(info)]; //cria um char para repassar ao const * char da função que dá append
  info.toCharArray(entradaLog, sizeof(entradaLog)); //pega a String e passa para o vetor de caracteres
  Serial.println("Gravando cartão...");
  microSD.appendFileWithoutPrint(SD, "/log.txt", entradaLog); //faz append do random no arquivo
}

void configuraFarois() {
  pinMode(farolMotorista.vermelho, OUTPUT);
  pinMode(farolMotorista.amarelo, OUTPUT);
  pinMode(farolMotorista.verde, OUTPUT);
  pinMode(farolPedestre.vermelho, OUTPUT);
  pinMode(farolPedestre.verde, OUTPUT);
}

void analiseTrafego() { //essa logica basicamente trata de incrementar em até 1x o tempo do pedestre
  int numeroRodadasPedestres = 5;
  for (int i=0; i < numeroRodadasPedestres; i++) {  //voltar depois ao sr04 para dar um jeito nele
    ultrassom.distancia();
    if(ultrassom.distanciaAtual < 10) {
      farolPedestre.coeficienteFluxo++;
    }
    delay(TEMPO_AMOSTRAS_PASSAGEM_PEDESTRE);
  }
  farolPedestre.coeficienteFluxo = farolPedestre.coeficienteFluxo/numeroRodadasPedestres;
  //vai dar de 0 a 1...
 }