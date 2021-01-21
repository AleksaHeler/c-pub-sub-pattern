make server
make client

Na početku se pokreće server: 
    ./server

Zatim se publisheri pokreću nakon pozicioniranja u src folder (nije bitan redosled pokretanja):
    python3 pub-numbers.py
    python3 pub-years.py
    python3 pub-trumpy.py
    python3 pub-weather.py
    python3 pub-pollution.py
    python3 pub-crypto.py

Na kraju se pokreće klijent: 
    ./client
