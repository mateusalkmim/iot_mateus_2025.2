//pinos de entrada
#define PIN_POTENCIOMETRO 36 //VP
#define PIN_DHT 34
#define PIN_LDR 39 //VN
#define PIN_BOTAO 27

//pinos de saída

#define PIN_BUZZER 26
#define PIN_RED 33
#define PIN_GREEN 25
#define PIN_BLUE 32

// Variáveis de Controle
volatile bool sistemaLigado = true; // "volatile" porque será alterada dentro da interrupção
// Variável para debouncing (evitar leituras falsas do botão)
volatile unsigned long ultimoClick = 0; 

// --- FUNÇÃO DE INTERRUPÇÃO (ISR) ---
// Essa função roda IMEDIATAMENTE quando o botão é apertado
void IRAM_ATTR trataBotao() {
  if (millis() - ultimoClick > 200) { // Debounce de 200ms
    sistemaLigado = !sistemaLigado;   // Inverte o estado (Ligado <-> Desligado)
    ultimoClick = millis();
  }
}

void setup() {
  Serial.begin(115200);

  // Configura Saídas
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Configura Botão com Resistor Interno (PULLUP)
  // Assim, botão solto = HIGH (3.3V), botão apertado = LOW (GND)
  pinMode(PIN_BOTAO, INPUT_PULLUP);

  // Configura a Interrupção
  // digitalPinToInterrupt(PIN_BTN) -> Qual pino vigiar
  // trataBotao -> Qual função chamar
  // FALLING -> Quando o sinal CAI (vai de HIGH para LOW, ou seja, ao apertar)
  attachInterrupt(digitalPinToInterrupt(PIN_BOTAO), trataBotao, FALLING);
}

void loop() {
  // 1. SEGURANÇA GERAL (Botão)
  if (!sistemaLigado) {
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    return; // Sai do loop e começa de novo
  }

  // 2. LEITURA DOS SENSORES
  int leitura_potenciometro = analogRead(PIN_POTENCIOMETRO);
  int leitura_LDR = analogRead(PIN_LDR);
  int leituraTempBruta = analogRead(PIN_DHT);
  
  // Conversão aproximada da temperatura (ajuste conforme seu sensor real na hora)
  float temperaturaCelcius = (leituraTempBruta / 4095.0) * 100.0;

  // 3. SEGURANÇA DA TEMPERATURA
  [cite_start]// "Caso a Temperatura seja inferior a 0ºC e superior a 30ºC" [cite: 83]
  if (temperaturaCelcius < 0.0 || temperaturaCelcius > 30.0) {
    // PERIGO
    digitalWrite(PIN_BUZZER, HIGH); [cite_start]// Liga Buzzer [cite: 98]
    
    [cite_start]// Desliga LED por segurança [cite: 122]
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
    
    Serial.println("Perigo! Desligar!"); [cite_start]// Mensagem obrigatória [cite: 83]

  } else {
    // SEGURO (Temperatura entre 0 e 30)
    digitalWrite(PIN_BUZZER, LOW); // Desliga Buzzer

    // 4. VERIFICAÇÃO DE LUZ (LDR)
    // Se tiver muita luz (> 2000), apaga. Se tiver pouca, acende.
    if (leitura_LDR > 2000) {
      [cite_start]// Tem luz -> LED OFF [cite: 121]
      digitalWrite(PIN_RED, LOW);
      digitalWrite(PIN_GREEN, LOW);
      digitalWrite(PIN_BLUE, LOW);
      Serial.println("LED OFF (Ambiente Claro)");
      
    } else {
      // Está escuro -> LED ON (Calcula cor)
      
      // 5. DEFINIÇÃO DA COR (POTENCIÔMETRO)
      if (leitura_potenciometro < 1300) {
        // Quente (Red)
        digitalWrite(PIN_RED, HIGH);
        digitalWrite(PIN_GREEN, LOW);
        digitalWrite(PIN_BLUE, LOW);
      } else if (leitura_potenciometro < 2700) {
        // Neutro (Amarelo = R+G)
        digitalWrite(PIN_RED, HIGH);
        digitalWrite(PIN_GREEN, HIGH);
        digitalWrite(PIN_BLUE, LOW);
      } else {
        // Frio (Blue)
        digitalWrite(PIN_RED, LOW);
        digitalWrite(PIN_GREEN, LOW);
        digitalWrite(PIN_BLUE, HIGH);
      }
      
      // Monitor Serial [cite: 81, 82]
      Serial.print("LED ON | Temp: ");
      Serial.print(temperaturaCelcius);
      Serial.println(" C");
    }
  }
  
  delay(100); // Pequena pausa para não travar o processador
}