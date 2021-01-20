#this test emulates server
import socket
import sys
import requests
import time

url_w = 'http://api.openweathermap.org/data/2.5/weather?q={}&appid={}' #api from https://openweathermap.org/api
api_key_w = 'YOUR API'
def getWeather(city):
    result = requests.get(url_w.format(city, api_key_w))
    if result:
        json = result.json()
        city = json['name']
        country = json['sys']['country']
        temp_celsius = json['main']['temp'] - 273.15
        weather = json['weather'][0]['main']
        description = json['weather'][0]['description']
        pressure = json['main']['pressure']
        humidity = json['main']['humidity']

        ret_val = 'pub -t "weather/'+ city.lower().replace(' ', '') + '" -m "' + str(city) + ' ' + str(country) + ' ' + str( round(temp_celsius,2) ) + ' ' + str(weather) + ' '
        ret_val += str(description) + ' ' + str(pressure) + ' ' + str(humidity) + '"'

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
    cities = [getWeather('Belgrade'), getWeather('Novi Sad'), getWeather('Kragujevac'), getWeather('Nis')]

    for city in cities:
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
