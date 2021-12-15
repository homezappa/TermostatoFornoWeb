# TermostatoFornoWeb
Pensato per mantenere la temperatura costante all'interno di un forno, spento, utilizzando una normale lampadina ad incandescenza come riscaldatore.
Viene utilizzato un DHT22 per controllare la temperatura, un relays per accendere e spegnere la lampada.
Mantiene tranquillamente una temperatura di 30 - 32 gradi (utili per effettuare lievitazioni> per panettone, colomba, ecc)
Viene utilizzato un ESP8266 come server web per variare i parametri.
L'ESP8266 genera anche un access point con SSID "TERMOSTATO" ... associando un telefonino a detto access point e navigando http://10.10.10.10 si potrà configurare la temperatura e controllarla a video.
È anche possibile far collegare l'ESP8266 alla nostra rete locale casalinga: all'uopo modificare le due define per SSID e password.
Il file webserver.html contiene la pagina web, completamente locale. (le immagini delle icone sono nel codice, in base64)


Liberi di usarlo come vi piace ...
