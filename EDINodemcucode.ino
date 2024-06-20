#include<ESP8266WiFi.h> 
#include<WiFiClient.h> 
#include<ESP8266HTTPClient.h>
String URL="http://api.thingspeak.com/update?api_key=BEKMK6C6HMV8NCLO&field1="; //upload your ThingsSpeak id
//Make sure all setup is connected to same wifi Network
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect(); 
  delay (2000); 
  Serial.print ("Start connection"); 
  WiFi.begin("Hanuman ","jaishreeram");//Name,password
  while ((! (WiFi.status()== WL_CONNECTED))) 
  { 
    delay(200);
    Serial.print("..");
  }
  Serial.println("Connected");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String data=Serial.readStringUntil('\n');
    Serial.println(data);
    int comma=data.indexOf(',');
    Serial.println(comma);
   if(comma!=-1){
//      float a=data.substring(0,comma).toFloat();
//      float v=data.substring(comma+1,comma+7).toFloat();
      float w=data.substring(comma+1,comma+7).toFloat();
//      float p=data.substring(comma+15,comma+21).toFloat();
      float e=data.substring(comma+8,comma+14).toFloat();
      float b=data.substring(comma+15).toFloat();
//      Serial.println(a);
//      Serial.println(v);
      Serial.println(w);
//      Serial.println(p);
      Serial.println(e,4);
      Serial.println(b,5);
      sendData(w,e,b);
    }
  }

}
void sendData(float w, float e, float b){
  WiFiClient client;
  HTTPClient http;
  String newUrl=URL+String(w)+"&field2="+String(e,4)+"&field3="+String(b,5); 
  http.begin(client, newUrl); 
  int responsecode=http.GET(); 
  String data=http.getString(); 
  Serial.println(data); 
  http.end();
  
}
