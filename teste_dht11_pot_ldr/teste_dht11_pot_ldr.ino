#include "DHT.h"

// --- Configurações de Pinos (ESP32) ---
#define PIN_DHT     4       // Pino digital onde está o DHT11
#define PIN_POT     34      // Pino analógico do Potenciômetro
#define PIN_LDR     35      // Pino analógico do LDR

#define TIPO_DHT    DHT11   // Define o modelo do sensor

// Inicializa o sensor DHT
DHT dht(PIN_DHT, TIPO_DHT);

void setup() {
  // Inicializa a Serial com a velocidade recomendada para ESP32
  Serial.begin(115200);
  
  // Configuração dos pinos (opcional, pois AnalogRead configura auto)
  pinMode(PIN_POT, INPUT);
  pinMode(PIN_LDR, INPUT);

  Serial.println("--- Iniciando Sistema de Monitoramento ---");
  dht.begin();
}

void loop() {
  // Aguarda 2 segundos entre leituras (necessário para o DHT11 não travar)
  delay(2000);

  // --- 1. Leitura do DHT11 ---
  float humidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // --- 2. Leitura do Potenciômetro ---
  // O ESP32 tem resolução de 12 bits (vai de 0 a 4095)
  int valorPot = analogRead(PIN_POT);

  // --- 3. Leitura do LDR ---
  // Quanto mais luz, menor a resistência -> valor lido muda (depende da montagem)
  int valorLDR = analogRead(PIN_LDR);

  // --- Exibição no Serial Monitor ---
  
  // Verifica erros no DHT
  if (isnan(humidade) || isnan(temperatura)) {
    Serial.println("ERRO: Falha ao ler o DHT11!");
  } else {
    Serial.print("Temp: ");
    Serial.print(temperatura);
    Serial.print("°C | Umid: ");
    Serial.print(humidade);
    Serial.print("%");
  }

  Serial.print(" | Potenciômetro: ");
  Serial.print(valorPot); // Vai de 0 a 4095

  Serial.print(" | Luz (LDR): ");
  Serial.println(valorLDR); // Vai de 0 a 4095

  Serial.println("------------------------------------------------");
}