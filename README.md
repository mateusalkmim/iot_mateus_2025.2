# iot_mateus_2025.2
material de apoio para prova

## 📋 Projeto ESP32 DevKit V1 - Sistema de Controle de Sensores e Atuadores

Este repositório contém código robusto para ESP32 DevKit V1 com integração de múltiplos sensores e atuadores.

### 🔧 Componentes Incluídos

- **DHT11** - Sensor de temperatura e umidade
- **LDR** - Sensor de luminosidade (fotoresistor)
- **Servo Motor** - Controle de posição com modo sweep
- **Botão de Interrupção** - Interrupção externa para controle

### 📁 Arquivos do Projeto

- **`esp32_sensor_control.ino`** - Código principal do Arduino/ESP32
- **`HARDWARE_SETUP.md`** - Guia completo de montagem e configuração do hardware
- **`platformio.ini`** - Configuração para PlatformIO (opcional)

### 🚀 Como Usar

#### Opção 1: Arduino IDE

1. Instale o suporte para ESP32 no Arduino IDE
2. Instale as bibliotecas necessárias (veja `HARDWARE_SETUP.md`)
3. Abra o arquivo `esp32_sensor_control.ino`
4. Selecione a placa "ESP32 Dev Module"
5. Configure o baud rate do Serial Monitor para 115200
6. Faça upload do código

#### Opção 2: PlatformIO

1. Abra o projeto no PlatformIO
2. Execute `pio lib install` para instalar dependências
3. Execute `pio run -t upload` para compilar e fazer upload
4. Execute `pio device monitor` para ver a saída serial

### 📊 Funcionalidades

- ✅ Leitura contínua de temperatura e umidade (DHT11)
- ✅ Monitoramento de nível de luz ambiente (LDR)
- ✅ Controle de servo motor com dois modos:
  - **STATIC**: Servo parado em 0°
  - **SWEEP**: Varredura contínua de 0° a 180°
- ✅ Interrupção externa para alternar modos
- ✅ Tratamento de erros robusto
- ✅ Debouncing de botão implementado em software
- ✅ Código não-bloqueante (sem delay())
- ✅ Saída formatada no Serial Monitor

### 📖 Documentação Completa

Consulte o arquivo **`HARDWARE_SETUP.md`** para:
- Diagrama de ligações detalhado
- Lista completa de materiais
- Instruções de instalação de bibliotecas
- Guia de troubleshooting
- Dicas de personalização

### 🔌 Pinagem Rápida

| Componente | Pino ESP32 |
|-----------|-----------|
| DHT11 Data | GPIO 4 |
| LDR Sensor | GPIO 34 (ADC) |
| Servo Signal | GPIO 25 |
| Button | GPIO 18 |

### 📚 Bibliotecas Necessárias

- DHT sensor library by Adafruit
- Adafruit Unified Sensor
- ESP32Servo by Kevin Harrington

### 🎓 Objetivo Educacional

Este código foi desenvolvido como material de apoio para estudos de IoT, demonstrando:
- Integração de múltiplos sensores
- Controle de atuadores
- Interrupções externas
- Boas práticas de programação embarcada
- Código robusto e bem documentado

---

**Licença:** Projeto educacional - livre para uso acadêmico  
**Ano:** 2025.2
