# c-pub-sub-pattern
Projektni zadatak iz predmeta ORM 1

## Zadatak: Publish – Subscribe Server – topic based
Realizacija jednostavnog modela poslužioca publish-subscribe asocijacija koristeći TCP protokol. Poslužilac spada u klasu „topic based“ poslužilaca. Realizovati i primer korišćenja poslužioca.

Primer: korisnik 1 se povezuje sa poslužiocem i traži prijem događaja o vremenskoj prognozi (subscriber 1). Korisnik 2 se povezuje sa poslužiocem i traži prijem događaja o rezultatima fudbalskih utakmica (subscriber 2). Sa poslužiocem se povezuje i nekoliko korisnika koji šalju događaje o raznim temama (publishers). Prikazati kako poslužilac filtrira i objavljuje tražene događaje prijavljenim korisnicima.

## TODO: zapisati strukturu programa ovde
Server:
 - provera dobijene poruke (format tacan)
 - poziva odgovarajucu funkciju:
   - publish: prosledi tekst poruke svim klijentima koji su prijavljeni na datu temu (format: -t "weather" -m "22C")
   - subscribe: prijavi klijenta na topic (ako je vec prijavljen nista se ne desi)
   - unsubscribe: odjavi klijenta sa topica (ako nije ni bio prijavljen nista se ne desi)
 - server cuva podatke o svim klijentima u nizu struktura
 
Client:
 - provera unosa (format tacan)
 - posalje serveru

## TODO: zapisati kako se koristi program
Server: 
 - za server ne postoje komande, u terminalu se ispisuje samo debug

Client:
 - sub -t "weather" (subscribes to topc weather)
 - unsub -t "weather"
 - pub -t "weather" -m "22C" (publish message to topic weather)
