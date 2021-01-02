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

url = 'http://api.airvisual.com/v2/city?city={}&state={}&country={}&key={}' #api from https://www.iqair.com/
api_key = 'YOUR API'

def getPollution(city, state, country):
    result = requests.get(url.format(city, state, country, api_key))
    if(result):
        json = result.json()
        country = json['data']['country']
        city = json['data']['city']
        aqi = json['data']['current']['pollution']['aqius']

        ret_val = str(country) + ': ' + str(city) + ' Index: ' + str(aqi) + ' '
        if(aqi <= 50):
            ret_val += 'Good'
        elif(aqi <= 100):
            ret_val += 'Moderate' 
        elif(aqi <= 150):
            ret_val += 'Unhealthy for Sensitive Groups'
        elif(aqi <= 200):
            ret_val += 'Unhealthy'
        elif(aqi <= 300):
            ret_val += 'Very Unhealthy'
        elif(aqi <= 500):
            ret_val += 'Hazardous'

        ret_val += 'p'
        return ret_val
    else:
        return 0

while(1):
    cities = [getPollution('Novi Sad', 'Autonomna Pokrajina Vojvodina', 'Serbia'), 
            getPollution('Belgrade', 'Central Serbia', 'Serbia'),
            getPollution('Kragujevac', 'Central Serbia', 'Serbia'),
            getPollution('Nis', 'Central Serbia', 'Serbia')]

    for city in cities:
        if(city == 0):
            continue

        time.sleep(2) #2 seconds
        try:
            ret_val = sock.send(city.encode())
            print('Message sent: ' + city)
        except:
            print("Error sending message")
            sys.exit(3)

    time.sleep(6) #6 seconds
    print()
