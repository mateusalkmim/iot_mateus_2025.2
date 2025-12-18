#define PINO_BOTAO 4  // Escolha o pino GPIO que você usou

void setup() {
  Serial.begin(115200);

  // O segredo está aqui: INPUT_PULLUP
  // Isso mantém o pino em estado ALTO (3.3V) quando o botão NÃO está apertado.
  pinMode(PINO_BOTAO, INPUT_PULLUP);
}

void loop() {
  // Lendo o estado do botão
  int estado = digitalRead(PINO_BOTAO);

  // Como estamos usando Pull-up conectando ao GND:
  // LOW (0) significa APERTADO
  // HIGH (1) significa SOLTO
  
  if (estado == LOW) {
    Serial.println("Botão APERTADO!");
  } else {
    // Serial.println("Botão solto..."); // Comentado para não poluir o monitor
  }
  
  delay(100); // Um pequeno atraso para facilitar a leitura
}