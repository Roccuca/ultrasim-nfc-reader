# UltraSim-NFC Reader

Firmware per il lettore NFC integrato nella sonda di **UltraSim-NFC**, simulatore ecografico low-cost basato su tag NFC applicati al manichino.

Il lettore legge l'UID del tag e lo invia al dispositivo collegato come **tastiera USB HID**: l'UID viene "digitato" nel campo attivo, seguito da Invio. Non servono driver.

## Hardware

- ESP32-S3 Super Mini
- Modulo Mini RC522 (13,56 MHz)
- Cavo USB-C (alimentazione e dati)

## Collegamenti

| Mini RC522 | ESP32-S3 Super Mini |
|---|---|
| 3V3 | 3V3 |
| GND | GND |
| SDA | GPIO 8 |
| SCK | GPIO 7 |
| MOSI | GPIO 5 |
| MISO | GPIO 3 |
| RST | GPIO 1 |
| IRQ | non collegato |

## Compilazione (Arduino IDE)

1. Installa il core **esp32** di Espressif (versione 3.x).
2. Installa la libreria **MFRC522** dal Library Manager.
3. Imposta in *Strumenti*:
   - Board: **ESP32S3 Dev Module**
   - USB CDC On Boot: **Enabled**
   - USB Mode: **USB-OTG (TinyUSB)**
4. Apri `nfc_hid_reader/nfc_hid_reader.ino` e carica.

## Comportamento

- UID in esadecimale maiuscolo, seguito da Invio.
- Lo stesso tag viene ignorato per 2 secondi, per evitare letture doppie.
- `PCD_StopCrypto1()` non viene chiamato di proposito, perché causava il blocco del lettore.

Al primo collegamento alcune schede si presentano con un nome USB diverso (ad esempio "Ozobot Circuit Kit"): è il descrittore di fabbrica e sparisce dopo il caricamento del firmware.
