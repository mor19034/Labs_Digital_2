# Código de laboratorio 5
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Pablo Moreno

from Adafruit_IO import Client, RequestError, Feed
import serial
import time

envio = 184     #dato que viene desde la pág
nulo = 0        #valor nulo
nulo = str(nulo)#char nulo
temporal1 = 0
temporal2 = 0

#ubicación de mi plataforma
ADAFRUIT_IO_KEY = "aio_VifF06V2bL60eCycFW47k9ElAwgp"  #contraseña
ADAFRUIT_IO_USERNAME = "mor19034"                    #usuario
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#Defino nombre de comunicación
puerto = serial.Serial("COM8",9600) 
puerto.timeout = 3 #tiempo de espera para recibir datos
time.sleep(1) #para que establezca conexion

print('Puerto activado con exito \n')
#----------------Temperatura y humedad valor---------------------------------

while True:
    with puerto:   #utilice el COM serial
        #nombre_feed = cliente.feed('llave_de_mi_feed')
        uart_feed = aio.feeds('contador-uart')
        botones_feed = aio.feeds('contador-botones')

        #variable = cliente.receive(mifeed_feed.key) ; Esto para leer valor
        uart_data = aio.receive(uart_feed.key)
        botones_data = aio.receive(botones_feed.key)
        
        valor = int(uart_data.value)
        puerto.write(b's') #mando codigo de caracter
        val = puerto.readline(3).decode('ascii') #recibo 2 bytes
        val = int(val)  

        if(valor != temporal1 or val1 != temporal2):
            print(f'valor que se le envia al pic: {uart_data.value}')
            print(f'valor de contador de botones: {botones_data.value} \n')

            if (valor < 10 and valor > -1):
                puerto.write(nulo.encode('ascii')) #envio un cero
                puerto.write(nulo.encode('ascii'))
                
            elif (valor <100 and valor > 9):
                puerto.write(nulo.encode('ascii')) #envio un cero
            
            envio = str(int(uart_data.value))
            puerto.write(envio.encode('ascii'))
            print("Escrito en pic: {} \n".format(envio))
            print("Escrito en AdaFruit: ")
            print(val)
            
            #cliente.send_data(mifeed.feed.key, valor a enviar) ; Esto para mandar dato y leer
            aio.send_data(botones_feed.key, val)
            print('----------------------------------------------------')
            temporal1 = int(uart_data.value)
            temporal2 = int(botones_data.value)
