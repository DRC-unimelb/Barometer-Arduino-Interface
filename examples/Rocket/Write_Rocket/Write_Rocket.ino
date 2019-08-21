#include <Wire.h>
#include <EEPROM.h>

#include "SparkFunBME280.h"
BME280 my_sensor;

//ADJUSTABLE HYPERPARAMETERS
float MAX_DURATION = 10.00e3;
int EEPROM_SPACE = 1024;

long TIME_INTERVAL = 0;
long previous_millis = 0;
long start_millis = 0;
long operation_time = 0;
int ee_address = 0;
bool can_loop = true;

void setup()
{
  TIME_INTERVAL = MAX_DURATION / (float)(EEPROM_SPACE / (sizeof(float) * 3));
  Serial.begin(9600);

  while(!Serial);
  
  Serial.print("Reading values from BME280 every "); Serial.print(TIME_INTERVAL); Serial.println(" milliseconds");
  Serial.println();

  my_sensor.setI2CAddress(0x76);
  Wire.begin();

  if (my_sensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
}

void loop()
{
    unsigned long current_millis = millis();
    if (current_millis - start_millis < MAX_DURATION and can_loop == true)
    {
      if (current_millis - previous_millis > TIME_INTERVAL)
      {
        previous_millis = current_millis;
      
        float pres = my_sensor.readFloatPressure();
        float temp = my_sensor.readTempC();
        float alti = my_sensor.readFloatAltitudeFeet();

        if (EEPROM_SPACE - ee_address > 3 * sizeof(float))
        {
          EEPROM.put(ee_address, pres);
          EEPROM.put(ee_address + sizeof(float), temp);
          EEPROM.put(ee_address + (2 * sizeof(float)), alti);
      
          ee_address = ee_address + (3 * sizeof(float));
        }
        else if (EEPROM_SPACE - ee_address > 0)
        {
          EEPROM.put(ee_address + 1, 'E'); 
          Serial.print("Finished writing to EEPROM up to address position "); Serial.println(ee_address);
          can_loop = false;
        }
      }
    }
    else if (can_loop == true)
    {
      EEPROM.put(ee_address + 1, 'E');
      Serial.print("Finished writing to EEPROM up to address position "); Serial.println(ee_address);
      can_loop = false;
    }
}