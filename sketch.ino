#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 10 // Pino utilizado para controle do led amarelo

const int pin_botao = 18;  // número do pino do botão
int estado_botao = 0;  // variável para ler o estado do botão

const int pin_ldr = 4;  // número do pino do LDR
int threshold = 600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo, OUTPUT);

  // Inicialização das entradas
  pinMode(pin_botao, INPUT); // Initialize the pushbutton pino as an input

  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  estado_botao = digitalRead(pin_botao);
  if (estado_botao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int codigo_resposta_http = http.GET(); // Código do Resultado da Requisição HTTP

    if (codigo_resposta_http>0) {
      Serial.print("Código de resposta do HTTP ");
      Serial.println(codigo_resposta_http);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código do erro: ");
      Serial.println(codigo_resposta_http);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  int ldrstatus=analogRead(pin_ldr);

  if(ldrstatus >= threshold){
    Serial.print("Está escuro, ligar o LED ");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH);

  }else{
    Serial.print("Está claro, desligar o LED ");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, LOW);

  }
}