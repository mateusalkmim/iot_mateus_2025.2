# 🚀 Guia Rápido de Início / Quick Start Guide

## ⚡ Setup Rápido / Quick Setup

### 1. Hardware (5 minutos)

```
DHT11:     VCC → 3.3V,  DATA → GPIO 4,   GND → GND
LDR:       Um lado → 3.3V,  Outro → GPIO 34 + Resistor 10kΩ → GND
Servo:     Vermelho → 5V,   Laranja → GPIO 25,   Marrom → GND
Botão:     Um lado → GPIO 18,   Outro → GND
```

### 2. Software (10 minutos)

#### Arduino IDE:
```
1. Arquivo → Preferências → URLs adicionais:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

2. Ferramentas → Placa → Gerenciador → Instalar "esp32"

3. Sketch → Biblioteca → Gerenciar → Instalar:
   - DHT sensor library (Adafruit)
   - Adafruit Unified Sensor
   - ESP32Servo

4. Ferramentas → Placa → ESP32 Dev Module
   Ferramentas → Porta → (Selecione sua porta)

5. Abrir esp32_sensor_control.ino → Upload
```

#### PlatformIO:
```bash
# Clonar repositório
git clone https://github.com/mateusalkmim/iot_mateus_2025.2.git
cd iot_mateus_2025.2

# Upload
pio run -t upload

# Monitor Serial
pio device monitor
```

### 3. Teste (2 minutos)

1. Abra Serial Monitor (115200 baud)
2. Veja dados dos sensores atualizando
3. Pressione o botão → Servo inicia movimento de 0° a 180°
4. Pressione novamente → Servo para em 0°

---

## 📊 Saída Esperada / Expected Output

```
====================================
ESP32 DevKit V1 - Sensor Control System
====================================

[INIT] DHT11 sensor initialized
[INIT] LDR sensor initialized
[INIT] Servo motor initialized at 0°
[INIT] External interrupt initialized on GPIO 18

[READY] System initialization complete!

Temp(°C) | Humid(%) | Light(0-4095) | Servo(°) | Mode
---------|----------|---------------|----------|----------
  24.5   |   58.3   |     2145      |    0°    | STATIC
  24.6   |   58.2   |     2148      |    0°    | STATIC
```

---

## 🔧 Personalização Rápida / Quick Customization

### Mudar Pinos / Change Pins

Edite no início do arquivo .ino:
```cpp
#define DHT_PIN 4              // Mude para seu pino
#define LDR_PIN 34             // Use GPIO 32-39 (ADC1)
#define SERVO_PIN 25           // Qualquer pino PWM
#define INTERRUPT_BUTTON 18    // Qualquer pino digital
```

### Alterar Intervalo de Leitura / Change Reading Interval

```cpp
const long sensorInterval = 2000;  // 2 segundos (2000ms)
```

### Ajustar Velocidade do Servo / Adjust Servo Speed

```cpp
const int servoSpeed = 50;  // 50ms entre atualizações
```

---

## ❗ Problemas Comuns / Common Issues

| Problema | Solução |
|----------|---------|
| DHT11 retorna NaN | Aguarde 2s, verifique conexões, adicione resistor pull-up 10kΩ |
| LDR sempre 0 ou 4095 | Verifique divisor de tensão, use GPIO 32-39 |
| Servo não move | Verifique alimentação 5V, pressione botão para ativar SWEEP |
| Botão não responde | Confirme GPIO 18 → GND, teste outros pinos |
| ESP32 reseta | Use fonte externa para servo, verifique cabo USB |

---

## 📱 Valores de Referência / Reference Values

### DHT11:
- **Temperatura:** 0°C a 50°C (±2°C precisão)
- **Umidade:** 20% a 90% RH (±5% precisão)

### LDR (0-4095):
- **Escuro:** 0-500
- **Ambiente:** 500-2000
- **Luz intensa:** 2000-4095

### Servo:
- **Ângulo:** 0° a 180°
- **Frequência PWM:** 50 Hz
- **Pulso:** 500-2400 μs

---

## 🎯 Checklist de Verificação / Verification Checklist

- [ ] ESP32 conectado via USB
- [ ] Porta COM selecionada corretamente
- [ ] Bibliotecas instaladas (DHT, Sensor Unificado, ESP32Servo)
- [ ] Hardware conectado conforme diagrama
- [ ] GND comum entre todos componentes
- [ ] Servo com alimentação 5V adequada
- [ ] Serial Monitor em 115200 baud
- [ ] Código compilado sem erros
- [ ] Upload concluído com sucesso

---

## 📖 Documentação Completa

- **`HARDWARE_SETUP.md`** - Guia detalhado de hardware com diagramas
- **`esp32_sensor_control.ino`** - Código fonte completo e documentado
- **`README.md`** - Visão geral do projeto

---

## 🆘 Suporte / Support

Problemas com:
- **Hardware:** Consulte `HARDWARE_SETUP.md` seção Troubleshooting
- **Software:** Verifique versões das bibliotecas
- **Upload:** Teste botão BOOT no ESP32 durante upload

---

**Tempo total estimado:** ~20 minutos da caixa ao funcionamento  
**Nível de dificuldade:** Intermediário  
**Conhecimentos necessários:** Arduino básico, eletrônica básica
