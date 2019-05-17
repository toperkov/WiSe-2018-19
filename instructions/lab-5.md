# Bežične senzorske mreže - Lab 5

### FESB, smjer 110/111/112/114/120, akademska godina 2018/2019

Nordic **nRF24L01+** je visoko integrirani primopredajnik male potrošnje (eng. *Ultra Low Power - ULP*) koji može transmitirati brzinom do **2Mbps** na frekvenciji od **2,4GHz** ISM (eng. *Industrial, scientific and medical*). Ovi primopredajnici koriste 2,4 GHz nelicencirani pojas poput mnogih WiFi routera, Bluetooth, nekih bežičnih telefona itd. čiji je raspon od **2,400 do 2,525 GHz**. Širina nRF24L01 kanala je 1 MHz što ukupno omogućava **125 nepreklapajućih kanala** (0 .. 124). Da ne bi došlo do interferencije sa WiFi-om predlaže se korištenje 25 najviših kanala. Naime, pri instalaciji Arduino RF24 biblioteke upotrebljava se kanal 76 koji se može promijeniti.

<img src="https://cloud.githubusercontent.com/assets/8695815/24585966/1f2fb258-1797-11e7-8946-5fc2f38f7846.jpg" width="300px" height="300px" />

Potrošnja nRF24L01+ radio modula prilikom transmisije/primanja paketa je u *aktivnom modu* manja of **14mA**, dok je *modu spavanja* manja od **1 uA**, što ga uz napajanje od 1,9 do 3,6V čini idealnim za realizaciju primopredajnika male potrošnje korištenjem AA/AAA baterija (eng. Ultra Low Power - ULP). Za realizaciju ukupnog radio sustava sa nRF24L01+ primopredajnikom, potreban nam je i mikrokontroler (npr. Arduino) te još neke pasivne komponente.

### Svojstva

 - Frekvencijski pojas 2,4 GHz
 - 126 radio kanala.
 - Brzina transmisije: 250kbps, 1 and 2Mbps.
 - GFSK modulacija, pojasne širine od 1 or 2MHz
 - Transmiter: 11.3mA na 0dBm izlazne snage.
 - Napajanje: od 1,9 V do 3,6 V
 - Mala veličina: 15mm*29mm

### Mala potrošnja snage:
 - 900nA sleep stanje
 - 11.3mA Radio TX na 0dBm
 - 13.3mA Radio RX na 2Mbps
 - Izlazna snaga 0, -6, -12, i -18dBm
 - Osjetljivost -94dBm RX pri 250 Kbps
 - Osjetljivost -82dBm RX pri 2 Mbps
 - Osjetljivost -85dBm RX pri 1 Mbps

<img src="https://cloud.githubusercontent.com/assets/8695815/24586100/bd5a43f6-1799-11e7-86cf-223bf6ca8d68.jpg" width="600px" />

Prilikom transmisije paketa svi uređaji koji slušaju na tom istom radio kanalu će primiti poruku. Transmiter prilikom slanja dodaje adresu primatelja u poruku (tzv. *pipe*) te prijemnik s druge strane ignorira sve poruke koje nemaju tu adresu. Ukupna veličina adrese je *5 byte*-ova (40 bitova). Nordic nRF24L01+ modul može istovremeno slušati poruke sa **6 pipe-ova**. Ti pipe-ovi omogućavaju nRF24 istovremeno slušanje poruka sa 6 različitih uređaja koje imaju 6 različitih (jedinstvenih) adresa. Ovo je primjer adresiranja u nRF24L01+:

``0xF0F0F0F0AALL``

gdje ``0x`` označava heksadecimalnu notaciju, dok ``LL`` označava Long Long.

## Radio interferencija

Ako dva ili više transmitera istovremeno prenesu poruku na istom kanalu oni će ometati jedni druge i poruka neće biti prenesena. Razlog tome je što nRF24 **ne može** istovremneno primati poruke sa svih 6 pipe-ova, odnosno sa 6 različitih uređaja. Isto tako može doći do kolizije ako se primjerice istovremeno transmitiraju poruke na istom kanalu na kojem radi neki WiFi sustav. Međutim, kako je za prijenos poruke potrebno veoma mali vremenski interval (nekoliko milisekundi), mala je vjerojatnost interferecije.

<img src="https://cloud.githubusercontent.com/assets/8695815/24586071/549f5202-1799-11e7-8c24-f000413e6073.png" width="600px" />

## Format paketa

