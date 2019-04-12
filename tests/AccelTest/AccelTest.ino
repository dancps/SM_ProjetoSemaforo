//Programa : Teste MPU6050 e LCD 20x4
//Alteracoes e adaptacoes : FILIPEFLOP
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
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
void setup()
{
    UART.begin(115200);
    //Inicializa o LCD
    Wire.begin(SDA_PIN,SCL_PIN); // tb é possível passar um 3o argumento com a I2C Bus Speed
    Wire.beginTransmission(MPU);
    Wire.write(0x6B); 
     
    //Inicializa o MPU-6050
    Wire.write(0); 
    Wire.endTransmission(true);
}

void loop(){
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    //Solicita os dados do sensor
    Wire.requestFrom(MPU,14,true);  
    //Armazena o valor dos sensores nas variaveis correspondentes
    AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
    AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
     
    //Envia valor X do acelerometro para a serial e o LCD
    UART.print("AcX = "); UART.print(AcX);
    
    //Envia valor Y do acelerometro para a serial e o LCD
    UART.print(" | AcY = "); UART.print(AcY);
   	
    //Envia valor Z do acelerometro para a serial e o LCD
    UART.print(" | AcZ = "); UART.print(AcZ);
    
    //Envia valor da temperatura para a serial e o LCD
    //Calcula a temperatura em graus Celsius
    UART.print(" | Tmp = "); UART.print(Tmp/340.00+36.53);
    
    //Envia valor X do giroscopio para a serial e o LCD
    UART.print(" | GyX = "); UART.print(GyX);
    
    //Envia valor Y do giroscopio para a serial e o LCD  
    UART.print(" | GyY = "); UART.print(GyY);
     
    //Envia valor Z do giroscopio para a serial e o LCD
    UART.print(" | GyZ = "); UART.println(GyZ);
     
    //Aguarda 300 ms e reinicia o processo
    delay(300);
}

/*
REFERENCIAS

I2C: https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/

















*/