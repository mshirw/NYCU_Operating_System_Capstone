import serial
import os
import os.path
import sys

s = serial.Serial("/dev/ttyUSB0",baudrate=115200)

# kernel file must be as small as possbile
# UART is a low-speed interface. It’s okay to send your kernel image because it’s quite small. Don’t use it to send large binary files.

def read_line(s):
    received_string = ""
    while True:
        c = s.read().decode()
        if c=="\r":
            continue
        if c=="\n":
            break
        received_string += c
    return received_string

print("send kernel bin data size")
size = os.stat("../kernel8.img").st_size
size_bytes = size.to_bytes(4,"big")
s.write(size_bytes)

#print(s.read_until(b"Start to load the kernel image... \r\n").decode(), end="")
received_size = read_line(s)
print(received_size)

received_content = read_line(s)
print(received_content)
#print("send kernel bin data")
with open("../kernel8.img","rb") as f:
    while size > 0:
        size -= s.write(f.read(1))

print("write kernel bin done!")

received_content = read_line(s)
print(received_content)

received_content = read_line(s)
print(received_content)

received_content = read_line(s)
print(received_content)
