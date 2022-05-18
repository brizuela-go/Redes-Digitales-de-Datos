#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <MQ135.h>

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
#define USER_PASSWORD "YOUR PASWORD"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//Sensor Analogo

MQ135 mq135_sensor(A0);
float temperature = 28.0;
float humidity = 33.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Inicializamos el serial
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

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
  delay(10); // Retraso esperamos se inicialize el Serial
}

void loop() {
  // put your main code hMere, to run repeatedly:
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.println("PPM:");
  Serial.print(correctedPPM);
  

  Serial.printf("Set float");
  Firebase.setFloat(fbdo, F("/aire"), correctedPPM);
  //Serial.printf("get float... %s\n", Firebase.getFloat(fbdo, F("/aire")));
  delay(1500);
}
