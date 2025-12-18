// Defina o pino onde o positivo (+) do buzzer está conectado
const int pinoBuzzer = 26; // Verifique se conectou no D4 (GPIO 4)

void setup() {
  // Configura o pino como saída
  pinMode(pinoBuzzer, OUTPUT);
  Serial.begin(115200);
  Serial.println("Iniciando teste do Buzzer...");
}

void loop() {
  Serial.println("Tocando som...");
  
  // Gera uma frequência de 1000Hz (1kHz) por 500ms
  // Sintaxe: tone(pino, frequência, duração);
  tone(pinoBuzzer, 1000); 
  delay(1000); // Espera 1 segundo com o som tocando (ou até o tempo do tone acabar)

  Serial.println("Silêncio...");
  
  // Para o som
  noTone(pinoBuzzer);
  delay(1000); // Espera 1 segundo em silêncio
}