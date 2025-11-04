# A3-ProjetoArduino-Alarme

Este projeto implementa um **sistema de segurança inteligente** baseado em **Arduino Mega 2560**, que permite armar e desarmar o alarme através de uma **senha numérica** (usando teclado matricial 4x4) ou de um **cartão RFID autorizado** (módulo RC522).  
O sistema também conta com um **sensor PIR** para detecção de movimento, **LEDs indicadores** de estado e um **buzzer** para alarmes sonoros.

## 🧩 Funcionalidades

- ✅ **Armar ou desarmar o sistema** por:
  - Senha digitada no **teclado 4x4**
  - Aproximação de **cartão RFID autorizado**
- 🚨 **Disparo automático do alarme** ao detectar movimento com o sensor PIR enquanto o sistema estiver armado
- 💡 **Indicação visual**:
  - LED vermelho: sistema **armado**
  - LED verde: sistema **desarmado**
  - LED amarelo: indica **atividade no teclado**
- 🔊 **Sinalização sonora**:
  - Alarme ativo em caso de movimento
  - Bip duplo em caso de senha ou cartão incorreto
- 🔁 **Alternância automática de estado** (armado/desarmado) ao usar senha correta ou cartão válido

## 🛠️ Componentes Utilizados

- 1x **Arduino Mega 2560**  
- 1x **Módulo RFID RC522**  
- 1x **Cartão ou chaveiro RFID**  
- 1x **Teclado matricial 4x4**  
- 1x **Sensor PIR (detecção de movimento)**  
- 3x **LEDs** (vermelho, verde e amarelo)  
- 3x **Resistores de 220 Ω** (para os LEDs)  
- 1x **Buzzer piezoelétrico**  
- 1x **Protoboard**  
- Vários **jumpers** (macho-macho e macho-fêmea)  
- 1x **Fonte de alimentação** ou conexão **USB**

## ⚙️ Ligações e Pinos

| Componente | Função | Pinos no Arduino Mega |
|-------------|--------|----------------------|
| **RC522 RFID** | Leitura de cartões | SDA → 48, RST → 53, MOSI → 51, MISO → 50, SCK → 52 |
| **Teclado 4x4** | Entrada de senha | Linhas → A0–A3 / Colunas → A4–A7 |
| **Sensor PIR** | Detecção de movimento | 4 |
| **LED Armado (vermelho)** | Indicação de sistema ativo | A8 |
| **LED Desarmado (verde)** | Indicação de sistema inativo | A11 |
| **LED Teclado (amarelo)** | Indicação de uso do teclado | A9 |
| **Buzzer** | Alerta sonoro | 24 |

## ⚡ Funcionamento do Sistema

1. **Inicialização:**  
   Ao ligar o Arduino, o sistema inicia **desarmado** (LED verde aceso). O monitor serial exibirá uma mensagem com instruções iniciais.

2. **Armar ou desarmar com senha:**  
   - Digite a senha (padrão: `1245`)  
   - Pressione `#` para confirmar  
   - Use `*` para limpar a digitação atual  

3. **Armar ou desarmar com RFID:**  
   - Aproxime um **cartão RFID autorizado** do leitor RC522  
   - O sistema alterna automaticamente entre armado e desarmado  

4. **Alarme ativado:**  
   - Quando o sistema está armado e o **PIR** detecta movimento, o buzzer é acionado e o LED vermelho pisca continuamente  
   - O alarme para apenas quando o sistema é desarmado novamente  

5. **Erro de autenticação:**  
   - Caso a senha ou o cartão não sejam válidos, o buzzer emite **dois bipes curtos** para indicar falha  

## 🔧 Personalização

**Alterar senha:**
  
  String senhaCorreta = "1245";

Modifique o valor entre aspas pela nova senha desejada.

* **Cadastrar novo cartão RFID:**

  const String UID_AUTORIZADO = "55E5CE65";

  Execute o código, aproxime o novo cartão e copie o UID mostrado no monitor serial.
  Substitua o valor no código e envie novamente para o Arduino.

* **Alterar comportamento dos LEDs ou buzzer:**
  Você pode editar facilmente os pinos e tempos de delay no código para adaptar o sistema às suas preferências.

## 🧠 Bibliotecas Necessárias

Certifique-se de instalar as seguintes bibliotecas antes de compilar o projeto:

* [**MFRC522**](https://github.com/miguelbalboa/rfid) – para comunicação com o módulo RFID RC522
* [**Keypad**](https://playground.arduino.cc/Code/Keypad/) – para leitura do teclado matricial
* **SPI** – já incluída por padrão na IDE Arduino

Instale as bibliotecas através da IDE Arduino:
**Sketch → Incluir Biblioteca → Gerenciar Bibliotecas...**

## 💡 Esquema de Ligações (Resumo)

[RC522]     SDA → 48
             RST → 53
             MOSI → 51
             MISO → 50
             SCK  → 52

[Keypad 4x4] Linhas → A0–A3
              Colunas → A4–A7

[PIR]        OUT → 4

[LEDs]       Armado (vermelho) → A8
             Desarmado (verde) → A11
             Teclado (amarelo) → A9

[Buzzer]     Sinal → 24

## 🧰 Ambiente de Desenvolvimento

* **Placa:** Arduino Mega 2560
* **Baud rate:** 9600
* **IDE:** Arduino IDE
* **Linguagem:** C++ (Arduino)

## 📘 Explicação Técnica

O sistema utiliza o **módulo RC522** conectado via **SPI** para leitura de cartões RFID.
O **teclado 4x4** é mapeado pelos pinos analógicos A0 a A7, permitindo digitar a senha de acesso.
O **sensor PIR** é responsável pela detecção de movimento e, quando o sistema está armado, aciona o **buzzer** como alarme sonoro.
Os **LEDs** indicam o estado geral do sistema (armado, desarmado ou digitando senha).
Todo o controle lógico é feito no loop principal, com funções específicas para verificação de senha, leitura de RFID e atualização dos estados visuais e sonoros.

## 🎯 Objetivo do Projeto

Este projeto foi desenvolvido com fins **educacionais e experimentais**, mostrando como integrar sensores e módulos diferentes em um sistema de segurança automatizado.
É ideal para estudantes, hobbystas e entusiastas de eletrônica e automação residencial que desejam aprender sobre controle de acesso e integração de periféricos no Arduino.

## 💬 Créditos e Licença

Projeto criado para aprendizado e uso livre.
Distribuído sob a licença **MIT**, permitindo copiar, modificar e compartilhar.

