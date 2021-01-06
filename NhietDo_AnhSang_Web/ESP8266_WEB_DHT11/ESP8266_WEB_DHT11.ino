
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHTesp.h>
#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

#define DHTPIN1 D3
DHTesp dht;

#define Quat D1
#define Led D2
#define SW1 D5
#define SW2 D6
#define SW3 D7
#define AS A0

#define FIREBASE_HOST "camtu-a8a62-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Qwjgqa2AjbSkse9SILdOTwsU7vGhLGi8KT5RWIUd"
#define WIFI_SSID "KILA 1"
#define WIFI_PASSWORD "Kila@888888"

int Mode;
String Modee="";
String data1, data2;
String LedStatus,FanStatus;
String myStr ="";
int a,b,c,d,e,f;
int t,as;
void setup() 
{
  Serial.begin(9600);
  dht.setup(0, DHTesp::DHT11); 
  pinMode(Quat,OUTPUT);
  pinMode(Led,OUTPUT);
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  pinMode(AS,INPUT);
  digitalWrite(Led,0);
  digitalWrite(Quat,0);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(10);
  Mode = 0; // thủ công
  Firebase.setString("/project6/Led","0");
  delay(10); 
  Firebase.setString("/project6/Quat","0");
  delay(10); 
}

void readSensor()
{
  delay(200);
  t = dht.getTemperature() - 2;
  delay(700);
  Serial.println("1");
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  else 
  {
      Firebase.setString("/project6/Nhietdo",String(t));
      delay(20);
  }
  Serial.println("2");
  as = map(analogRead(AS),0,1024,100,0);
  Serial.println("3");
  Firebase.setString("/project6/Anhsang",String(as));
  delay(20);
  Serial.println("4");
}

void loop() 
{
  if(Mode==0) // thủ công
  {  
      readSensor(); 
      Serial.println("5");
      data2 = Firebase.getString("/project6/Quat");
      delay(30);
      Serial.println("6");
      data1 = Firebase.getString("/project6/Led");
      delay(350);
      Serial.println("7");
      if(data1.indexOf("1")!=-1) a = 1; else a = 0;
      delay(5);
      Serial.println("8");
      if(data2.indexOf("1")!=-1) b = 1; else b = 0;
      delay(5);
      Serial.println("9");
      if (a==1)
      { 
        digitalWrite(Led,1);
        Serial.println("10");
      }
      if (a==0) 
      {
        digitalWrite(Led,0);
        Serial.println("11");
      }
      if (b==1)
      {
        Serial.println("12");
        digitalWrite(Quat,1);
      }
      if (b==0) 
      {
        digitalWrite(Quat,0);
        Serial.println("13");
      }

      if(digitalRead(SW2)==0)
      {
        delay(30);
        if((digitalRead(SW2)==0)&&(a==1))
        {
          //đang bật thì tắt
          digitalWrite(Led,0);
          Serial.println("14");
          LedStatus = "0";
          Firebase.setString("/project6/Led",LedStatus);
          delay(20);
          Serial.println("15");
          while(digitalRead(SW2)==0)
          {}
        }
        if((digitalRead(SW2)==0)&&(a==0))
        {
          LedStatus = "1";
          digitalWrite(Led,1);
          Serial.println("16");
          Firebase.setString("/project6/Led",LedStatus);
          delay(20);
          Serial.println("17");
          while(digitalRead(SW2)==0)
          {}
        }
      }

      if(digitalRead(SW3)==0)
      {
        delay(30);
        if((digitalRead(SW3)==0)&&(b==1))
        {
          FanStatus = "0";
          digitalWrite(Quat,0);
          Serial.println("18");
          Firebase.setString("/project6/Quat",FanStatus);
          delay(20);
          Serial.println("19");
          while(digitalRead(SW3)==0){}
        }
        if((digitalRead(SW3)==0)&&(b==0))
        {
          FanStatus = "1";
          digitalWrite(Quat,1);
          Serial.println("20");
          Firebase.setString("/project6/Quat",FanStatus);
          delay(30);
          Serial.println("20");
          while(digitalRead(SW3)==0){}
        }
      }
      Chedo();
      delay(50);
      //------------------------------------------------------------------
  }

 if (Mode==1) // Tự động
  {
      readSensor();
      if (as < 30)
      {
        digitalWrite(Led,HIGH);
        Serial.println("21");
        LedStatus = "1";
        Firebase.setString("/project6/Led",LedStatus);
        delay(20); 
        Serial.println("22");   
      }
      if (as > 60) 
      {
        digitalWrite(Led,0);
        LedStatus = "0";  
        Serial.println("23");
        Firebase.setString("/project6/Led",LedStatus);
        delay(20); 
        Serial.println("24");
      }
      
      if (t >= 28)
      {
        digitalWrite(Quat,HIGH);
        FanStatus = "1";
        Serial.println("25");
        Firebase.setString("/project6/Quat",FanStatus);
        delay(20); 
        Serial.println("26");
      }
      if (t < 25)
      {
        digitalWrite(Quat,0); 
        FanStatus = "0";
        Serial.println("27");
        Firebase.setString("/project6/Quat",FanStatus);
        delay(20); 
        Serial.println("28");
      }   
      Chedo();
      Serial.println("xxxxxxxxxxxxxxxxxxxxxxxx");
      delay(50);
      //-------------------------------------------
      
  }
}

void Chedo()
{
    Modee = Firebase.getString("/project6/Chedo"); 
    delay(50);
    Serial.println("29");
    if (Modee.indexOf("1")!= -1) c = 1; else c = 0;
    if (c == 0)
    {
      Mode = 0;
      Serial.println("30");
    }
    else if (c == 1)
    {
      Mode = 1;
      Serial.println("31");
    }
    if (digitalRead(SW1)==0)
    {
      delay(30);
      if(digitalRead(SW1)==0)
      {
        if (c==1)
        {
          Mode = 0;
          Firebase.setString("/project6/Chedo",String(Mode));
          delay(20); 
          Serial.println("31");   
          while((digitalRead(SW1)==0)){}
        }
        if (c==0)
        {
          Mode = 1;
          Serial.println("30");
          Firebase.setString("/project6/Chedo",String(Mode));
          delay(20);
          while((digitalRead(SW1)==0)){}
        }
     }
    }
}
