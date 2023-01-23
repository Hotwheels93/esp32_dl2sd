# esp32_dl2sd
ESP32 - Download files via http and store it on sd card 

Use this function to download files from a server using the HTTPClient library included in the Arduino-ESP32 Core from Espressif.

Usage: download("URL to file", "destination_file"[absolute path]);

i.e.: download("http://scraperurl.php?hello.mp3", "/mp3/hi.mp3")
