// --- DEFINIÇÃO DOS PINOS ---
// Altere estes números conforme a sua ligação na placa
const int PINO_R = 19; // Pino do Vermelho
const int PINO_G = 18; // Pino do Verde
const int PINO_B = 5;  // Pino do Azul

// --- CONFIGURAÇÃO DO TIPO DE LED ---
// Mude para 'true' se for Anodo Comum (positivo comum)
// Mude para 'false' se for Catodo Comum (negativo comum)
const bool EH_ANODO_COMUM = false; 

void setup() {
  // Configura os pinos como saída
  pinMode(PINO_R, OUTPUT);
  pinMode(PINO_G, OUTPUT);
  pinMode(PINO_B, OUTPUT);

  // Começa com tudo apagado para evitar sustos
  desligarTudo();
}

void loop() {
  // 1. Testa VERMELHO
  ligarCor(true, false, false); 
  delay(1000); // Espera 1 segundo

  // 2. Testa VERDE
  ligarCor(false, true, false);
  delay(1000);

  // 3. Testa AZUL
  ligarCor(false, false, true);
  delay(1000);

  // 4. Testa BRANCO (Todas as cores juntas)
  ligarCor(true, true, true);
  delay(1000);

  // 5. Apaga tudo antes de recomeçar
  desligarTudo();
  delay(1000);
}

// --- FUNÇÕES AUXILIARES ---

// Função inteligente que lida com a lógica Anodo vs Catodo automaticamente
void ligarCor(bool r, bool g, bool b) {
  if (EH_ANODO_COMUM) {
    // No Anodo Comum, LOW liga e HIGH desliga
    digitalWrite(PINO_R, r ? LOW : HIGH);
    digitalWrite(PINO_G, g ? LOW : HIGH);
    digitalWrite(PINO_B, b ? LOW : HIGH);
  } else {
    // No Catodo Comum, HIGH liga e LOW desliga
    digitalWrite(PINO_R, r ? HIGH : LOW);
    digitalWrite(PINO_G, g ? HIGH : LOW);
    digitalWrite(PINO_B, b ? HIGH : LOW);
  }
}

void desligarTudo() {
  ligarCor(false, false, false);
}