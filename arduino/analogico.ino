#include <ESP8266WiFi.h> // Conexion del NodeMCU a la red Wifi Local
#include <WiFiClient.h>   // Configurar NodeMCU cliente
#include <ESP8266HTTPClient.h> //Funciones enviar request POST o GET  Cliente
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <FirebaseESP8266.h>
#endif

#define FIREBASE_HOST "sensores-87115-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "cW5Hrzn6EzOnpGRr4wJpcIzwN61iaEGKz6VDliTv"

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "IBERO"
#define WIFI_PASSWORD ""

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "YOUR API KEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "sensores-87115-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "YOUR EMAIL"
#define USER_PASSWORD "YOUR PASSWORD"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//Sensor Analogo
int sensorValue1 = 0;
//int sensorValue2 = 0;
int sensorValue3 = 0;

unsigned int sensorValue2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Inicializamos el serial
  delay(10); // Retraso esperamos se inicialize el Serial

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code hMere, to run repeatedly:
  sensorValue2 = analogRead(A0);

  Serial.println(sensorValue2);
  delay(1500);

  Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/potenciometro/analogico"), sensorValue2) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Set float... %s\n", Firebase.getFloat(fbdo, F("/potenciometro/analogico"))? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

  if (Firebase.ready() && (millis() - sensorValue2 >= 0 || sensorValue2 <= 1024))
  {
    sensorValue2 = analogRead(A0);
  }
}
