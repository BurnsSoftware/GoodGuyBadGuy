/* Program: BadGuy
/* Author: Zachary Burns (badguy@zackburns.com)
 * Date: 10/31/2017
 * Purpose: This program will activate a linear actuator to drive a paper 
 * target system that faces the shooter at randomly timed intervals.
 * 
 * Parts Needed:
 * - Arduino Duemilanove, Uno, or Nano (you choose depending on your needs)
 * - A L298N Chip H-Bridge Motor Controller (gives ability to use PWM or analog)
 * 
 * - Linear Actuator or Wiper Motor (you choose depending on your design)
 * 
 * Linear Actuator Motor Information:
 * I've used two IP65 (outdoor) linear actuators to move the weight of 
 * the target and frame. The actuators go forward with a positive voltage 
 * and backward with a negative voltage.  They can move 330 pounds 
 * at 12v/3A max. The stroke length is 2 inches and the maximum speed 
 * of the actuator is 5.7mm (0.22 inches) per second.
 * So to move the actuator the entire length takes about 10 seconds.
 * This is slow, but powerful. This setup unfortunately causes you 
 * to have to build a contraption to flip the BadGuy 90 degrees 
 * and back again.  This requires that you change linear motion to 
 * circular motion - thus increasing the complexity and chances that
 * something will go wrong.
 * 
 * WARNING - There is no sensor to detect that the linear 
 * actuator is at full extension or retraction!  Be careful...your
 * actuator may not have a limit switch inside to prevent it from
 * stopping - mine does.
 * 
 * Windshield Wiper Motor Information:
 * I have also designed this to work with a standard 12v windshield
 * wiper motor if you want to design your BadGuy system to spin your
 * targets that way.  These types of motors have some pretty good
 * torgue - but again not very fast.  You really don't need them to
 * move that fast, as you only need to make a quarter-turn (90 degrees)
 * to show or hide your BadGuy.
 * 
 * In order for the BadGuy to change from visible mode to hidden mode, the 
 * target needs to change 90 degrees.  I really wanted to use a stepper
 * motor or servo motor, but something that could rotationally move that
 * much force (due to the frame/target/wind load/etc) would be expensive.
 * 
 */

#define DEBUG

unsigned long rndinitpin = 0;
unsigned long waittime = 0;

unsigned long movetime = 1000;

int m1on = 10;
int m1pos = 9;
int m1neg = 8;

int m2on = 7;
int m2pos = 6;
int m2neg = 5;

//
bool m1enab = true;
bool m2enab = false;
 
void setup() 
{
  #ifdef DEBUG
     Serial.begin(9600);
  #endif
  //Initialize Psudeo-Random Number Generator
  randomSeed(analogRead(rndinitpin));  //init with noise

  //Initialize Output Pins for Motors
  if (m1enab)
  {
    pinMode(m1on, OUTPUT);
    pinMode(m1pos, OUTPUT);
    pinMode(m1neg, OUTPUT);
  }

  if (m2enab)
  {
    pinMode(m2on, OUTPUT);
    pinMode(m2pos, OUTPUT);
    pinMode(m2neg, OUTPUT);
  }
}

void loop() 
{  
  //get random time between 5 seconds and 12 seconds
  waittime = random(5000,12000);
  
  #ifdef DEBUG
     Serial.println("------- START -------");
  #endif
  
  if (m1enab)
  {
    #ifdef DEBUG
       Serial.println("Setting Target 1 High Clockwise");
    #endif
    m1enab = true;             //Denote we've moved CW, next time we move CCW
    digitalWrite(m1pos,HIGH);  //Set L298N Positive Control Line IN1 High
    digitalWrite(m1pos,LOW);   //Set L298N Negative Control Line IN2 Low
    //The L298N uses the jumper to the left of IN1 as a DC motor 1 enable jumper. 
    //We connect this to an Arduino PWM output for DC motor and speed control.
    analogWrite(m1on, 255);    //Set to as fast as motor will go (from 0 to 255)
    delay(movetime);           //We now wait for motor to finish moving 90 degrees (depends on motor gearing)
  }
  else
  {
    #ifdef DEBUG
       Serial.println("Setting Target 1 Low Counter Clockwise");
    #endif
    m1enab = false;           //Denote we've moved CCW, next time we move CW
    digitalWrite(m1pos,LOW);  //Set L298N Positive Control Line IN1 High
    digitalWrite(m1pos,HIGH); //Set L298N Negative Control Line IN2 Low
    analogWrite(m1on, 255);   //Set to as fast as motor will go (from 0 to 255)
    delay(movetime);           //We now wait for motor to finish moving 90 degrees (depends on motor gearing)
  }


  //Done Moving Motors either showing or hiding the BadGuy
  //Let's wait a random amount of time to enable the shooter
  //to draw his weapon, shoot, and re-holster his weapon
  //We also do this so the system is not predictable
  #ifdef DEBUG
     Serial.print("Waiting ");
     Serial.print(waittime/1000);
     Serial.println(" seconds.");
     Serial.println("-------  END  -------");
  #endif
  
  delay(waittime);
}

