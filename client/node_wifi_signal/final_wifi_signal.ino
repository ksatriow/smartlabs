// PROGRAM PENGUKUR KEKUATAN SINYAL WIFI

#include "ESP8266WiFi.h"
const int RSSI_MAX =-50; // Tentukan kekuatan maksimum sinyal dalam dBm 
const int RSSI_MIN =-100; // Tentukan kekuatan minimum sinyal dalam dBm


//konfigurasi I2C LCD 
#include <LiquidCrystal_I2C.h>
// Tentukan address LCD (Terlebih dulu scan alamat LCDnya)
LiquidCrystal_I2C lcd(0x27, 16, 2);

int count =0;

int scanDelayTime = 1000;
int wiFiDisplayTime = 3000;

void setup() {
  // Konfigurasi WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();//disconnect from an AP if it was previously connected
    
  lcd.begin();  
  lcd.backlight();
  lcd.print("Kukuh");
  lcd.setCursor(0,1);
  lcd.print("Wifi Scan");  
  delay(2000); 
}


void loop() {
 // Kukuh WiFi Scan

// WiFi.scanNetworks akan menghasilkan semua jaringan yang masih dalam jangkauan 
  int n = WiFi.scanNetworks();
  lcd.clear();// menghapus tampilan lcd
  lcd.print("Penelusuran selesai");     
  if (n == 0) {
    lcd.print("WiFi Tidak Ditemukan");   
  } else {
    lcd.setCursor(0,1);
    lcd.print(n);
    lcd.print( "WiFi Ditemukan");
    for (int i = 0; i < n; ++i) {

      // Print SSID and RSSI for each network found
      lcd.clear();// clear previous values from screen
      lcd.setCursor(0,0);
      lcd.print(i+1);      
      lcd.print(") "); 
      lcd.print(WiFi.SSID(i));     
      lcd.setCursor(0,1);        
      lcd.print("S: ");      
      lcd.print(WiFi.RSSI(i)); 
      lcd.print("dBm,");
      lcd.print(dBmtoPercentage(WiFi.RSSI(i)));
      lcd.print("%)");
      delay(wiFiDisplayTime);
    }
  }  

  delay(scanDelayTime); 
}

int dBmtoPercentage(int dBm)
{
  int quality;
    if(dBm <= RSSI_MIN)
    {
        quality = 0;
    }
    else if(dBm >= RSSI_MAX)
    {  
        quality = 100;
    }
    else
    {
        quality = 2 * (dBm + 100);
   }

     return quality;
}
