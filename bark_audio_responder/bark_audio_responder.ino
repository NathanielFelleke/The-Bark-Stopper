#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <SoftwareSerial.h>

#define VS1053_RESET   -1
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin


Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

const int numOfTracks = 5;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  if (!musicPlayer.begin()) { // initialise the music player
    Serial.println("Couldn't find VS1053, do you have the right pins defined?");
    while (1);
  }

  Serial.println("VS1053 found");

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");

  // list files
  printDirectory(SD.open("/"), 0);

  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

  musicPlayer.setVolume(10, 10);

  Serial.print("Playing start sound");
  Serial.println(musicPlayer.playFullFile("/track000.mp3")); //play start up sound
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(2));
  if (digitalRead(2)) {
    int randomTrack = round(random(1, numOfTracks)); //play random track two times
    musicPlayer.playFullFile(String("/track00" + String(randomTrack) + ".mp3").c_str()); 
    musicPlayer.playFullFile(String("/track00" + String(randomTrack) + ".mp3").c_str());
    delay(10000); //wait 10 seconds until listening for next message
  }

}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
