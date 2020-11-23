#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "DHT.h"

//isi untuk jaringan hotsopt
const char* ssid = "xxxxxxx";  //masukan nama WiFi
const char* pass = "xxxxxxx";   //paswwor wifi

//bagian kode thingspeak
unsigned long myChannelNumber = xxxxx;  //chanel ID
const char * myWriteAPIKey = "xxxxxx";  //Wtite
WiFiClient client;

//sensor DHT di pin gpio2 dan tipe dht11
DHT dht(2, DHT11); 
float t, h;

void koneksi()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("\nMenyambungkan ke hotspot: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nBerhasil terhubung.");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  koneksi();
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  //suhu dan kelembaban
  t = dht.readTemperature();
  h = dht.readHumidity();

  //cek kondisi sensor
  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor Error/kabel data tidak terhubung!");
    return;
  }

  //perintah untuk data ke field thingspeak
  ThingSpeak.setField(1, String(t,2));
  ThingSpeak.setField(2, String(h,2));

  //data di serial Monitor
  Serial.print("Suhu: ");
  Serial.print(t,2);
  Serial.print(" °C | Humd: ");
  Serial.print(h,2);
  Serial.print(" % ");

  // update data ke channel thingspeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200)
  {
    Serial.println("Data terkirim.");
  }
  else
  {
    Serial.println("Gagal mengirim!. Kode HTTP error " + String(x));
  }

  delay(20000);
}
