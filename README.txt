Uputstvo za korišćenje
 - Kako prevesti
 	U folderu "src" se nalaze izvorni kod i makefile
 	U terminalu u tom folderu pokrenuti komande:
 		make server
 		make client
 	Potrebno je imati napravljen folder "obj" unutar foldera "src"
 
 - Kako pokrenuti
 	Izlaz kompajlovanja pomoću makefile-a su dve datoteke: server i client
 	Pokrenuti ih sledećim komandama:
 		./server
 		./client
 	
 	Server aplikacija nema interakcije sa korisnikom, samo ispis statusa u terminal.
 	Klijent aplikacija se koristi preko izbora u meniju, unošenjem rednog broja stavke.

    python fajlovi služe kao publisheri koji šalju informacije sa web-a
    Pokreću se sa: 
        python3 pub-numbers.py
        python3 pub-years.py
        python3 pub-trumpy.py

    Za ove je potreban api-key:
        python3 pub-weather.py
        python3 pub-pollution.py
        python3 pub-crypto.py


 	
 
