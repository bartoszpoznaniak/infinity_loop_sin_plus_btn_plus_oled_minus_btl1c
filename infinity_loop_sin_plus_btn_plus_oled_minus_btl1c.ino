#include <TimerOne.h>
#include <Button.h>

#include <Bounce2.h>

Bounce2::Button button = Bounce2::Button();

#define BUTTON_PIN 3


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

const float pi = 3.14159 ;
const float T = 100 ;    // sample time in microseconds
const float freq = 50 ;  // frequency of tone in hertz
const float omega = 2*pi*freq ;
const float A = 490 ;  // amplitude
// next line initializes oscillation with amplitude A
float a[]={0.0, A*sin(omega*T/1000000.0),0.0};
// c1 is the difference equation coefficient
const float c1 = (8.0 - 2.0*pow(omega*T/1000000.0,2))/(4.0+pow(omega*T/1000000.0,2));
bool flag=false;
bool flag_count=false;
int count=0;
long c_1=0;
#define outPin  10
const int thermistor2_output = A1;

float ntc_Read(int thermistor_output)
{
  int thermistor_adc_val;
  double output_voltage, thermistor_resistance, therm_res_ln, temperature; 
  thermistor_adc_val = analogRead(thermistor_output);
  output_voltage = ( (thermistor_adc_val * 5.0) / 1023.0 );
  thermistor_resistance = ( ( 5 * ( 10.0 / output_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor_resistance = thermistor_resistance * 1000 ; /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);
  /*  Steinhart-Hart Thermistor Equation: */
  /*  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)   */
  /*  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
  return temperature = temperature - 273.15; /* Temperature in degree Celsius */
}
void compute()
{
  // c_1++;
  // if (c_1 % 100000 == 0) {
  //   Serial.print("a[0]: "); Serial.println(a[0]);
  //   Serial.print("a[1]: "); Serial.println(a[1]);
  //   Serial.print("a[2]: "); Serial.println(a[2]);
  // }
 // digitalWrite(delayPin, HIGH);
  a[2] = c1*a[1] - a[0] ;  // recursion equation
  a[0] = a[1] ;
  a[1] = a[2] ;
  Timer1.setPwmDuty(outPin, int(a[2])+512);
 // digitalWrite(delayPin, LOW);
}
void show_value(int str1,int x,int y)
{
 // display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(x, y);
 // display.print("Temperature: ");
  display.print(str1);
  display.display();
}
void show_value_c(int str1,int x,int y)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(x, y);
 // display.print("Temperature: ");
  display.print(str1);
  display.display();
}
void show_value(String str1,int x,int y)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(x, y);
 // display.print("Temperature: ");
  display.print(str1);
  display.display();
}
void show_value(float str1,int x,int y)
{
  //display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(x, y);
 // display.print("Temperature: ");
  display.print(str1);
  display.display();
}
//Button button1(3);
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Timer1.initialize(T);  // set sample time for discrete tone signal
  Timer1.pwm(outPin, 0,T);  //generates pwm signal on outPin(9), with 0 duty cycle and sample period of T=100us
  Timer1.detachInterrupt();
  //button1.begin();
  button.attach( BUTTON_PIN, INPUT_PULLUP );
  button.interval(50); 
  pinMode(7, OUTPUT);//BTL

   // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
  //Timer1.attachInterrupt(compute);
  Serial.println("Program NIeskonczinej pętli to testów");
  show_value("START",0,7);

}

void loop() {
  //Serial.print("T=");
  
  //ntc_Read(thermistor2_output)
  for(int i=0; i<1000;i++) {
     button.update();
    if ( button.pressed() ) {
    flag  =  !flag;
	  Serial.print("btn:");Serial.println(flag);
    //Serial.println("Button 1 pressed");
    
    }
  // if (button1.pressed())
  //  {
  //   flag  =  !flag;
	//   Serial.print("btn:");Serial.println(flag);
  //   //Serial.println("Button 1 pressed");
  //  }
	// if (button1.released())
  //  {

  //  }
	   if (flag) {
        Timer1.attachInterrupt(compute);
       // digitalWrite(7, HIGH);
        //digitalWrite(7, LOW);
         digitalWrite(7, HIGH);
        flag_count=true;
     }

	   else {
      Timer1.detachInterrupt();
      //digitalWrite(7, LOW);
      digitalWrite(7, HIGH);
      flag_count=false;
     }

   
     delay(1);
   }
     if (flag_count){
      count++;
     }
     else
     {
      count=0;
     }
show_value("Tmp:",0,20);
show_value(ntc_Read(thermistor2_output),30,20);
//Serial.println(count);
show_value(count,30,50);
if (count>400) 
 {
  flag  =  !flag;
  count=0;
 }

 if (ntc_Read(thermistor2_output)>80) {
  //delay(1000);
  ///show_value_c("Wartość temperatury",0,7);
  flag=false;
  Timer1.detachInterrupt();
  show_value("Przekroczona TMP!!!",0,27);
  digitalWrite(7, LOW);
  delay(5000);
  //Timer1.detachInterrupt();
  //show_value("Poczatek!",0,7);
  
 }
 else{
 // delay(1000);
 }
 // Serial.print("minuta mineła");
 // Serial.print("1 sek");

 // Serial.println(ntc_Read(thermistor2_output));
     
 // put your main code here, to run repeatedly:


}
