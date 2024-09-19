// Librerías necesarias
#include <HTTPClient.h> // Incluye la librería HTTPClient para realizar solicitudes HTTP.
#include <WiFiMulti.h>  // Incluye la librería WiFiMulti para gestionar conexiones WiFi.
#include <WiFiClient.h> 
#include "DHT.h" // Incluye la librería DHT para gestionar el sensor de temperatura y humedad.

// Definición del pin y tipo de sensor DHT
#define DHTPIN 14      // Pin donde está conectado el DHT en ESP32
#define DHTTYPE DHT11 // Puedes cambiarlo por DHT22 si es necesario

DHT dht(DHTPIN, DHTTYPE); // Inicializa el sensor DHT

// Credenciales WiFi
const char* ssid = "Estudiantes";
const char* password = "Ci3nt1f1c0@24";

// Servidor donde enviarás los datos (ajusta esto según tu configuración)
const char* serverName = "zapatazojeje.x10.mx";

void setup() {
  // Inicia el puerto serial
  Serial.begin(115200);

  // Inicia el sensor DHT
  dht.begin();

  // Conecta a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
}

void loop() {
  // Lee los valores del sensor DHT
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  // Verifica si los valores son válidos
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Fallo en la lectura del sensor DHT");
    return;
  }

  // Imprime los valores en el puerto serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C, Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  // Si está conectado al WiFi, envía los datos al servidor
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Formatea los datos en JSON para enviar
    String postData = "temperatura=" + String(temperatura) + "&humedad=" + String(humedad);

    // Realiza la petición POST al servidor
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(postData);

    // Verifica la respuesta del servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la solicitud POST: ");
      Serial.println(httpResponseCode);
    }

    // Finaliza la conexión HTTP
    http.end();
  }

  // Espera 10 segundos antes de volver a leer y enviar los datos
  delay(10000);
}
