"""
Serial snippet.
It switch the MCU into continuous mode,
reads data lines from the MCU, decodes to dictionary and prints.
"""
from serial import Serial

PORT = 'COM3'
RATE = 115200


def decode_response(line) -> dict:
    return {k: int(v) for k, v in [i.split('=') for i in line.split(',')]}


ser = Serial(port=PORT, baudrate=RATE)

line = ser.readline()
if line == b'mcu_serial\r\n':
    print('mcu_serial detected')

# Switch to continuous mode
ser.write(b'C')

while True:
    line = ser.readline()
    if line[0] == 30:
        print(decode_response(line[1:].strip().decode('ascii')))
    else:
        line = line.strip().decode('ascii')
        print(f'MCU message: {line}')
