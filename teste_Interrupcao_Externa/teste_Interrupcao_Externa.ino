// Definição dos pinos
// Interrupção externa


const int pinoBotao = 14; 
const int pinoLED = 2;

// Variável para armazenar o estado (deve ser volatile se alterada na interrupção)
volatile bool interrupcaoOcorreu = false;

// Função da Interrupção (ISR - Interrupt Service Routine)
// O atributo IRAM_ATTR coloca o código na RAM interna para execução mais rápida
void IRAM_ATTR trataInterrupcao() {
  interrupcaoOcorreu = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(pinoLED, OUTPUT);
  
  // Configura o pino do botão como entrada com resistor de Pull-up interno
  pinMode(pinoBotao, INPUT_PULLUP);

  // Configura a interrupção: 
  // pino, função a ser chamada, modo (FALLING = quando o botão é pressionado)
  attachInterrupt(digitalPinToInterrupt(pinoBotao), trataInterrupcao, FALLING);
}

void loop() {
  if (interrupcaoOcorreu) {
    Serial.println("Botão pressionado! Interrupção detectada.");
    
    // Inverte o estado do LED
    digitalWrite(pinoLED, !digitalRead(pinoLED));
    
    // Reseta a flag da interrupção
    interrupcaoOcorreu = false;
  }
}