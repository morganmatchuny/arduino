/*
 * Author: Morgan Matchuny
 * 
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 

Line format for Intel HEX hex file:

  :bbaaaatt(data)cc

  Where:
  :      = a colon

  (All of below in hex format)

  bb     = length of data part
  aaaa   = address (eg. where to write data)
  tt     = transaction type
           00 = data
           01 = end of file
           02 = extended segment address (changes high-order byte of the address)*
           03 = start segment address *
           04 = linear address 
           05 = start linear address *
  (data) = variable length data
  cc     = sumcheck

            * = We don't use these
*/

//Basic includes for SD config
#include <SPI.h>
#include <SD.h>
#include <utility/SdFat.h>
#include <avr/eeprom.h>

//file object and chip select
File file;
const int chip_select = 10;

void setup() {
  // open serial communications and wait for port to open
  Serial.begin(9600);
  while (!Serial) {
    ; 
 }
 
  Serial.print("Initializing SD card...\n");
  
  // see if the card is present and can be initialized
  if(!SD.begin(chip_select)) {
     Serial.println("Card failed, or not present");
     return;
  }

  Serial.println("SD card initialized.");
  file = SD.open("test.txt");
  
  //if file not found
  if(!file) {
     Serial.print("The text file cannot be opened");
     while(1);
  }
     
   //print each line & info- debugging purposes
   while(file.available()) {
      
     unsigned long file_length = file.size();   //get length of file
     Serial.println("The file length is: ");
     Serial.println(file_length);              //5657 bytes

     //each chunk is 512 bytes: BYTE keyword no longer supported- use (char(512))
     int chunk_size = (char(512));
     
     //buffer holds 512 bytes at a time
     String buffer_array[chunk_size];

       //print which chunk # we are on
       for(int i = 1; i < 10; i++) {  
           Serial.println("current chunk: = ");
           Serial.println(i);
         
         //buffer prints 16 lines at a time, 32 bytes each = 512 bytes
         for(int j = 0; j < 16; j++) {  
             //new lines start with ':', we don't want that to be terminating character
             buffer_array[chunk_size] = file.readStringUntil('\n');
             delay(500);
             //print buffer holding 512 bytes
             Serial.println(buffer_array[chunk_size]);
         }
      }
  }
   //close
   file.close();
}

void loop() {
  //nothing
}
