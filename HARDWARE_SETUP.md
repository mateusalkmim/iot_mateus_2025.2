# ESP32 DevKit V1 - Hardware Setup Guide

## Componentes / Components

1. **ESP32 DevKit V1**
2. **DHT11** - Sensor de temperatura e umidade / Temperature and humidity sensor
3. **LDR** - Sensor de luz / Light sensor
4. **Servo Motor** (SG90 ou similar / or similar)
5. **Push Button** - Botão para interrupção externa / External interrupt button
6. **Resistores / Resistors**:
   - 10kΩ × 2 (para LDR e DHT11 / for LDR and DHT11)
7. **Breadboard e jumpers / Breadboard and jumpers**

---

## Pinagem / Pin Configuration

| Componente / Component | Pino ESP32 / ESP32 Pin | Descrição / Description |
|------------------------|------------------------|-------------------------|
| DHT11 Data             | GPIO 4                 | Pino de dados / Data pin |
| LDR Sensor             | GPIO 34 (ADC1_CH6)     | Entrada analógica / Analog input |
| Servo Motor Signal     | GPIO 25                | Sinal PWM / PWM signal |
| Interrupt Button       | GPIO 18                | Interrupção externa / External interrupt |

---

## Diagrama de Ligações / Wiring Diagram

### 1. DHT11 Temperature and Humidity Sensor

```
DHT11 Module:
┌─────────────┐
│   DHT11     │
│             │
│  VCC  DATA  GND
│   │    │    │
└───┼────┼────┼───
    │    │    │
    │    │    └─────────────── GND (ESP32)
    │    │
    │    └──────┬─────────────── GPIO 4 (ESP32)
    │           │
    │          [10kΩ] (Pull-up resistor, opcional se módulo já tiver)
    │           │
    └───────────┴─────────────── 3.3V or 5V (ESP32)
```

**Conexões / Connections:**
- VCC → 3.3V ou 5V
- DATA → GPIO 4 (com resistor pull-up de 10kΩ para VCC)
- GND → GND

**Nota:** Alguns módulos DHT11 já incluem o resistor pull-up. Verifique antes de adicionar outro.

---

### 2. LDR Light Sensor

```
Light Sensor (Voltage Divider):

        3.3V
         │
         │
       [LDR] ← Fotoresistor / Photoresistor
         │
         ├───────────── GPIO 34 (ESP32) - Analog Input
         │
      [10kΩ] ← Resistor fixo / Fixed resistor
         │
        GND

```

**Conexões / Connections:**
- Terminal 1 do LDR → 3.3V
- Terminal 2 do LDR → GPIO 34 (ADC1_CH6) E resistor de 10kΩ para GND
- O GPIO 34 lê a tensão do divisor de tensão (0-3.3V = 0-4095 ADC)

**Nota:** O LDR forma um divisor de tensão. Quanto mais luz, menor a resistência do LDR, maior a tensão lida.

---

### 3. Servo Motor

```
Servo Motor (SG90 typical):
┌────────────┐
│   SERVO    │
│            │
│  Brown Red Orange
│   │    │    │
└───┼────┼────┼───
    │    │    │
    │    │    └─────────────── GPIO 25 (ESP32) - PWM Signal
    │    │
    │    └─────────────────────── 5V (External power recommended)
    │                            Ou use VIN do ESP32 se USB fornecer corrente suficiente
    └──────────────────────────── GND (ESP32 + External power common GND)
```

**Conexões / Connections:**
- Fio Marrom/Preto (GND) → GND
- Fio Vermelho (VCC) → 5V (recomendado fonte externa para múltiplos servos)
- Fio Laranja/Amarelo (Signal) → GPIO 25

**Importante:** 
- Para um servo, a alimentação do ESP32 pode ser suficiente
- Para múltiplos servos ou servos de alto torque, use fonte externa de 5V
- **Sempre conecte o GND comum** entre ESP32 e fonte externa

---

### 4. Push Button (External Interrupt)

```
Button with Internal Pull-up:

  GPIO 18 ─────┬───────[Button]────── GND
               │
           (Pull-up interno ativado no código)
           (Internal pull-up enabled in code)

```

