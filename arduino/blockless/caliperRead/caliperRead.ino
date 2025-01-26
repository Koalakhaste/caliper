#define CLOCK_PIN 9
#define DATA_PIN  8

unsigned long tempmicros;

void setup() 
{
  Serial.begin( 115200 );
  pinMode( CLOCK_PIN, INPUT );
  pinMode( DATA_PIN, INPUT );
  Serial.print( "Ready:" );
}

float decode() {
  int sign = 1;
  long value = 0;
  
  for (int i = 0; i < 23; i++) 
  {
    unsigned char dilbit = 0;
    //wait until clock returns to HIGH - the first bit is not needed
    while ( digitalRead( CLOCK_PIN ) == LOW ) {}
    
    //wait until clock returns to LOW
    while ( digitalRead( CLOCK_PIN) == HIGH ) {}

    for(int i = 0;i<5;i++){
       dilbit += digitalRead( DATA_PIN ) ;
    }
    
    if ( dilbit>3) 
    {
      if (i < 20) 
      {
        value |= 1 << i;
      }
      if (i == 20) 
      {
        sign = -1;
      }
    }
  }
  return ( value * sign );// / 100.00;
}
 
void loop () {
  //wait until clock turns to HIGH
  while (digitalRead( CLOCK_PIN) == LOW ) {}

  tempmicros = micros();

  //wait for the end of the HIGH pulse
  while ( digitalRead( CLOCK_PIN ) == HIGH ) {}
  
  //if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence
  if ( ( micros() - tempmicros ) )//> 500 ) 
  { 
    int result = decode();

    //print measurments
    //Serial.print( "Lenght: " );
    Serial.print( millis() );
    Serial.println( result );
    //delay( 1000 );
  }
}