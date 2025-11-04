#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

// === CONFIG RFID RC522 (/ Arduino Mega) ===
#define SS_PIN 48   // SDA do RC522 → pino 48 (conforme você pediu)
#define RST_PIN 53  // RST do RC522 → pino 53
MFRC522 rfid(SS_PIN, RST_PIN);

// === CONFIGURAÇÕES DE PINOS ===
// Ajustados para evitar conflito com o teclado em A0..A7
const int PIR_PIN = 4;        // movido para A8 (porque teclado usa A0..A3)
const int LED_ARMADO = A8;
const int LED_DESARMADO = A11;
const int LED_TECLADO = A9;
const int BUZZER = 24;         // movido para pino digital 22 (livre no Mega)

// === VARIÁVEIS DE ESTADO ===
bool sistemaAtivo = false;
bool alarmeDisparado = false;
String senhaCorreta = "1245";
String senhaDigitada = "";

// === CONFIGURAÇÃO DO TECLADO 4x4 (A0..A7) ===
const byte LINHAS = 4;
const byte COLUNAS = 4;
char teclas[LINHAS][COLUNAS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pinosLinhas[LINHAS] = { A0, A1, A2, A3 };    // linhas do keypad
byte pinosColunas[COLUNAS] = { A4, A5, A6, A7 };  // colunas do keypad
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

// === UID autorizado (sem espaços, em maiúsculas) ===
const String UID_AUTORIZADO = "55E5CE65";

void setup() {
  Serial.begin(9600);

  // SPI + RC522
  pinMode(SS_PIN, OUTPUT); // boa prática
  SPI.begin();
  rfid.PCD_Init();

  // pinos do sistema
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(LED_ARMADO, OUTPUT);
  pinMode(LED_DESARMADO, OUTPUT);
  pinMode(LED_TECLADO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_TECLADO, LOW);
  digitalWrite(LED_ARMADO, LOW);
  digitalWrite(LED_DESARMADO, HIGH);  // começa desarmado

  Serial.println("=== Sistema de Alarme com Teclado + RFID RC522 (Mega) ===");
  Serial.println("Aproxime o cartão (UID: 55 E5 CE 65) ou digite a senha para ARMAR/DESARMAR.");
}

void loop() {
  // --- TECLADO ---
  char tecla = teclado.getKey();
  if (tecla) {
    Serial.print("Tecla: "); Serial.println(tecla);
    digitalWrite(LED_TECLADO, HIGH);
    delay(100);
    digitalWrite(LED_TECLADO, LOW);

    if (tecla == '#') {
      verificarSenha();
      senhaDigitada = "";
    } else if (tecla == '*') {
      senhaDigitada = "";
      Serial.println("Senha limpa.");
    } else {
      senhaDigitada += tecla;
    }
  }

  // --- RFID ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // monta UID em HEX (sempre 2 dígitos por byte) sem espaços
    String conteudo = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      byte val = rfid.uid.uidByte[i];
      if (val < 0x10) conteudo += "0";
      conteudo += String(val, HEX);
    }
    conteudo.toUpperCase();

    Serial.print("UID detectado (normalizado): ");
    Serial.println(conteudo);

    if (conteudo == UID_AUTORIZADO) {
      Serial.println("Cartão AUTORIZADO! Alternando estado do sistema...");
      sistemaAtivo = !sistemaAtivo;
      atualizarEstado();
    } else {
      Serial.println("Cartão NÃO autorizado! (equivalente a senha incorreta)");
      sinalErroSenha();
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // --- SENSOR PIR ---
  if (sistemaAtivo) {
    int movimento = digitalRead(PIR_PIN);
    if (movimento == HIGH) {
      alarmeDisparado = true;
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_ARMADO, LOW);
      delay(150);
      digitalWrite(LED_ARMADO, HIGH);
      delay(150);
      Serial.println("⚠️ Movimento detectado! Alarme ativo!");
    } else {
      if (!alarmeDisparado) {
        digitalWrite(BUZZER, LOW);
      }
    }
  } else {
    digitalWrite(BUZZER, LOW);
  }
}

void verificarSenha() {
  if (senhaDigitada == senhaCorreta) {
    sistemaAtivo = !sistemaAtivo;
    atualizarEstado();
  } else {
    Serial.println("Senha incorreta!");
    sinalErroSenha();
  }
}

void atualizarEstado() {
  alarmeDisparado = false;
  digitalWrite(BUZZER, LOW);
  if (sistemaAtivo) {
    Serial.println("🔒 Sistema ARMADO!");
    digitalWrite(LED_ARMADO, HIGH);
    digitalWrite(LED_DESARMADO, LOW);
  } else {
    Serial.println("🔓 Sistema DESARMADO!");
    digitalWrite(LED_ARMADO, LOW);
    digitalWrite(LED_DESARMADO, HIGH);
  }
}

void sinalErroSenha() {
  // mesmo comportamento de "senha incorreta" do sketch original:
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}
