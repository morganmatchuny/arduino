/*
 * Author: Morgan Matchuny
 * 
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 50
 ** MISO - pin 51
 ** CLK - pin 52
 ** CS - pin 53

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
const int chip_select = 53;

void setup() {
  pinMode(53, OUTPUT);
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

  //open file
  Serial.println("SD card initialized.");
  file = SD.open("test.txt");
  
  unsigned long file_length = file.size();   //get length of file
  unsigned int place = 0;
  
  //buffer holds all bytes of hex file in array
  char hex_array[file_length];

  //if file not found
  if(!file) {
     Serial.print("The text file cannot be opened");
     while(1);
  }
     
   //print each line & info- debugging purposes
   while(file.available()) {
   
     //new lines start with ':', we don't want that to be terminating character
     hex_array[place++] = file.read();
  }

   //printing full array of hex file data
   for(int i = 0; i < (place - 1); i++) {
     Serial.write(hex_array[i]);
  }
  
   //close
   file.close();
}

void loop() {
  //nothing
}
