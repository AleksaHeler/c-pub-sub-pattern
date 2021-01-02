import socket
import sys
import requests
import time

#socket creation and connection
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")
except:
    print("Can't create socket")
    sys.exit(1)

try:
    sock.connect( ("127.0.0.1", 27015) )
    print("Connection the the server established")
except:
    print("Can't connect to the server")
    sys.exit(2)


url = 'http://numbersapi.com/random?min=0&max=551' #api from http://numbersapi.com/#42
#sending data
while(1):
    try:
        result = requests.get(url.format()).text + 'n'
        ret_val = sock.send(result.encode())
        print('Message sent: ' + result)
    except:
        print("Error sending message")
        sys.exit(3)

    time.sleep(6) #6 seconds