**Conexões / Connections:**
- Terminal 1 → GPIO 18
- Terminal 2 → GND
- O código ativa o resistor pull-up interno (INPUT_PULLUP)
- Quando pressionado, GPIO 18 vai para LOW (GND)

**Nota:** Não é necessário resistor externo, o código usa INPUT_PULLUP.

---

## Diagrama Completo / Complete Circuit Diagram

```
                              ESP32 DevKit V1
                         ┌──────────────────────┐
                         │                      │
    DHT11 ──────────────>│ GPIO 4               │
                         │                      │
    LDR (Divider) ──────>│ GPIO 34 (ADC)        │
                         │                      │
    Servo Signal ───────>│ GPIO 25 (PWM)        │
                         │                      │
    Button ─────────────>│ GPIO 18 (Interrupt)  │
                         │                      │
    3.3V ───────────────>│ 3.3V                 │
                         │                      │
    5V (Servo) ─────────>│ VIN or 5V            │
                         │                      │
    GND ────────────────>│ GND ─────────────────┤← Common GND
                         │                      │
                         └──────────────────────┘
```

---

## Lista de Materiais / Bill of Materials

| Qtd | Item | Especificação |
|-----|------|---------------|
| 1 | ESP32 DevKit V1 | Microcontrolador |
| 1 | DHT11 | Sensor temperatura/umidade |
| 1 | LDR | Fotoresistor (~1-10kΩ luz ambiente) |
| 1 | Servo Motor | SG90 ou similar (5V, 180°) |
| 1 | Push Button | Botão táctil momentâneo |
| 2 | Resistor 10kΩ | 1/4W (para LDR e DHT11) |
| 1 | Breadboard | 400 ou 830 pontos |
| - | Jumpers | Macho-macho para conexões |
| 1 | Cabo USB | Para programação e alimentação |
| 1 | Fonte 5V (opcional) | Para alimentar servo separadamente |

---

## Instalação das Bibliotecas / Library Installation

### Arduino IDE

1. Abra Arduino IDE
2. Vá em **Sketch** → **Include Library** → **Manage Libraries**
3. Instale as seguintes bibliotecas:

   - **DHT sensor library** by Adafruit (v1.4.4+)
   - **Adafruit Unified Sensor** by Adafruit (dependência)
   - **ESP32Servo** by Kevin Harrington (v0.13.0+)

### PlatformIO

Adicione ao `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    adafruit/DHT sensor library@^1.4.4
    adafruit/Adafruit Unified Sensor@^1.1.9
    madhephaestus/ESP32Servo@^0.13.0
```

---

## Configuração da Placa / Board Configuration

### Arduino IDE

1. Adicione o suporte para ESP32:
   - **File** → **Preferences** → **Additional Board Manager URLs**
   - Adicione: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

2. Instale a placa ESP32:
   - **Tools** → **Board** → **Boards Manager**
   - Procure por "ESP32" e instale "esp32 by Espressif Systems"

3. Selecione a placa:
   - **Tools** → **Board** → **ESP32 Arduino** → **ESP32 Dev Module**

4. Configure a porta:
   - **Tools** → **Port** → Selecione a porta COM/Serial do ESP32

5. Configurações recomendadas:
   - Upload Speed: 115200
   - Flash Frequency: 80MHz
   - Flash Mode: QIO
   - Flash Size: 4MB
   - Partition Scheme: Default

---

## Upload do Código / Code Upload

1. Conecte o ESP32 ao computador via USB
2. Abra o arquivo `esp32_sensor_control.ino`
3. Verifique se a placa e porta estão corretas
4. Clique em **Upload** (→) ou pressione Ctrl+U
5. Aguarde a compilação e upload
6. Abra o **Serial Monitor** (Ctrl+Shift+M) com baud rate **115200**

---

## Funcionamento / Operation

### Após o Upload:

1. **Monitor Serial** mostrará:
   - Mensagens de inicialização de cada componente
   - Dados dos sensores em formato de tabela
   - Status do servo (posição e modo)

2. **Leitura dos Sensores**:
   - DHT11: temperatura (°C) e umidade (%)
   - LDR: nível de luz (0-4095, onde maior = mais luz)
   - Atualização a cada 2 segundos

