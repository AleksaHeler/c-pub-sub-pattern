#this test emulates server
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
    sock.bind(('127.0.0.1', 27015))
    print("Server created")
except:
    print("Can't create server")
    sys.exit(2)

sock.listen(1)
print('Listening for 1 connection')

print('Waiting for connection\n')
clientSock, addr = sock.accept()

#facts about years are sent randomly so we can only check if their format is correct
for i in range(15):
    data = clientSock.recv(512)

    if(data.decode().startswith('pub -t "numbers" -m "')):
        if(data.decode().endswith('"')):
            print('OK ' + str(i))
        else:
            print('ERROR')
            sys.exit(3)
    else:
        print('ERROR')
        sys.exit(3)

print('ALL TESTS PASSED SUCCESSFULLY')