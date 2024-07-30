#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <Blynk.h>

#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <Servo.h>

#include <DHT11.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
const int D4=4;const int D5=5,D3=3,D7=7;
#include <DHT.h>
const int rs=6,en=2,d4=10,d5=9,d6=8,d7=12;
const int rss=15,enn=16,d44=17,d55=18,d66=19,d77=20;//1037
const int d444=1,d555=0,d666=3,d777=7;
#define BLYNK_PRINT Debugserial
SoftwareSerial Debugserial(0,1); //RX,TX
#define BLYNK_TEMPLATE_ID "TMPL3GDn0OoB5"
#define BLYNK_TEMPLATE_NAME "ps2"
#define BLYNK_AUTH_TOKEN "phY6DlEUPbKehBgyDjffVQo8LU72kDsn"
char auth[] = BLYNK_AUTH_TOKEN;
#define r2 A6
#define r3 A7
const int p=11;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
LiquidCrystal lcd2(15,16,17,3,7,13);
//LiquidCrystal_I2C lcd3(0x20, 16, 2);
//LiquidCrystal lcd3(6,14,d444,d555,d666,d777);
DHT dht(D4, DHT11);
#define soil A6     //A0 Soil Moisture Sensor
#define PIR D5      //D5 PIR Motion Sensor
int PIR_ToggleValue;
void checkPhysicalButton();
int relay1State = LOW;
int pushButton1State = HIGH;
#define RELAY_PIN_1       D3   //D3 Relay
#define PUSH_BUTTON_1     D7   //D7 Button
double T, P;
char status;
Servo ser;
int var2;
BLYNK_WRITE(V0){
  var2=param.asInt();
 digitalWrite(18,var2);
}
void setup() {
  Serial.begin(9600);
  pinMode(18,OUTPUT);
  lcd.begin(16,2);
  lcd2.begin(16,2);
  //lcd3.begin();
  //lcd3.backlight();
  pinMode(11,OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);
  //ser.attach(11);
  dht.begin();
  lcd.setCursor(0, 0);
  lcd2.setCursor(0,0);
  lcd.print ("  Initializing...  ");
  lcd2.print("  Initializing...  ");
  //lcd3.print("  Initializing...  ");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("W:OFF,M:off");
  delay(500);
  Debugserial.begin(9600);
  Blynk.begin(Serial,auth);
  //Call the function
  //timer.setInterval(100L, soilMoistureSensor);
  //timer.setInterval(100L, DHT11sensor);
  //timer.setInterval(500L, checkPhysicalButton);
}

void DHT11sensor() {
  Blynk.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.print("Failed to read from DHT sensor!");
    return;
  }
  //Blynk.virtualWrite(V0, t);
  //Blynk.virtualWrite(V1, h);
  lcd.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("T:");
  lcd2.print(t);

  lcd2.setCursor(8, 0);
  lcd2.print("H:");
  lcd2.print(h);

}


//Get the soil moisture values
void soilMoistureSensor() {
  Blynk.run();
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  //Blynk.virtualWrite(V3, value);
  lcd2.setCursor(0,1);
  lcd2.print("S:");
  lcd2.print(value);
  lcd2.print(" ");

}

//Get the PIR sensor values
void PIRsensor() {
  Blynk.run();
  bool value = digitalRead(PIR);
  if (value) {
    //Blynk.logEvent("pirmotion","WARNNG! Motion Detected!"); //Enter your Event Name
    lcd.print("MOTION DETECTED");
    lcd.clear();
    lcd.setCursor(0,0);
    //WidgetLED LED(V5);
    //LED.on();
  } else {
    lcd.print("MOTION NOT");
    lcd.clear();
    lcd.setCursor(0,0);
    //WidgetLED LED(V5);
    //LED.off();
  }  
  }

//BLYNK_WRITE(V6)
//{
 //PIR_ToggleValue = param.asInt();  
//}


//BLYNK_CONNECTED() {
  // Request the latest state from the server
  //Blynk.syncVirtual(VPIN_BUTTON_1);
//}

//BLYNK_WRITE(VPIN_BUTTON_1) {
  //relay1State = param.asInt();
  //digitalWrite(RELAY_PIN_1, relay1State);
//}



void loop() {
    Blynk.run();
    PIR_ToggleValue=digitalRead(PIR);
    int soill=analogRead(soil);
    Blynk.virtualWrite(V2," ");
    Blynk.virtualWrite(V1," ");
    //lcd3.clear();
    //lcd3.setCursor(0,0);
    //digitalWrite(11,LOW);
    while (PIR_ToggleValue == 1 && soill>=50)
    {
    //digitalWrite(11,LOW);
    Blynk.run();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ALERT! MOTION");
    //lcd3.print("Alert! MOTION!");
    delay(300);
    DHT11sensor();
    soilMoistureSensor();
    Blynk.virtualWrite(V1,"ALERT MOTION");
    //lcd2.setCursor(0,0);
    delay(200);
    Blynk.virtualWrite(V1," ");
    PIR_ToggleValue=digitalRead(PIR);
     soill=analogRead(soil);
    soill = map(soill, 0, 1024, 0, 100);
    soill = (soill - 100) * -1;
    }
     lcd.clear();
     lcd.setCursor(0,0);
     //digitalWrite(11,LOW);
    soill = map(soill, 0, 1024, 0, 100);
    soill = (soill - 100) * -1;
    soilMoistureSensor();
    DHT11sensor();
    delay(200);
  
  while(soill<50 && PIR_ToggleValue==0){
    //ser.write(90);
    Blynk.run();
    digitalWrite(p,HIGH);
    delay(300);
    lcd.clear();
    lcd.setCursor(0,0);
    //digitalWrite(11,HIGH);
    soill=analogRead(soil);
    soill = map(soill, 0, 1024, 0, 100);
    soill = (soill - 100) * -1;
    lcd.print("pour water");
    //lcd.print(soill);
    delay(300);
    DHT11sensor();
    Blynk.virtualWrite(V1,"pour water");
    soilMoistureSensor();
    delay(300);
     Blynk.virtualWrite(V1," ");
     PIR_ToggleValue=digitalRead(PIR);
    //digitalWrite(11,0);

  }  //ser.write(90);
     digitalWrite(11,LOW);

    while(soill<50 && PIR_ToggleValue==1){
    //ser.write(90);
    Blynk.run();
    //digitalWrite(p,HIGH);
    delay(300);
    lcd.clear();
    lcd.setCursor(0,0);
    //digitalWrite(11,HIGH);
    soill=analogRead(soil);
    soill = map(soill, 0, 1024, 0, 100);
    soill = (soill - 100) * -1;
    lcd.print("dry+motion");
    //lcd.print(soill);
    delay(300);
    DHT11sensor();
    Blynk.virtualWrite(V1,"pour water");
    Blynk.virtualWrite(V2,"motion also");
    soilMoistureSensor();
    delay(300);
    Blynk.virtualWrite(V1," ");
    Blynk.virtualWrite(V2," ");
      PIR_ToggleValue=digitalRead(PIR);
   // digitalWrite(11,0);
  }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("NORMAL");
     //lcd.setCursor(0,0);
     //lcd2.print("hello");
      DHT11sensor();
     //DHT11sensor();
     soilMoistureSensor();
     delay(200);
     Blynk.run();
      
  //Blynk.run();//Run the Blynk library
  //timer.run();//Run the Blynk timer

  }