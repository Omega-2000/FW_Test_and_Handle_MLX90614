# FW_Test_and_Handle_MLX90614

**HARDWARE**
- Scheda Arduino Mega
- Alimentazione 5V
- Collegare il sensore di temperatura al connettore dedicato posizionato sulla shield, oppure collegarlo a SDA, SCL, GND, +5V

**AMBIENTE DI SVILUPPO**
- Arduino IDE
- Ambiente e librerie di Arduino

**LIBRERIE**
- Wire : libreria presente all'interno dell'ambiente Arduino per utilizzare l'i2c
- SparkFun_MLX90614 : libreria che gestisce il sensore di temperatura MLX90614

**FUNZIONAMENTO IN SINTESI**
- Il programma dopo aver stampato la lista dei comandi, attenderà che venga inviato in seriale il numero di un comando da eseguire per poi eseguirlo e ritornare il risultato.

**FUNZIONAMENTO CODICE**
- serialEvent() : funzione che legge la seriale e all'invio di "\n" ritorna "true" che indicherà la lettura della seriale avvenuta e si potrà passare al controllo del comando ricevuto

- confronta_stringhe() : funzione che avvia il test a seconda del comando ricevuto in seriale

- scan() : funzione che scannerizza il bus i2c e ritorna il risultato se trova qualche dispositivo i2c collegato oppure no

- change_i2c_address() : funzione che cambia indirizzo i2c al sensore di temperatura

- read_temperature() : funzione che legge la temperatura dal sensore di temperatura e ritorna il risultato in seriale

- setup() : inizializza seriale e i2c

- loop() : attende finchè la funzione "serialEvent()" ritornerà il valore true, per poi eseguire la funzione "confronta_stringhe"
