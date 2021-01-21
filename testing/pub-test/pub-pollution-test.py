import socket
import sys
import requests
import time

url = 'http://api.airvisual.com/v2/city?city={}&state={}&country={}&key={}' #api from https://www.iqair.com/
api_key = 'YOUR API-KEY'

def getPollution(city, state, country):
    result = requests.get(url.format(city, state, country, api_key))
    if(result):
        json = result.json()
        country = json['data']['country']
        city = json['data']['city']
        aqi = json['data']['current']['pollution']['aqius']

        ret_val = 'pub -t "pollution/' + city.lower().replace(' ', '') + '" -m "' + str(country) + ': ' + str(city) + ' Index: ' + str(aqi) + ' '
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

        ret_val += '"'
        return ret_val
    else:
        return 0

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

for i in range(3):
    cities = [getPollution('Novi Sad', 'Autonomna Pokrajina Vojvodina', 'Serbia'), 
            getPollution('Belgrade', 'Central Serbia', 'Serbia'),
            getPollution('Kragujevac', 'Central Serbia', 'Serbia')]

    for city in cities:
        if(city == 0):
            print('Failed to get information about city')
            continue

        data = clientSock.recv(512)

        print('Expected: ' + city)
        print('Actual:   ' + data.decode())

        if(city != data.decode()):
            print('ERROR')
            sys.exit(3)
        else:
            print('OK\n')
        
    time.sleep(6) #6 seconds
    print('----------------------------')

print('ALL TESTS PASSED SUCCESSFULLY')
