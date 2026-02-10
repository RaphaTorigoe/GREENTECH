/*
SKETCH: ESP-01_Master
AUTOR: Raphael Kayky Hata Torigoe
DATA: 16/08/24
*/

// Arquivo de configuração da rede
#include "config.h"

// Sensores
AdafruitIO_Feed *umidadear = io.feed("umidadear");
AdafruitIO_Feed *temperaturaar = io.feed("temperaturaar");
AdafruitIO_Feed *umidadesolo = io.feed("umidadesolo");

// Relés
AdafruitIO_Feed *iluminacao = io.feed("iluminacao");
AdafruitIO_Feed *ventoinha = io.feed("ventoinha");
AdafruitIO_Feed *irrigacao = io.feed("irrigacao");

void configuraMQTT();

void setup() {
  Serial.begin(9600);

  configuraMQTT();
}

void loop() {
  io.run();

  if (Serial.available() > 6) {
    delay(15000);

    String umidadeAr = Serial.readStringUntil('\n'); 
    umidadear->save(umidadeAr);

    String temperaturaAr = Serial.readStringUntil('\n');
    temperaturaar->save(temperaturaAr);

    String umidadeSolo = Serial.readStringUntil('\n');
    umidadesolo->save(umidadeSolo);

    String vldr = Serial.readStringUntil('\n');
    iluminacao->save(vldr);

    String vent = Serial.readStringUntil('\n');
    ventoinha->save(vent);

    String irrig = Serial.readStringUntil('\n');
    irrigacao->save(irrig);
  }
}

void configuraMQTT() {
  Serial.print("Conectando ao Adafruit IO");
  io.connect(); 

  while (io.status() < AIO_CONNECTED) { 
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}
