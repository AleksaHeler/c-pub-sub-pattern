from requests import Request, Session
from requests.exceptions import ConnectionError, Timeout, TooManyRedirects
import json

import socket
import sys
import time

url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/listings/latest'
parameters = {
  'start':'1',
  'limit':'5',
  'convert':'USD'
}
headers = {
  'Accepts': 'application/json',
  'X-CMC_PRO_API_KEY': 'YOUR API-KEY',
}

session = Session()
session.headers.update(headers)

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


for x in range(3):
    try:
        response = session.get(url, params=parameters)
        json = response.json()
    except (ConnectionError, Timeout, TooManyRedirects) as e:
        print(e)
        sys.exit(3)
  
    name = json['data'][0]['name']
    symbol = json['data'][0]['symbol']
    price = json['data'][0]['quote']['USD']['price']
          

    ret_val = 'pub -t "crypto" -m "'
    ret_val += str(1) + '. ' + str(symbol) + '(' + str(name) + ') USD: ' + str( round(price, 4) ) + '\n'
  
    for i in range(1,5):
        name = json['data'][i]['name']
        symbol = json['data'][i]['symbol']
        price = json['data'][i]['quote']['USD']['price']
          
        ret_val += str(i + 1) + '. ' + str(symbol) + '(' + str(name) + ') USD: ' + str( round(price, 2) ) + '\n'

    ret_val += '"'
    

    data = clientSock.recv(512)
    if(ret_val != data.decode()):
        print('ERROR')
        sys.exit(4)
    else:
        print('OK ' + str(x))

    time.sleep(10) #10 seconds

print('ALL TESTS PASSED SUCCESSFULLY')
