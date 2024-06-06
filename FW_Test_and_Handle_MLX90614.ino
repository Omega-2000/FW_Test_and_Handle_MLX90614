#include <Wire.h>
#include <SparkFunMLX90614.h>

String inputString = "";
bool stringComplete = false;

byte oldAddress;
byte newAddress;

void serialEvent();
void confronta_stringhe();
//bool isValidMessage(String message);
//bool isPrintable(char c);

void scan();
void change_i2c_address();
void read_temperature();


//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void serialEvent()
{
  if (Serial.available() > 0)
  {
    char inChar = (char)Serial.read(); // get the new byte:
    inputString += inChar;             // add it to the inputString:
    if (inChar == '\n')
    { // if the incoming character is a newline, set a flag so the main loop cando something about it:
      stringComplete = true;
    }
  }
}

void confronta_stringhe()
{
  stringComplete = false;

  String confronto = String(inputString);
  //Serial.println(confronto);

  if (confronto.equals("0\n") == 1) {
    scan();

  } else if (confronto.equals("1\n") == 1) {
    oldAddress = 0x5A;
    newAddress = 0x5B;
    change_i2c_address();

  } else if (confronto.equals("2\n") == 1) {
    oldAddress = 0x5B;
    newAddress = 0x5A;
    change_i2c_address();

  } else if (confronto.equals("3\n") == 1) {
    oldAddress = 0x5A;
    read_temperature();

  } else if (confronto.equals("4\n") == 1) {
    oldAddress = 0x5B;
    read_temperature();

  } else {
    Serial.println("COMANDO ERRATO:");
    Serial.println(confronto);
    Serial.println(".");
  }

  inputString = ""; // clear the string:
  stringComplete = false;
}

/*bool isValidMessage(String message) {
  for (unsigned int i = 0; i < message.length(); i++) {
    char c = message.charAt(i);
    if (!isPrintable(c)) {
      return false; // Contains special character, not a valid message
    }
  }
  return true; // No special characters found, valid message
}*/


/*bool isPrintable(char c) {
  // Check if character is printable and not a special character
  // ASCII values 32 (space) to 126 (~) are printable characters
  return (c >= 32 && c <= 126);
}*/

void scan() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  //delay(5000); // Wait 5 seconds for next scan

}

void change_i2c_address() {
  IRTherm therm; // Create an IRTherm object to interact with throughout

  therm.begin(oldAddress); // Try using the old address first
  
  byte address;
  if (!therm.readID()) // Try reading the ID registers
  {
    // If the read fails, try re-initializing with the
    // new address. Maybe we've already set it.
    therm.begin(newAddress);
    
    if (therm.readID()) // Read from the ID registers
    { // If the read succeeded, print the ID:
      Serial.println("Communicated with new address.");
      Serial.println("ID: 0x" + 
                     String(therm.getIDH(), HEX) +
                     String(therm.getIDL(), HEX));
    }
    else
    {
      Serial.println("Failed to communicate with either address.");      
    }
  }
  else
  {
    // If the read suceeds, change the address to something
    // new.
    if (!therm.setAddress(newAddress))
    {
      Serial.println("Failed to set new address.");
    }
    else
    {
      Serial.println("Set the address to 0x" + String(newAddress, HEX));
      Serial.println("Cycle power to try it out.");
    }
  }

  Serial.println("\nScollegare e ricollegare il sensore di temperatura, oppure spegnere e riaccendere la scheda prima di verificare il cambio d'indirizzo avvenuto");

}

void read_temperature() {
  IRTherm therm; // Create an IRTherm object to interact with throughout

  therm.begin(oldAddress); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C); // Set the library's units to Celsius

  if (therm.read()) // On success, read() will return 1, on fail 0.
  {
    // Use the object() and ambient() functions to grab the object and ambient
	// temperatures.
	// They'll be floats, calculated out to the unit you set with setUnit().
    Serial.print("Object: " + String(therm.object(), 2));
    Serial.write('°'); // Degree Symbol
    Serial.println("C");
    Serial.print("Ambient: " + String(therm.ambient(), 2));
    Serial.write('°'); // Degree Symbol
    Serial.println("C");
    Serial.println();
  }

}


//  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


void setup() {
  Serial.begin(115200);

  inputString.reserve(200); // reserve 200 bytes for the inputString

  Wire.begin();

  Serial.println("");
  Serial.println("-----------------------------------------------------------------");
  Serial.println("( Rullo superiore = 0x5A - Rullo inferiore = 0x5B )");
  Serial.println("");
  Serial.println("[ 0 ] SCAN");
  Serial.println("[ 1 ] CHANGE I2C ADDRESS FROM 0x5A TO 0x5B");
  Serial.println("[ 2 ] CHANGE I2C ADDRESS FROM 0x5B TO 0x5A");
  Serial.println("[ 3 ] READ TEMPERATURE FROM 0x5A");
  Serial.println("[ 4 ] READ TEMPERATURE FROM 0x5B");
  Serial.println("");
  Serial.println("Inviare in seriale il numero del comando...");
  Serial.println("");
}

void loop() {
  serialEvent();
  if (stringComplete)
  {
    delay(1000);
    confronta_stringhe();
    delay(1000);

    Serial.println("");
    Serial.println("-----------------------------------------------------------------");
    Serial.println("( Rullo superiore = 0x5A - Rullo inferiore = 0x5B )");
    Serial.println("");
    Serial.println("[ 0 ] SCAN");
    Serial.println("[ 1 ] CHANGE I2C ADDRESS FROM 0x5A TO 0x5B");
    Serial.println("[ 2 ] CHANGE I2C ADDRESS FROM 0x5B TO 0x5A");
    Serial.println("[ 3 ] READ TEMPERATURE FROM 0x5A");
    Serial.println("[ 4 ] READ TEMPERATURE FROM 0x5B");
    Serial.println("");
    Serial.println("Inviare in seriale il numero del comando...");
    Serial.println("");

    return;
  }

}
