#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

int targethumidity;
int sensorPin = A1;
int sum=0;
int memory;
int humidifierCount=0;
int OSerrorCount=0;
int realhumidity;
int realtemperature;
double realpressure;

boolean humidifier=false ;
boolean OPcondition=true;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
  pinMode(sensorPin,INPUT); //Knob
  pinMode(9,OUTPUT); //Humidifier control
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
else
{
  display.clearDisplay();
  display.setTextSize(1);                   
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.println("testing build");
  display.setCursor(35,8);
  display.println("Designed By:");
  display.setCursor(45,16);
  display.println("Sam Lau");
  display.setCursor(100,24);
  display.println("V1.0");
  display.display();
  display.startscrollleft(0x00, 0x00);
  delay(3000);
  display.stopscroll();
  delay(1000);
}

while (!bme.begin(0x76)) {
display.clearDisplay();
display.setTextSize(1);                   
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.println("No Sensor!");
  display.println("Check Wiring!");
  display.display();   
  display.startscrollright(0x00, 0x01);
  delay(2500);
  display.stopscroll();
  display.startscrollleft(0x00, 0x01);
  delay(2500);
  display.stopscroll();     
Serial.println("Could not find a valid BME280 sensor, check wiring!");
}
}


int sethumidity()
{
memory=targethumidity;
for(int i=0;i<30;i++)      
{ 
sum=analogRead(sensorPin)+sum;
delay(1);
  }
int avgValue=sum/30;
sum=0;
targethumidity = 0.089*avgValue+5;
  if(memory!=targethumidity)
  return targethumidity;
}



void loop() {
  
while(OPcondition)
{
realhumidity =bme.readHumidity();
realtemperature = bme.readTemperature();
realpressure= bme.readPressure();
OPcondition=(realhumidity>=0 && realhumidity<=100 && realtemperature>=5 && realtemperature<=50);
display.clearDisplay();
display.setTextSize(1);                   
display.setTextColor(WHITE);
display.setCursor(0,0); 
display.print("Target Humidity:");
int x=sethumidity();
display.print(x);
display.println("%");
display.print("Humidity:");
display.print(realhumidity);
display.println("%");
display.print("Temperature:");
display.print(realtemperature);
display.println("C");
display.print("Pressure:");
display.print(realpressure/1000);
display.println("Kpa");
display.print("Humidifier Count:");
display.println(humidifierCount);
display.print("OS Error Count:");
display.print(OSerrorCount);
//display.print(realhumidity-targethumidity);
//display.println("%");
display.display();

if (realhumidity<x && humidifier==false)
   {
   digitalWrite(9,HIGH);
   delay(300);
   digitalWrite(9,LOW);
   delay(100);
   humidifier=true;
   humidifierCount++;
   }
else if(x<realhumidity && humidifier==true)
 {
   digitalWrite(9,HIGH);
   delay(300);
   digitalWrite(9,LOW);
   delay(100);
   humidifier=false;
   }
else if(realhumidity>85 && humidifier==true)
 {
   digitalWrite(9,HIGH);
   delay(300);
   digitalWrite(9,LOW);
   OSerrorCount++;
   delay(5000);
   humidifier=false;
   }
}
while(!OPcondition){
display.clearDisplay();
display.setTextSize(1);                   
display.setTextColor(WHITE);
display.setCursor(0,0); 
display.println("System Error!");
display.println("Check Sensor!");
display.print("Resetting");
OPcondition=true;
for (int j=0;j<=9;j++)
{
display.print(".");
delay(1000);
}
display.display();
}
}