3. **Controle do Servo**:
   - **Modo STATIC**: Servo parado em 0°
   - **Modo SWEEP**: Servo varre de 0° a 180° continuamente

4. **Interrupção Externa**:
   - Pressione o botão para alternar entre modos STATIC/SWEEP
   - LED/mensagem indicará a mudança de modo

### Exemplo de Saída no Serial Monitor:

```
====================================
ESP32 DevKit V1 - Sensor Control System
====================================

[INIT] DHT11 sensor initialized
[INIT] LDR sensor initialized
[INIT] Servo motor initialized at 0°
[INIT] External interrupt initialized on GPIO 18

[READY] System initialization complete!
Press the button to toggle servo sweep mode

Temp(°C) | Humid(%) | Light(0-4095) | Servo(°) | Mode
---------|----------|---------------|----------|----------
  25.3   |   62.1   |     1234      |    0°    | STATIC
  25.4   |   62.0   |     1240      |    0°    | STATIC

[INTERRUPT] Button pressed!
[MODE] Servo sweep mode ENABLED
  25.4   |   62.2   |     1235      |   45°    | SWEEP
  25.5   |   62.1   |     1238      |   90°    | SWEEP
```

---

## Troubleshooting / Solução de Problemas

### DHT11 não lê valores (NaN):
- ✓ Verifique conexões VCC, GND e DATA
- ✓ Confirme resistor pull-up de 10kΩ
- ✓ Aguarde 2 segundos após inicialização
- ✓ Teste com outro pino (mude DHT_PIN no código)

### LDR sempre retorna 0 ou 4095:
- ✓ Verifique divisor de tensão (LDR + resistor 10kΩ)
- ✓ Use GPIO 32-39 (ADC1 apenas)
- ✓ Teste cobrir/iluminar o LDR
- ✓ Ajuste valor do resistor se necessário (1kΩ - 100kΩ)

### Servo não se move:
- ✓ Verifique alimentação 5V adequada
- ✓ Confirme conexão do fio de sinal (GPIO 25)
- ✓ GND comum entre ESP32 e servo
- ✓ Pressione botão para ativar modo SWEEP

### Botão não responde:
- ✓ Verifique conexão GPIO 18 para GND
- ✓ Teste com outro pino (mude INTERRUPT_BUTTON no código)
- ✓ Confirme debounce (pode precisar ajustar debounceDelay)

### ESP32 reseta sozinho:
- ✓ Alimentação insuficiente (use fonte externa para servo)
- ✓ Cabo USB de má qualidade
- ✓ Curto-circuito nas conexões

---

## Personalização / Customization

### Alterar Frequência de Leitura dos Sensores:

No código, linha ~45:
```cpp
const long sensorInterval = 2000;  // 2000ms = 2 segundos
```

### Alterar Velocidade do Servo:

No código, linha ~153:
```cpp
const int servoSpeed = 50;  // 50ms entre movimentos
```

### Alterar Pinos:

No código, linhas 20-23:
```cpp
#define DHT_PIN 4
#define LDR_PIN 34
#define SERVO_PIN 25
#define INTERRUPT_BUTTON 18
```

---

## Considerações de Segurança / Safety Considerations

1. **Alimentação:**
   - Não exceda 5V no VIN
   - Use fonte externa para servos múltiplos ou de alto consumo
   - Verifique especificações de corrente da fonte USB

2. **Conexões:**
   - Sempre desligue antes de modificar circuito
   - Confirme GND comum em todas as alimentações
   - Evite curtos-circuitos

3. **Componentes:**
   - DHT11 opera em 3-5.5V
   - ESP32 GPIOs são 3.3V (tolerante a 5V em alguns pinos, verifique datasheet)
   - Servo normalmente requer 5V

---

## Recursos Adicionais / Additional Resources

- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [DHT11 Datasheet](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [ESP32Servo Library GitHub](https://github.com/madhephaestus/ESP32Servo)

---

## Licença / License

Este projeto é fornecido para fins educacionais.
This project is provided for educational purposes.

---

**Autor / Author:** IoT Project 2025.2  
**Data / Date:** 2025
