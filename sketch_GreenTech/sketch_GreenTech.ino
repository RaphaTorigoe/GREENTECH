/*
SKETCH: GreenTech
AUTOR: Raphael Kayky Hata Torigoe
DATA: 16/08/24
*/


// Inclui o arquivo config
#include "config.h"

// Biblioteca I2C 
#include <Wire.h> 

// Biblioteca para o display LCD I2C
#include <LiquidCrystal_I2C.h> 

// Biblioteca para o sensor DHT
#include "DHT.h" 


// Define o endereço do LCD
#define enderecoLcd 0x27 

// Define as colunas do LCD
#define colunas 20 

// Define as linhas do LCD
#define linhas 4 

// Define o pino digital onde o sensor LDR está conectado
#define ldr 8 

// Define o pino digital onde o sensor DHT está conectado 
#define DHTPIN 9 

// Define o tipo do sensor DHTT: DHT11 (pode ser DHT22 ou DHT21)
#define DHTTYPE DHT11 

// Define o pino analógico onde o sensor de umidade do solo está conectado 
#define sensorUmidadeSolo A0 

// Define o pino digital para o relé que controla a iluminação
#define iluminacao 2 

// Define o pino digital para o relé que controla a ventoinha
#define ventoinha 3 

// Define o pino digital para o relé que controla a mini bomba de água
#define miniBombaAgua 4 


// Inicializa o LCD 
LiquidCrystal_I2C lcd(enderecoLcd, colunas, linhas);

// Inicializa o sensor DHT
DHT dht (DHTPIN, DHTTYPE); 


// Variável para armazenar a leitura da umidade do ar
int umidadeAr; 

// Variável para armazenar a leitura da temperatura do ar
int temperaturaAr; 

// Variável para armazenar a leitura da luminosidade 0 = Com luz, Led desligado / 1 = Sem luz, Led ligado
int vldr; 

// Variável para armazenar a leitura da umidade do solo
int umidadeSolo; 

// Variável para armazenar a leitura da umidade do solo em porcentagem
int umidadeSoloPercentual; 


// Byte vazio
byte semStatus[8] = { 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// Byte de gota 
byte statusGota[8] = { 
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000
};

// Byte de ventilador
byte statusVent[8] = { 
  B00000,
  B00000,
  B00000,
  B10011,
  B10100,
  B01110,
  B00101,
  B11001
};


void setup() {
    // Inicializa a comunicação serial para monitoramento
    Serial.begin(9600); 

    // Inicia a comunicação com o display
    lcd.init(); 

    // Liga a iluminação do display
    lcd.backlight(); 

    // Limpa o display 
    lcd.clear(); 

    // Cria os bytes
    lcd.createChar(0, semStatus); 
    lcd.createChar(1, statusGota);
    lcd.createChar(2, statusVent);

    // Inicializa o sensor DHT
    dht.begin(); 

    // Configura o pino do sensor de luminosidade como entrada
    pinMode(ldr, INPUT); 

    // Configura o pino do sensor de umidade do solo como entrada
    pinMode(sensorUmidadeSolo, INPUT); 

    // Configura o pino do relé como saída
    pinMode(ventoinha, OUTPUT); 

    // Configura o pino do relé como saída
    pinMode(iluminacao, OUTPUT); 

    // Configura o pino do relé como saída
    pinMode(miniBombaAgua, OUTPUT); 
}


void loop() {
    // Lê a luminosidade do sensor 
    vldr = digitalRead(ldr); 

    // Lê a umidade do ar do sensor DHT
    umidadeAr = dht.readHumidity();  

    // Lê a temperatura do ar do sensor DHT 
    temperaturaAr = dht.readTemperature(); 

    // Lê a umidade do solo do sensor
    umidadeSolo = analogRead(sensorUmidadeSolo); 

    // Mapeia a leitura do sensor (0 a 1024) para uma escala de 100 a 0, o que significa que 0 é 100% de umidade e 1024 é 0% de umidade
    umidadeSoloPercentual = map(umidadeSolo, 0, 1024, 100, 0); 


    if (vldr == 1){ 
      digitalWrite(iluminacao, HIGH);
      lcd.setCursor(7, 3);
      lcd.print("LIG");

    }else if (vldr == 0){ 
      digitalWrite(iluminacao, LOW);
      lcd.setCursor(7, 3);
      lcd.print("DES");
    }


    if (temperaturaAr >= 35){ 
      digitalWrite(ventoinha, HIGH);
      lcd.setCursor(19, 2);
      lcd.write(2);

    }else if (temperaturaAr <= 25){ 
      digitalWrite(ventoinha, LOW);
      lcd.setCursor(19, 2);
      lcd.write(0);
    }


    if (umidadeSoloPercentual <= 60){ 
      digitalWrite(miniBombaAgua, HIGH);
      lcd.setCursor(19, 0);
      lcd.write(1);

    }else if (umidadeSoloPercentual >= 70){ 
      digitalWrite(miniBombaAgua, LOW);
      lcd.setCursor(19, 0);
      lcd.write(0);
    }
    

    // Define a posição do caracter no display LCD (coluna 0, linha 0)
    lcd.setCursor(0, 0); 
    // Exibe caracteres no display
    lcd.print("UMIDADE SOLO:"); 
    lcd.setCursor(13, 0);
    lcd.print(umidadeSoloPercentual);
    lcd.setCursor(15, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("UMIDADE AR:");
    lcd.setCursor(11, 1);
    lcd.print(umidadeAr);
    lcd.setCursor(13, 1);
    lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("TEMPERATURA:");
    lcd.setCursor(12, 2);
    lcd.print(temperaturaAr);
    lcd.setCursor(14, 2);
    lcd.print(char(B11011111));
    lcd.setCursor(15, 2);
    lcd.print("C");
    
    lcd.setCursor(0, 3);
    lcd.print("LED UV:");
   
    
    // Exibe a leitura de umidade do solo no monitor serial
    Serial.print("Leitura umidade solo: ");
    Serial.println(umidadeSolo);

    // Exibe a umidade do solo em porcentagem no monitor serial
    Serial.print("Umidade solo em porcentagem: ");
    Serial.println(umidadeSoloPercentual);

    // Exibe a umidade do ar no monitor serial
    Serial.print("Umidade ar: ");
    Serial.println(umidadeAr);

    // Exibe a temperatura do ar no monitor serial
    Serial.print("Temperatura ar: ");
    Serial.println(temperaturaAr);

    // Exibe a luminosidade no monitor serial
    Serial.print("Iluminação: ");
    Serial.println(vldr);

    Serial.println("--------------------------------");
    delay(2000);
}
