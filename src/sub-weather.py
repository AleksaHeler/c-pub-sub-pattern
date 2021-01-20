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

url = 'http://api.openweathermap.org/data/2.5/weather?q={}&appid={}' #api from https://openweathermap.org/api
api_key = 'YOUR API'

def getWeather(city):
    result = requests.get(url.format(city, api_key))
    if result:
        json = result.json()
        city = json['name']
        country = json['sys']['country']
        temp_celsius = json['main']['temp'] - 273.15
        weather = json['weather'][0]['main']
        description = json['weather'][0]['description']
        pressure = json['main']['pressure']
        humidity = json['main']['humidity']

        ret_val = 'pub -t "weather" -m "' + str(city) + ' ' + str(country) + ' ' + str( round(temp_celsius,2) ) + ' ' + str(weather) + ' '
        ret_val += str(description) + ' ' + str(pressure) + ' ' + str(humidity) + '"'

        return ret_val  
    else:
        return 0

while(1):
    cities = [getWeather('Belgrade'), getWeather('Novi Sad'), getWeather('Kragujevac'), getWeather('Niš')]

    for city in cities:
        time.sleep(2) #2 seconds
        try:
            ret_val = sock.send(city.encode())
            print('Message sent: ' + city)
        except:
            print("Error sending message")
            sys.exit(3)

    time.sleep(6) #6 seconds
    print()