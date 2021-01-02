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
  'X-CMC_PRO_API_KEY': 'YOUR API',
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
    sock.connect( ("127.0.0.1", 27015) )
    print("Connection the the server established")
except:
    print("Can't connect to the server")
    sys.exit(2)


while(1):
    try:
        response = session.get(url, params=parameters)
        json = response.json()
    except (ConnectionError, Timeout, TooManyRedirects) as e:
        print(e)
        sys.exit(3)
  
    name = json['data'][0]['name']
    symbol = json['data'][0]['symbol']
    price = json['data'][0]['quote']['USD']['price']
          
    ret_val = str(1) + '. ' + str(symbol) + '(' + str(name) + ') USD: ' + str( round(price, 2) ) + '\n'
  
    for i in range(1,5):
        name = json['data'][i]['name']
        symbol = json['data'][i]['symbol']
        price = json['data'][i]['quote']['USD']['price']
          
        ret_val += str(i + 1) + '. ' + str(symbol) + '(' + str(name) + ') USD: ' + str( round(price, 2) ) + '\n'

    ret_val += 'c'

    #send data to server
    try:
        result = sock.send(ret_val.encode())
        print('Message sent:\n' + ret_val)
    except:
        print("Error sending message")
        sys.exit(4)

    time.sleep(10) #10 seconds