Nordic nRF24L01+ podržava tzv "Enhanced Shockburst" mode koji ima sljedeće karakteristike:
 - Duljina payload-a poruke se može dinamički mijenjati
 - Pri primanju poruke nRF24 automatski šalje **ACK** poruku transmiteru kojom potvrđuje uspješno primanje poruke. Ukoliko se ACK poruka ne primi unutar predefiniranog isteka vremena, pošiljatelj automatski nastavlja *retransmisiju* poruke onaj broj puta dok se ACK paket ne primi unutar isteka vremena (timeout). Maksimalana broj puta koji se može paket retransmitirati je **15**. Ovo primjer naredbe kojom se definira timeout kao i broj retransmisija:

``radio.setRetries(3,5)``

U nastavku je dan format paketa kojeg nRF24L01+ šalje preko radio kanala:

<img src="https://cloud.githubusercontent.com/assets/8695815/24586075/7df737fa-1799-11e7-9e6b-fa889b121e00.png" width="600px" />

Kao što se može primjetiti, 9 bitno kontrolno polje sadži informaciju o duljini *payload-a*, tzv. *PID* za detekciju retransmisija i flag kojom se omogućava/onemogućava slanje *ACK* paketa. Također paket sadrži *CRC*. Primjetite da ukupno nRF24L01+ može prenijeti najviše **32 byte-a** informacije u jednom paketu. Za slanje više informacija poruke će ze razbiti u više manjih paketa.

# Zadatak

U sklopu današnje vježbe student će ralizirati jedan primopredajnik. Pri tom ćete se podijeliti u grupe po dva uređaja, gdje će jedan uređaj biti predajnik, a drugi prijemnik. Pri tome će svaka grupa koristiti svoj *pipe* odnosno adresu i to na način da ćete u dijelu koda u kojem se navodi ``slaveAddress``, primjerice za grupu 1 unijeti sljedeću adresu:

```arduino
const byte slaveAddress[5] = {'R','x','G','r','1'}; //RxGr1
```

Vaše nRF24L01+ primopredajnike spojite prema slici koja je dana u nastavku.

<img src="https://cloud.githubusercontent.com/assets/8695815/24586396/2791afe2-17a0-11e7-9d71-6c84ff14d9a4.png" width="400px" />

U direktoriju ``labs/lab-5`` se nalazi kod za transmiter (``SimpleTx.ino``) i receiver (``SimpleRx.ino``) pa ga testirajte.  
**NAPOMENA:** za realizaciju današnje vježbe koristit ćemo ``RF24`` verziju RF24 biblioteke pa ju je potrebno prethodno instalirati.

## Povećanje dometa

Domet veoma ovisi o tome nalazi li se naš nRF24L01 vani na otvorenom ili u zatvorenom, gdje signal prolazi kroz zidove. Uobičajena udaljenost kojom se može za low-power verziju uređaja je oko 100 m. Ovo se odnosi na otvoreni prostor u kojem uređaji transmitiraju brzinom od 250 Kbps. Primjerice, ako stavite ``radio.setPALevel(RF24_PA_LOW);`` imat ćete pouzdanu komunikaciju, ali samo na 5 m. Da biste povećali domet napravite sljedeće:

 - postavite RF *"Power Amplifier Level"* na MAX: ``radio.setPALevel(RF24_PA_MAX);`` Možete eventualno testirati još i ove snage signala: ``RF24_PA_MIN``, ``RF24_PA_LOW``, ``RF24_PA_HIGH`` and ``RF24_PA_MAX``,
 - postavite brzinu transmisije na 250 Kbps naredbom: Set ``radio.setDataRate(RF24_250KBPS);``. Testiraje još neke dostupne brzine: ``RF24_1MBPS``, ``RF24_2MBPS``, ``RF24_250KBPS``,
 - postavite frekvenciju transmisije na radio kanal 108 (ili više); tada se transmisija odvija iznad frekvencije WiFi-a: ``radio.setChannel(108);``
 
## Korisni linkovi
 [1] http://arduinoinfo.mywikis.net/wiki/Nrf24L01-2.4GHz-HowTo  
 [2] http://forum.arduino.cc/index.php?topic=421081.0  
 [3] http://arduino.stackexchange.com/questions/2946/nrf24l01-pipe-question  
 [4] https://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24L01P  
 [5] http://yveaux.blogspot.hr/2014/07/nrf24l01-sniffer-part-1.html  
 [6] https://www.mysensors.org/build/battery  
 [7] https://maniacbug.wordpress.com/2012/03/30/rf24network/