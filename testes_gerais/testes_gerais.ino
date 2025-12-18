#include <DHT.h>
#include <ESP32Servo.h>

// --- Configurações de Pinos ---
#define PIN_DHT 4
#define PIN_LDR 34     // Pinos 34, 35, 36 e 39 são ADC e apenas entrada
#define PIN_SERVO 18
#define PIN_BOTAO 14   // Pino para interrupção externa

// --- Constantes e Objetos ---
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);
Servo meuServo;

// --- Variáveis Globais ---
volatile bool interrupcaoDetectada = false;
unsigned long tempoAnterior = 0;
const long intervaloLeitura = 2000; // Ler sensores a cada 2 segundos
int anguloServo = 0;

// --- Função de Interrupção (ISR) ---
void IRAM_ATTR trataBotao() {
  interrupcaoDetectada = true;
}

void setup() {
  Serial.begin(115200);
  
  // Inicialização de Sensores e Periféricos
  dht.begin();
  meuServo.attach(PIN_SERVO);
  
  // Configuração do Botão com Pull-up interno e Interrupção
  pinMode(PIN_BOTAO, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BOTAO), trataBotao, FALLING);

  Serial.println("Sistema Iniciado!");
}

void loop() {
  unsigned long tempoAtual = millis();

  // 1. Lógica da Interrupção (Resposta Imediata)
  if (interrupcaoDetectada) {
    interrupcaoDetectada = false; // Reseta a flag
    Serial.println(">>> Interrupção: Botão Pressionado!");
    
    // Move o servo como resposta visual imediata
    anguloServo = (anguloServo == 0) ? 180 : 0;
    meuServo.write(anguloServo);
  }

  // 2. Lógica Temporizada (Não bloqueante)
  if (tempoAtual - tempoAnterior >= intervaloLeitura) {
    tempoAnterior = tempoAtual;

    // Leitura DHT11
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Leitura LDR (0 a 4095 no ESP32)
    int ldrValor = analogRead(PIN_LDR);

    // Verificação de Erro no DHT
    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT!");
    } else {
      Serial.printf("Temp: %.1f°C | Umid: %.1f%% | LDR: %d\n", t, h, ldrValor);
    }

    // Exemplo: Controlar servo baseado na luz se não houver interrupção
    // int mapaLuz = map(ldrValor, 0, 4095, 0, 180);
    // meuServo.write(mapaLuz);
  }
}

//interrupcao externa
void IRAM_ATTR minhaISR() {
  // Código rápido de execução
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(14), minhaISR, RISING);
}