# Bežične senzorske mreže - Lab 6

### FESB, smjer 110/111/112/114/120, akademska godina 2018/2019

U sklopu današnje vježbe student će realizirati jednostavnu bežičnu senzorsku mrežu *zvjezdaste topologije* (``star topology``).

Mreža će se sastojati od jednog **centralnog uređaja**, te max. **pet krajnjih bežičnih senzorskih uređaja** sa svojom jedinstvenom adresom. Krajnji senzorski uređaji su opremljeni sa senzorima temperature/vlage **DHT11/22** i osvjetljenosti **BH1750** te radio primopredajnikom **nRF24L01**. Senzorski podaci se transmitiraju s periodom od 8-10 sekundi prema centralnom uređaju koji te podatke ispisuje na serijski port. Nakon transmisije senzorski uređaji iz aktivnog stanja (*active state*) ulaze u tzv. stanje spavanja (*sleep state*) u kojem ostaju 8 sekundi; max. vrijeme koje *watchdog timer* može držati mikrokontroler u *sleep* stanju.

## Krajnji senzorski uređaj

Vaš zadatak je spojiti krajnje senzorske uređaje prema slici:

<img src="https://cloud.githubusercontent.com/assets/8695815/24838259/eed6ec80-1d44-11e7-8137-7fabad4a0e53.png" width="400px" />

Kod za krajnji senzorski uređaj se nalazi u direktoriju ``vjezba/sender``. Kao što se može primjetiti, kod je organiziran pomoću *state mašin*e. Također, radi jednostavnosti razumijevanja rada, kod je razbijen u dvije biblioteke, jednu za čitanje senzora, te drugu za slanje/primanje poruka preko radio kanala.

Svaki senzorski uređaj (od njih pet) ima jedinstvenu adresu (tzv. *pipe*). Adresu svakom uređaju dodjeljujete pomoću ``const byte address[]`` varijable koja se nalazi u ``RAdio_nRF.cpp`` datoteci. Vaš zadatak je da se podijelite u grupe te svakom uređaju pridjelite jedinstvenu adresu. Primjerice:

```arduino
Senzor 1: const byte address[6] = {"1node"};
Senzor 2: const byte address[6] = {"2node"};
Senzor 3: const byte address[6] = {"3node"};
```

Također primjetite da preko radio kanala šaljemo strukturu ``SensorData`` koja se nalazi u ``TempHumLightState.h`` headeru u kojeg spremamo informacije o temperaturi, vlazi i osvjetljenju.

```arduino
// Payload that child sends to the parent
struct SensorData {
    float temp;
    float hum;
    uint16_t lightLevel;
};
```

Vaš zadatak je da integrirate kod slanje i primanje poruka preko radio kanala iz ``Vjezbe 5`` u funkcije ``RF_send()`` i ``RF_receive(bool)`` koje se nalaze u ``Radio_nRF.cpp datoteci``. **NAPOMENA:** Funkcija ``RF_send`` vraća *bool* vrijednost kojom potvrđuje ispravno je li senzor uspješno poslao paket (senzorski uređaj prima *ACK* paket). Tu informaciju ćemo proslijediti funkciji ``RF_receive`` koja kao ulaznu varijablu prima upravo navedenu bool varijablu.

## Centralni uređaj

Vaš zadatak je spojiti centralni senzorski uređaj prema slici:

<img src="https://cloud.githubusercontent.com/assets/8695815/24586396/2791afe2-17a0-11e7-9d71-6c84ff14d9a4.png" width="400px" />

Kod za krajnji senzorski uređaj se nalazi u direktoriju ``lab-6/receiver``. Primjetite u setup funkciji da centralni uređaj istovremeno otvara konekciju prema pet uređaja. Prilikom primanja podataka preko radio kanala, inforamcije se pohranjuju u strukturu ``SensorData``. Vaš zadatak je da ekstendirate postojeću funkciju ``showData`` na način da ispisujete na serijski port vrijednost temperature, vlage i osvjetljenja koje primite preko radio kanala i pokranite u strukturu ``SensorData``.

