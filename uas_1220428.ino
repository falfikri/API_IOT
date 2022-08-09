#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "DHT.h"

#define WIFI_SSID "lah?"
#define WIFI_PASSWORD "lahgataunanyasaya"

#define API_KEY "AIzaSyCH0HkShCRuMSUvtTmv-n5w4x3HoEXD6Ko"
#define FIREBASE_PROJECT_ID "uas12204028"

#define USER_EMAIL "fazli.fikri46@gmail.com"
#define USER_PASSWORD "021102"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

void setup()
{
  
    pinMode(D1, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D5, OUTPUT);
    
    Serial.begin(115200);
    delay(2000);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.print(WiFi.localIP());
    Serial.println();

    dht.begin();

    
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback;
    #if defined(ESP8266)
        fbdo.setBSSLBufferSize(2048, 2048);
    #endif
    fbdo.setResponseSize(2048);
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop(){

  /* controller lampu merah */
  
    if (Firebase.ready()){
        String documentPath = "iot/perintah";
        String field = "lampu_merah";
        Serial.println("Read/Get data from cloud firestore...");
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), field.c_str())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            
            // Create a FirebaseJson object and set content with received payload
            FirebaseJson payload;
            payload.setJsonData(fbdo.payload().c_str());
        
            // Get the data from FirebaseJson object 
            FirebaseJsonData jsonData;
            payload.get(jsonData, "fields/" + field + "/stringValue", true);
            
            if(jsonData.stringValue == "on"){
              digitalWrite(D1, HIGH);
              Serial.println("Lampu Merah ON");
            }else if(jsonData.stringValue == "off"){
              digitalWrite(D1, LOW);
              Serial.println("Lampu Merah OFF");
            }
        }else{
            Serial.println(fbdo.errorReason());
        }
    }

    /* controller lampu kuning */
    
    if (Firebase.ready()){
        String documentPath = "iot/perintah";
        String field = "lampu_kuning";
        Serial.println("Read/Get data from cloud firestore...");
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), field.c_str())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            
            // Create a FirebaseJson object and set content with received payload
            FirebaseJson payload;
            payload.setJsonData(fbdo.payload().c_str());
        
            // Get the data from FirebaseJson object 
            FirebaseJsonData jsonData;
            payload.get(jsonData, "fields/" + field + "/stringValue", true);
            
            if(jsonData.stringValue == "on"){
              digitalWrite(D3, HIGH);
              Serial.println("Lampu Kuning ON");
            }else if(jsonData.stringValue == "off"){
              digitalWrite(D3, LOW);
              Serial.println("Lampu Kuning OFF");
            }
        }else{
            Serial.println(fbdo.errorReason());
        }
    }

    /* controller lampu hijau */

    if (Firebase.ready()){
        String documentPath = "iot/perintah";
        String field = "lampu_hijau";
        Serial.println("Read/Get data from cloud firestore...");
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), field.c_str())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            
            // Create a FirebaseJson object and set content with received payload
            FirebaseJson payload;
            payload.setJsonData(fbdo.payload().c_str());
        
            // Get the data from FirebaseJson object 
            FirebaseJsonData jsonData;
            payload.get(jsonData, "fields/" + field + "/stringValue", true);
            
            if(jsonData.stringValue == "on"){
              digitalWrite(D5, HIGH);
              Serial.println("Lampu Hijau ON");
            }else if(jsonData.stringValue == "off"){
              digitalWrite(D5, LOW);
              Serial.println("Lampu Hijau OFF");
            }
        }else{
            Serial.println(fbdo.errorReason());
        }
    }
    
    /* Sensor Suhu Ruangan */
    
    if (Firebase.ready()){
        float valueSensorDht11 = dht.readTemperature();
        FirebaseJson content;
        String documentPath = "iot/perintah";
        String field = "suhu_ruangan";
        content.set("fields/" + field + "/stringValue", String(valueSensorDht11));
        Serial.print("Write/Push data DHT11 to cloud firestore...");
        if (Firebase.Firestore.patchDocument (&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), field)){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        }else{
          Serial.print(fbdo.errorReason());
        }
    }
}
