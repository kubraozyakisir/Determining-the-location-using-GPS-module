#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <TinyGPS++.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

DS3231  rtc(SDA, SCL);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// Choose two Arduino pins to use for software serial
  
// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 5 (for MKRZero SD: SDCARD_SS_PIN)

TinyGPS gps;
 
File myFile;

SoftwareSerial ss(4,3);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  lcd.init(); 
   lcd.begin(16, 2);
   lcd.print("waiting for gps");
  lcd.backlight();
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(9))
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } 
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) 
    {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else
  { // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

   rtc.begin();
 
  rtc.setDOW(PAZARTESI);      
  rtc.setTime(16, 45, 0);     
  rtc.setDate(30, 12, 2019);   
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
//     lcd.clear();
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    
//    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
//    delay(2000);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());

  
          
  } //lcdye yazdırmak icin fonksyonlar yeniden tanımlandı
   float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

       lcd.clear();
      lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    delay(2000);



    lcd.clear();
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    
     delay(2000);
    
    lcd.clear();
 lcd.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
   
   delay(2000);

  lcd.clear();
 lcd.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop()); 
   
   delay(2000);



  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("* No characters received from GPS: check wiring *");



//saat modulu adına kod void

    Serial.print(" Haftanin gunu: ");
  Serial.println(rtc.getDOWStr());
  lcd.clear(); 
  lcd.println(rtc.getDOWStr()); 
   delay(2000); 
    lcd.clear(); 
  Serial.print(" Tarih:");
  
  Serial.println(rtc.getDateStr());
  
  lcd.print(rtc.getDateStr());  
  delay(2000);
  

 Serial.print(" Saat: ");
 
  Serial.println(rtc.getTimeStr()); 
  lcd.clear();
  lcd.setCursor(1,4);
  
  lcd.clear();
  lcd.println(rtc.getTimeStr()); 
  delay(1000);
  

  
 Serial.println("--------------------------------");
  delay (1000);
}
