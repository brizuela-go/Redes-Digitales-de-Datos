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
#define API_KEY "API HERE"

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
int sensorValue2 = 0;
String estado = "desconocido";

void setup() {
  // put your setup code here, to run once:

  pinMode(5, INPUT);//Inicializamos GPIO5 como input
  
  Serial.begin(115200);

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
  // put your main code here, to run repeatedly:

  sensorValue2 = digitalRead(5);

  if(sensorValue2==HIGH){
    Serial.println("Ruido detectado");
    estado = "Ruido detectado";
  }
  else{
    Serial.println("Silencio");
    estado = "Silencio";
  }

  Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/sonido/estado"), estado)? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Set string... %s\n", Firebase.getString(fbdo, F("/sonido/estado"))? fbdo.to<const char *>() : fbdo.errorReason().c_str());

  delay(500);
}
