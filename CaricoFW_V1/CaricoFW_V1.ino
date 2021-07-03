/****************************
 * SPI PINs for MCP4151 digipot:
 * 11 DI
 * 13 SCK
 * 10 CS
 * 
 * Analog PINs:
 * A0 Current
 * A1 Voltage
 * 
 * Switches:
 * 4 SW1
 * 5 SW2
 * 6 SW3
 * 7 SW4
 */

#include <SPI.h>

#define CS1     10

#define SW1     4
#define SW2     5
#define SW3     6
#define SW4     7

#define I       A0
#define V       A1

void setup() {
  pinMode (SW1, OUTPUT);
  pinMode (SW2, OUTPUT);
  pinMode (SW3, OUTPUT);
  pinMode (SW4, OUTPUT);

  digitalWrite(SW1, LOW);
  digitalWrite(SW2, LOW);
  digitalWrite(SW3, LOW);
  digitalWrite(SW4, LOW);
  
  pinMode (I, INPUT);
  pinMode (V, INPUT);
  
  pinMode (CS1, OUTPUT);
  digitalWrite(CS1, HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.begin();

  Serial.begin(9600);
}

void loop() {
  char input;
  char * command;
  int value = 0;
  
  if(Serial.available())
  {
    input = Serial.read();
    Serial.print("Command: ");
    Serial.println(input);
  }

  switch(input)
  {
    case 'i':
      // Current mode
      Serial.print("Current mode, insert value (0-255): ");
      command = serialRead();
      value = atoi(command);

      digitalWrite(SW1, HIGH);
      digitalWrite(SW2, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, HIGH);

      digiPotWrite(value, CS1);
      
    break;
    
    case 'r':
      // Resistance mode
      Serial.print("Resistance mode, insert value (0-255): ");
      command = serialRead();
      value = atoi(command);
      
      digitalWrite(SW1, LOW);
      digitalWrite(SW2, HIGH);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, HIGH);

      digiPotWrite(value, CS1);
      
    break;

//    case 'v':
//      // Voltage mode
//      Serial.print("Voltage mode, insert value (0-255): ");
//      command = serialRead();
//      value = atoi(command);
//
//      digitalWrite(SW1, HIGH);
//      digitalWrite(SW2, LOW);
//      digitalWrite(SW3, HIGH);
//      digitalWrite(SW4, LOW);
//
//      digiPotWrite(value, CS1);
//      
//    break;

    case 'm':
      // Measure mode
      Serial.println("Measure mode");
      while(Serial.available() == 0)
      {
        int count = 0;
        Serial.print("Value: ");
        Serial.println(analogRead(I));
        while((Serial.available() == 0) && (count < 400))
        {
          count++;
          delay(1);
        }
      }
      
    break;

    case '0':
      // Stop mode
      Serial.print("STOP");

      digitalWrite(SW1, LOW);
      digitalWrite(SW2, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, LOW);

      digiPotWrite(0, CS1);
      
    break;
      
    default:
      delay(1);
    break;
  }
  
}

char * serialRead()
{
  char text[4];         
  while (!Serial.available()) delay (1);
     
  int i = 0;  
  int k = Serial.available();
  
  while (k  > 0)
  {
    char inchar = Serial.read();   
    text[i] = inchar;               
    i++;
    
    if (k < 3) delay (10);    
    k = Serial.available();     
  }
     
  text[i]='\0';        
  Serial.println(text);
  return text;
}

void digiPotWrite(int value, int cs)
{
  digitalWrite(cs,LOW);
  SPI.transfer(0x00);
  SPI.transfer(255-value);
  digitalWrite(cs,HIGH);

  return;
}

