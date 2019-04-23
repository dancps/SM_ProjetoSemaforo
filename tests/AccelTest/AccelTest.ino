//Programa : Teste MPU6050 e LCD 20x4
//Alteracoes e adaptacoes : FILIPEFLOP
//
// Também usei como referência a biblioteca
// de Korneliusz Jarzębski.
//
//Baseado no programa original de JohnChi
//
//			FUNCIONANDO
//

//Pinos de conexao I2C
#define SDA_PIN 21
#define SCL_PIN 22
//Endereco I2C do MPU6050
#define MPU 0x68  
 
//Carrega a biblioteca Wire
#include "Wire.h"
#include "HardwareSerial.h"

//Inicializa uma comunicacao serial
HardwareSerial UART(0);

//Variaveis para armazenar valores dos sensores
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double AcX_norm,AcY_norm,AcZ_norm,Tmp_norm,GyX_norm,GyY_norm,GyZ_norm;
double gravity=9.80665;
double sensitityRange=0.000061;
byte temp_h,temp_l;
void setup()
{
    UART.begin(115200);
    //Inicializa o LCD
    Wire.begin(SDA_PIN,SCL_PIN); // tb é possível passar um 3o argumento com a I2C Bus Speed
    Wire.beginTransmission(MPU);
    Wire.write(0x6B); // PWR_MGMT_1 register
     
    //Inicializa o MPU-6050
    Wire.write(0); 
    Wire.endTransmission(true);
}

void loop(){
    Wire.beginTransmission(MPU);

    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    //Solicita os dados do sensor
    Wire.requestFrom(MPU,14,true); // Lê 14 bytes  
    //Armazena o valor dos sensores nas variaveis correspondentes
    AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
    AcX_norm=AcX*sensitityRange*gravity;//Essa conta da valores em m/s^2     
    AcY_norm=AcY*sensitityRange*gravity;//Essa conta da valores em m/s^2
    AcZ_norm=AcZ*sensitityRange*gravity;//Essa conta da valores em m/s^2
    Tmp_norm=Tmp/340.00+36.53;
    GyX_norm=GyX*sensitityRange*gravity;
    GyY_norm=GyY*sensitityRange*gravity;
    GyZ_norm=GyZ*sensitityRange*gravity;
     
    //Envia valor X do acelerometro para a serial.
    UART.print("AcX = "); UART.print(AcX_norm);
    
    //Envia valor Y do acelerometro para a serial.
    UART.print(" | AcY = "); UART.print(AcY_norm);
   	
    //Envia valor Z do acelerometro para a serial.
    UART.print(" | AcZ = "); UART.print(AcZ_norm);
    
    //Envia valor da temperatura para a serial.
    //Calcula a temperatura em graus Celsius
    UART.print(" | Tmp = "); UART.print(Tmp_norm);
    //Envia valor X do giroscopio para a serial.
    UART.print(" | GyX = "); UART.print(GyX_norm);
    
    //Envia valor Y do giroscopio para a serial.  
    UART.print(" | GyY = "); UART.print(GyY_norm);
     
    //Envia valor Z do giroscopio para a serial.
    UART.print(" | GyZ = "); UART.println(GyZ_norm);
     
    //Aguarda 300 ms e reinicia o processo
    delay(300);
}

/*
REFERENCIAS

I2C: https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/

















*/