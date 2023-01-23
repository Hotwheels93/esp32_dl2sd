#define FS_NO_GLOBALS   // avoid namespacing errors when using spiffs and sd card
#include <WiFiManager.h> 
#include <HTTPClient.h>
#include <SPI.h>
#include <SD.h>
// #include "SPIFFS.h" // uncomment to enable access to internal storage (EEPROM)
// #include "SdFat.h", uncomment if you want to use SDFat instead of the SD libray
// SdFat SD;


File uploadFile; // uploaded to SD 
boolean result; 

void setup() {
    SPI.begin(18, 19, 23); // HSPI or VSPI possible
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("WiFiSetup"); 
    while (!res) {
       delay(1000);
       Serial.println(".");
    }
    // delay(100) // uncomment if you get an error from watchdog
    Serial.println("IP address: ");
    Serial.print(WiFi.localIP());
    download("http://file-examples.com/wp-content/uploads/2017/11/file_example_MP3_2MG.mp3", "/test.mp3");
}

void loop() {  

}

void download(String file_url, String filePath_local) {
  HTTPClient Http;
  Http.begin(file_url);
  Serial.print("Start Dobwnload");
  Serial.println("URL: " + file_url);
  Serial.println("Destination path of file: " + filePath_local);
  int httpCode = Http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK)
    {
      uploadFile = SD.open(filePath_local, FILE_WRITE);
      int len = Http.getSize();
      Serial.printf("Size [%d] bytes.\r\n", len);
      uint8_t buff[2048] = {0}; // buffer can be bigger if additional PSRAM is installed 
      int cycles = len / 2048;
      int i = 0;
      WiFiClient *stream = Http.getStreamPtr();
      Serial.println("Downloading...");
      // read all data from server
      while (Http.connected() && (len > 0 || len == -1))
      {
        i++;
        size_t size = stream->available();
        if (size) {
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
          int cycles_left = cycles - i;
          uploadFile.write(buff, c);
          if (len > 0) {
            len -= c;
          }
        }
        delayMicroseconds(1);
      }
      Serial.println();
      Serial.print("Connection closed");
      result = true;
    }
  }
  else
  {
    Serial.printf("Failed downloading file, error: %s\n", Http.errorToString(httpCode).c_str());
  }
  Http.end();
  uploadFile.close();
}
