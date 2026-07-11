import psutil
import datetime
import time
import serial
arduino = serial.Serial(port='COM3', baudrate=9600) 
while True:
    cpu = psutil.cpu_percent(interval=0.5 )
    ram = psutil.virtual_memory().percent
    disk = psutil.disk_usage("C:\\").percent
    now = datetime.datetime.now().strftime("%H:%M:%S")
        
    data = f"CPU={cpu}|RAM={ram}|DISK={disk}|NOW={now}\n"   
    arduino.write(data.encode("utf-8"))


