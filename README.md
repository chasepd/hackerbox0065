# Hackerbox 0065 - Realtime
My code for Hackerbox 0065 - Realtime. I created a cryptocurrency ticker:

![cryptoticker](https://raw.githubusercontent.com/chasepd/extras/main/hackerbox0065/cryptoticker.jpg)

**Parts:**

- 64x32 LED Matrix
- ESP32
- ~~Battery Pack~~ (I originally used a battery pack, but this ate 4 AA batteries per day so I switched to outlet power)
- 5V AC/DC Power Adapter capable of at least 2A
- Power Connector to connect board to AC/DC adapter
- Hackerbox 0065 Blaster

The blaster shield is optional, you could make this without it with some extra wiring. See [https://www.instructables.com/HackerBox-0065-Realtime/](https://www.instructables.com/HackerBox-0065-Realtime/) for details on what the blaster actually does.


**Steps:**

1. Hook everything up so that the ESP32 can control the LED matrix (see the above link to the Hackerbox instructables page for more detail)
2. Start python/dataserver.py (I am running it on a Raspberry pi server)
3. Upload the sketch esp32/Hackerbox0065Realtime.ino to the esp32 using the Arduino IDE


**Python Library Dependencies**:

- pycoingecko


**Arduino Library Dependencies**:

- Wifi by Arduino
- SmartMatrix by Louis Beaudoin
- ArduinoHttpClient by Arduino
