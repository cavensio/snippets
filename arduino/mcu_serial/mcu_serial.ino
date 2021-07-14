/**
   Serial snippet - read from and write to serial.
   It prints lines with mcu state (uptime, counter and values) every second
   in continuous mode.
   Each data line started with RS control character.
   Other lines (welcome, info, warning messages) don't have leading control character.
   It reads commands with integer parameter from the serial and updates data values.

   Commands:
   - V <integer> - set value
   - X <integer> - set xvalue
   - C - switch to continuous mode to print data lines every second
   - S - switch continuous mode off
   - R - print single line of data

   Usage samples:
   - "V123 X456 R" - set value to 123, xvalue to 456 and print data line
*/

uint32_t counter = 0;
uint32_t value = 0;
uint32_t xvalue = 0;
bool continuous_mode = false;

void setup() {
  Serial.setTimeout(1000);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("mcu_serial");
}

void printDataLine() {
  counter++;
  Serial.print('\x1e');
  Serial.print("time=");
  Serial.print(millis());
  Serial.print(",counter=");
  Serial.print(counter);
  Serial.print(",value=");
  Serial.print(value);
  Serial.print(",xvalue=");
  Serial.print(xvalue);
  Serial.println();
}


void waitForSerialOrTimeout(uint32_t timeoutMillis) {
  for (uint32_t i = 0; i < timeoutMillis && !Serial.available(); i++) {
    delay(1);
  }
}

void loop() {
  if (Serial.available()) {
    bool read = true;
    while (read) {
      char command = Serial.read();
      switch (command) {
        case '\n':
        case '\xff':
          read = false;
          break;
        case 'V':
          value = Serial.parseInt(SKIP_WHITESPACE);
          break;
        case 'X':
          xvalue = Serial.parseInt(SKIP_WHITESPACE);
          break;
        case 'C':
          continuous_mode = true;
          break;
        case 'S':
          continuous_mode = false;
          break;
        case 'R':
          printDataLine();
          break;
        case '\t':
        case ' ':
          break;
        default:
          read = false;
          Serial.print("Unknown command: '");
          Serial.print(command);
          Serial.println("'");
      }
    }
  }

  if (continuous_mode) {
    printDataLine();
    waitForSerialOrTimeout(1000);
  }
}
