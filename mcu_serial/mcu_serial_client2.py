"""
Serial snippet.
It sends command, reads state, decodes to dictionary and prints.
"""
from time import sleep
from random import randint
from serial import Serial

PORT = 'COM3'
RATE = 115200


def decode_response(line) -> dict:
    return {k: int(v) for k, v in [i.split('=') for i in line.split(',')]}


ser = Serial(port=PORT, baudrate=RATE)

line = ser.readline()
if line == b'mcu_serial\r\n':
    print('mcu_serial detected')

i = 0
while True:
    i += 1
    sleep(1)
    ser.write(f'V{i} X{randint(10, 99)}R'.encode('ascii'))
    line = ser.readline()
    if line[0] == 30:
        print(decode_response(line[1:].strip().decode('utf-8')))
    else:
        line = line.strip().decode('utf-8')
        print(f'MCU message: {line}')
