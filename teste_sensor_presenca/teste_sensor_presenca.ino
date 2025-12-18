// --- Definições de Pinos ---
#define PIN_PIR 27      // Pino de saída do sensor PIR
#define PIN_LED_ALERTA 2 // LED interno do ESP32

// --- Variáveis de Controle ---
volatile bool movimentoDetectado = false;
unsigned long tempoUltimoMovimento = 0;
const long tempoLigado = 5000; // Mantém o LED aceso por 5 segundos após o movimento

// --- Função de Interrupção (ISR) ---
// Executada instantaneamente quando o sensor detecta movimento
void IRAM_ATTR detectaMovimento() {
  movimentoDetectado = true;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_PIR, INPUT); // A maioria dos PIRs já tem pull-up/down interno
  pinMode(PIN_LED_ALERTA, OUTPUT);

  // Configura interrupção para o pino do PIR
  // RISING: dispara quando o sensor vai de LOW para HIGH (movimento detectado)
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), detectaMovimento, RISING);

  Serial.println("Sensor de Presença Ativo e Calibrando...");
  // Nota: Alguns sensores PIR precisam de 30-60s para estabilizar ao ligar
}

void loop() {
  // Se a interrupção detectou algo
  if (movimentoDetectado) {
    Serial.println(">>> MOVIMENTO DETECTADO!");
    digitalWrite(PIN_LED_ALERTA, HIGH);
    
    // Reseta a flag e atualiza o cronômetro
    movimentoDetectado = false;
    tempoUltimoMovimento = millis();
  }

  // Desliga o LED após o tempo determinado (5 segundos)
  if (digitalRead(PIN_LED_ALERTA) == HIGH) {
    if (millis() - tempoUltimoMovimento >= tempoLigado) {
      Serial.println("Área limpa. Desligando alerta.");
      digitalWrite(PIN_LED_ALERTA, LOW);
    }
  }
}