/*
    Nov 2014
    Simple code for a remote controlled tank platformer
    Hardware setup:
    - Arduino Uno + Ardumoto + XBee Shield + 6 AA Batteries plugged into Arduino's DC power jack
    - Tamiya Tank Track + Tamiya Gearboxes + 5V Motors replacing original 3V motors from Tamiya
    
    Controlled with Logitech Precision Gamepad. See ./traxxjoystick/
*/

// pin layouts
int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13

int val = 0;    
int cvroom_a = 0;                   // holder variable for motor a speed
int cvroom_b = 0;
int cvroom_speed = 10;              // general motor speed
int max_cvroom_a = 4*cvroom_speed;  // max speed for motor a
int max_cvroom_b = 4*cvroom_speed;
int looping = 0;
boolean accel = false;              // true if direction buttons are pressed on the joystick, false if released

void setup() {
  Serial.begin(9600);
  Serial.setTimeout( 30 );
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
}

void loop()
{ 
  if( Serial.available() > 0 ) {
    int msg = Serial.parseInt();

    switch( msg ){
      case 1:
        north();
      break;
        
      case 2 :
        northeast();
      break;
      
      case 3 :
        east();
      break;
      
      case 4 :
        southeast();
      break;
      
      case 5 :
        south();
      break;
      
      case 6 :
        southwest();
      break;
      
      case 7 :
        west();
      break;
      
      case 8 :
        northwest();
      break;
      
      case 9 :
        release();
      break;
      
      case 10 :
        cvroom_speed++;
        if( cvroom_speed > 50 )
          cvroom_speed = 50;
      break;
      
      case 11:
        cvroom_speed--;
        if( cvroom_speed < 0 )
          cvroom_speed = 0;
      break;
    }
  } // end Serial.available > 0
  vroom();
}



void north() 
{ 
  max_cvroom_a = 4*cvroom_speed;
  max_cvroom_b = 4*cvroom_speed;
  accel = true;
  digitalWrite(dir_a, HIGH);  
  digitalWrite(dir_b, HIGH);  
}

void south() 
{
  max_cvroom_a = 4*cvroom_speed;
  max_cvroom_b = 4*cvroom_speed;
  accel = true;
  digitalWrite(dir_a, LOW);  
  digitalWrite(dir_b, LOW);  
}

void east() {
   max_cvroom_a = 4*cvroom_speed;
   max_cvroom_b = 4*cvroom_speed;
   accel = true;
   digitalWrite( dir_a, LOW );
  digitalWrite( dir_b, HIGH ); 
}

void northeast() {
  max_cvroom_a = 1*cvroom_speed;
  max_cvroom_b = 4*cvroom_speed;
  accel = true;
  digitalWrite( dir_a, HIGH );
  digitalWrite( dir_b, HIGH );
} 

void southeast() {
  max_cvroom_a = 1*cvroom_speed;
  max_cvroom_b = 4*cvroom_speed;
  accel = true;
  digitalWrite( dir_a, LOW );
  digitalWrite( dir_b, LOW );
}

void west(){
  max_cvroom_a = 4*cvroom_speed;
  max_cvroom_b = 4*cvroom_speed;
  accel = true;
  digitalWrite( dir_a, HIGH );
  digitalWrite( dir_b, LOW ); 
}

void northwest() {
  max_cvroom_a = 4*cvroom_speed;
  max_cvroom_b = 1*cvroom_speed;
  accel = true;
  digitalWrite( dir_a, HIGH );
  digitalWrite( dir_b, HIGH );
}

void southwest() {
  max_cvroom_a = 4*cvroom_speed;
  max_cvroom_b = 1*cvroom_speed;
  accel = true;
  digitalWrite( dir_a, LOW );
  digitalWrite( dir_b, LOW );
}

void release() {
  accel = false;
}


void vroom() {
  if( accel ) {
    do_acdeccel( 1 );
  } else {
    do_acdeccel( -1 );
  }
  
  digitalWrite( pwm_a, cvroom_a );
  digitalWrite( pwm_b, cvroom_b ); 
  delay( 20 );
}

void do_acdeccel( int acdec ) {
// acdec is either +1 ( accel ) or -1 ( deccel )
  cvroom_a += ( acdec * 10 );
  cvroom_b += ( acdec * 10 );
  if( cvroom_a > max_cvroom_a ) {
    cvroom_a = max_cvroom_a;
  }
  if( cvroom_a < 0 ) {
    cvroom_a = 0;
  }
  if( cvroom_b > max_cvroom_b ) {
    cvroom_b = max_cvroom_b;
  }
  if( cvroom_b < 0 ) {
    cvroom_b = 0;
  }
}

// NOT USED

void forward() //full speed forward
{ 
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high  
  analogWrite(pwm_a, 255);    //set both motors to run at (100/255 = 39)% duty cycle
  analogWrite(pwm_b, 255);
}

void backward() //full speed backward
{
  digitalWrite(dir_a, LOW);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, LOW);  //Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, 255);   //set both motors to run at 100% duty cycle (fast)
  analogWrite(pwm_b, 255);
}

void stopped() //stop
{ 
  digitalWrite(dir_a, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, LOW); //Set motor direction, 3 high, 4 low
  analogWrite(pwm_a, 0);    //set both motors to run at 100% duty cycle (fast)
  analogWrite(pwm_b, 0); 
}

void fadein()
{ 
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) 
  { 
     // sets the value (range from 0 to 255):
    analogWrite(pwm_a, fadeValue);   
    analogWrite(pwm_b, fadeValue);    
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
}

void fadeout()
{ 
  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) 
  { 
    // sets the value (range from 0 to 255):
    analogWrite(pwm_a, fadeValue);
    analogWrite(pwm_b, fadeValue);
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);  
}
}

void astop()                   //stop motor A
{
  analogWrite(pwm_a, 0);    //set both motors to run at 100% duty cycle (fast)
}

void bstop()                   //stop motor B
{ 
  analogWrite(pwm_b, 0);    //set both motors to run at 100% duty cycle (fast)
}



