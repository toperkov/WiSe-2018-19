# Bežične senzorske mreže - Lab 7

## MQTT

MQTT (eng. ``Message Queuing Telemetry Transport``) je protokol za komunikaciju koji je razvijen u IBMu. Radi na vrhu TCP/IP protokola na portu 1883 ili na portu 8883 ukoliko se koristi SSL veza. MQTT radi na principu ``publish/subscribe`` te je kreiran je za kao jednostavan protokol za upotrebu. Dizajniran je kako bi se minimizirala propusnost mreže i zahtjevi resursa uređaja. Jedna od bitnih značajki koju su uz prije navedeno željeli ostvariti je da se osigura pouzdanost i određeni stupanj osiguranja isporuke. Zbog toga je postao jako rasprostranjen u primjeni i popularan. Najviše se koristi u IoT (eng. ``Internet of things`` – Internet stvari) uređajima i M2M (eng. ``Machine to machine``) komunikaciji gdje je potreban protokol koji ne povećava potrošnju energije.

U strukturi ``publish/subscribe`` je jako bitan server na koji će se poruke slati, a nazivamo ga brokerom (eng. ``message broker``). On služi kao središnji organizacijski dio cijelog protokola. Naime, sve objavljene poruke (``publish``) su zapravo poslane na broker u određenu temu (eng. ``topic``). Teme su informacije o smjeru poruke u neki unaprijed određeni dio memorije servera koji ima naziv te teme. Na te teme se može bilo koji korisnik može pretplatiti (eng. ``subscribe``). Broker nakon primitka poruke na određenu temu, tu poruku prosljeđuje svim korisnicima koji su se prethodno pretplatili na tu temu. Onog trenutka kada broker proslijedi poruku, on tu poruku briše iz svoje privremene memoriju u koju je bila pohranjena. Na slici ispod je prikazana arhitektura takvog jednog sustava koja će se upotrebljavati u današnjoj vježbi.

## Zadatak

U sklopu današnje vježbe student će korištenjem Python programskog okruženja slati senzorske podatke na MQTT broker te će se odgovarajućim klijentom pretplatiti na navedene podatke. Naime, u prethodnoj vježbi studenti su realizirali jednostavnu senzorsku mrežu gdje su podatke od temperaturi i vlažnosti sa jednog senzora slali putem radio kanala prema drugom Arduino uređaju koji je nadalje te senzorske podatke ispisivao na serijskom portu na računalu. Cilj današnje vježbe je navedene senzorske podatke koji su poslani putem serijskog porta na računalo prosljeđivati putem MQTT protokola prema brokeru koji će prosljeđivati te podatke studentu na mobitel. Naime, krajnji cilj današnje vježbe je  vizualizirati u realnom vremenu kroz aplikaciju na mobilnom uređaju koju će student skinuti preko Google Play store-a.

### Uputstva za instalaciju i pokretanje aplikacije

Spojite uređaj prema uputama koje su dane u prethodnoj vježbi. Za razliku od prošlog puta, profesor će povezati ``Sender`` uređaj, dok će student povezati ``Receiver`` uređaj sa kojeg će putem serijskog porta slati senzorske podatke prema računalu.

Prije korištenja dolje navedenog kod provjerite je li instaliran Python na vašem računalu. Nadalje, za realizaciju ove vježbe potrebno je instalirati Mosquitto Python modul pod nazivom Paho MQTT client, tako da jednostavno utipkate sljedeću naredbu u CLI (Command Line):

`pip install paho-mqtt`

Sada ćemo kreirati jednostavnu python skriptu koja će slati podatke sa serijskog porta prema javno distupnom brokeru. Otvorite text editor (npr. ``Notepad++``), sačuvajte datoteku sa imenom koja ima ekstenziju `.py`, primjerice `sensor.py`. Nakon toga u datoteku kopirajte ispod naveden kod:

```python
import serial
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
CLIENTID = "MQTTExample"
TOPIC_ONE = "sensors/temperature/data"
TOPIC_TWO = "sensors/humidity/data"
QOS = 1

import time

mqttc = mqtt.Client(CLIENTID)
client.connect(BROKER)

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5)  # open first serial port
while True:
	message = ser.readline()
	print message
	if "T:" in message:
		str, temp = message.split(" ")
		mqttc.publish(TOPIC_ONE, payload=temp, qos=QOS, retain=False)
	if "H:" in message:
		str, hum = message.split(" ")
		mqttc.publish(TOPIC_TWO, payload=hum, qos=QOS, retain=False)
	time.sleep(0.01)
mqttc.disconnect()
time.sleep(1)
```

Primjetite da bi se poruka uspješno poslala prema brokeru, ona putem serijskog porta mora biti poslana u formatu npr. **"T: 25.3"** ili **"H: 33"**. Prema tome, prilagodite kod na Arduino Receiveru kako bi slao podatke prema serijskom portu u navedenom formatu.

Nakon što su podaci uspješno poslani prema MQTT brokeru, potrebno je iste vizualizirati na mobilnoj aplikaciji. Za tu svrhu instalirajte i testirajte neke od javno i besplatno dostupnih aplikacija na ``Google Play Store``-u.