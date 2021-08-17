# Código de laboratorio 5
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Pablo Moreno

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_YCmA99tGPD5IWk68fd0PvjbsJrtr"
ADAFRUIT_IO_USERNAME = "mor19034"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while True:
    lab05_sensor1 = aio.feeds('sensor-1')
    sensor_1 = int(input('Que valor queres para el sensor 1?: ')) #valor del sensor 1
    aio.send_data(lab05_sensor1.key, sensor_1)
    sensor1_data = aio.receive(lab05_sensor1.key)
    print(f'Sensor_1 : {sensor1_data.value}')

    lab05_sensor2 = aio.feeds('sensor-2')
    sensor_2 = int(input('Que valor queres para el sensor 2?: ')) #valor del sensor 1
    aio.send_data(lab05_sensor2.key, sensor_2)
    sensor2_data = aio.receive(lab05_sensor2.key)
    print(f'Sensor_2 : {sensor2_data.value}')