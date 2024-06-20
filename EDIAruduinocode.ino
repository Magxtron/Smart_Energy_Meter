
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorIn = A0;
const int sensorIn2 = A1;
int mVperAmp = 185; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module

double Voltage = 0;
double V=0,A=0;
double ARMS = 0;
double AmpsRMS = 0;
double Pow = 000.00;
float energy = 0.0;     // cumulative energy in kWh
unsigned long lastMillis = 0;
float bill_amount = 0;

int sensorValue1 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;

void setup(){ 
      lcd.init();         // initialize the lcd
      lcd.backlight();    // Turn on the LCD screen backlight
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      Serial.begin(115200);
      delay(10);
      Serial.println(F("Init...."));
}

void loop(){
 A = getAmp();
 ARMS = (A/2.0) *0.707; // sq root
 AmpsRMS = (ARMS * 1000)/mVperAmp;
 V = getVeff();
 float Wattage = (V*AmpsRMS)-18; //Observed 18-20 Watt when no load was connected, so substracting offset value to get real consumption.
 Pow=AmpsRMS*V; 




  
 if(AmpsRMS < 0.34 ){
 Serial.print(0.00);
 //Serial.println(" Amps RMS ");
 Serial.print("||");
 Serial.print(0.00,4);
 //Serial.println(" Voltage ");
 Serial.print("||");
 Pow=0;
 Serial.print(Pow,4);
 Serial.print(",");
 //Serial.println(" Power ");
 Serial.print(0.00,4);
 
  unsigned long nowMillis = millis();
  energy = energy + (Pow * (nowMillis - lastMillis) / 3600000.0);
  lastMillis = nowMillis; 

  bill_amount = energy ;
 Serial.print(",");
 Serial.print(energy,4);  
 //Serial.println(" Energy ");
 Serial.print(",");
 Serial.print(bill_amount *0.008,5);
  
 //Serial.println(" bill_amount ");
 //Serial.println("--------------"); 
 Serial.println();
  lcd.setCursor(0, 0);
  lcd.print("Units=");
  lcd.print(energy,4);
  lcd.setCursor(0, 1);
  lcd.print("Amount=");
  lcd.print(bill_amount *0.008,5);




}
 else{
 Serial.print(AmpsRMS);
// Serial.println(" Amps RMS ");
 Serial.print("||");
 Serial.print(V);
Pow=AmpsRMS*V;
if(Pow>=100){
 Serial.print("||");
 Serial.print(Pow); 
 }
 else{
 Serial.print("||");
 Serial.print(Pow,3); 
 }
 if(Wattage>=100){
  Serial.print(",");
  Serial.print(Wattage);
 }
 else{
  Serial.print(",");
  Serial.print(Wattage,3);
 }
 

  unsigned long nowMillis = millis();
  energy = energy + (Pow * (nowMillis - lastMillis) / 3600000.0);
  lastMillis = nowMillis; 

 bill_amount = energy ;
 Serial.print(",");
 Serial.print(energy,4); 
 //Serial.println(" Energy ");
 Serial.print(",");
 Serial.print(bill_amount *0.008,5);

 //Serial.println(" bill_amount ");
 //Serial.println("--------------");
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print("Units=");
  lcd.print(energy,4);
  lcd.setCursor(0, 1);
  lcd.print("Amount=");
  lcd.print(bill_amount *0.008,5);
 

 }

}

double getVeff()
{
    double sensorValue1 = 0;
    int crosscount = 0;
    int climb_flag = 0;
    int val[100];
    int max_v = 0;
    double VmaxD = 0;
    double VeffD = 0;
    double Veff = 0;
  for ( int i = 0; i < 100; i++ ) {
    sensorValue1 = analogRead(sensorIn2);
    if (analogRead(sensorIn2) > 511) {
      val[i] = sensorValue1;
    }
    else {
      val[i] = 0;
    }
    delay(1);
  }

  max_v = 0;

  for ( int i = 0; i < 100; i++ )
  {
    if ( val[i] > max_v )
    {
      max_v = val[i];
    }
    val[i] = 0;
  }
  if (max_v != 0) {
    VmaxD = max_v;
    VeffD = VmaxD / sqrt(2);
    Veff = (((VeffD - 420.76) / -90.24) * -210.2) + 210.2;
  }
  else {
    Veff = 0;
  }

  VmaxD = 0;
  delay(100);
  return Veff;
 }

float getAmp()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();

   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           //record the maximum sensor value/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           //record the maximum sensor value/
           minValue = readValue;
       }

    }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5)/1024.0;
      
   return result;

   // voltage

   delay(5000);
 }
