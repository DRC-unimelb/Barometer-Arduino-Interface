#include <EEPROM.h>

int EEPROM_SPACE = 1024;
bool can_read = true;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  float pres = 0.0;
  float temp = 0.0;
  float alti = 0.0;
  
  for (int i = 0; i < EEPROM_SPACE; i = i + (3 * sizeof(float)))
  {
    if (can_read)
    {
      pres = EEPROM.get(i, pres);
      temp = EEPROM.get(i + sizeof(float), temp);
      alti = EEPROM.get(i + (2 * sizeof(float)), alti);

      char end_char;
      EEPROM.get(i + (3 * sizeof(float)) + 1, end_char);

      Serial.print("Pressure ");Serial.println(pres);
      Serial.print("Temperature ");Serial.println(temp);
      Serial.print("Altitude ");Serial.println(alti);
      Serial.println();
      
      if (end_char == 'E')
      {
        Serial.print("All written EEPROM data has been read. Read up to address position "); Serial.println(i + (3 * sizeof(float)));
        can_read = false;
      }
    }
  }
}
