/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2022 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "sbus.h"
#include <ArtnetWifi.h>
#define ONBOARD_LED  1

ArtnetWifi artnet;

// const char* ssid = "MagentaWLAN-TTX9";
// const char* password = "61986585827922876861";

const char* ssid = "agj";
const char* password = "kreuzzugang!";


int tixPin = 17;
int rixPin = 16;

/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial1, rixPin, tixPin, true);
/* SBUS data */
bfs::SbusData data;

bool ConnectWifi(void) 
{
  bool state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  //// Serial.println("");
  //// Serial.println("Connecting to WiFi");
  
  // Wait for connection
    // Serial.print("Try connected to " );
    // Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    // Serial.println("");
    // Serial.print("Connected to " );
    // Serial.println(ssid);
    // Serial.print("IP address: ");
    // Serial.println(IPAddress(WiFi.localIP()));
  } else {
    // Serial.println("");
    // Serial.println("Connection failed.");         
  }

  return state;
}


void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* dataart)
{

      digitalWrite(33, HIGH);


  bool debug = false;

  bool tail = false;

  // copy from ARTNET to ESP32 DMX Interface


      data.ch[0] = map((int)dataart[20],0,255,352,1696);
      data.ch[1] = map((int)dataart[21],0,255,352,1696);
      data.ch[3] = map((int)dataart[22],0,255,352,1696);
      data.ch[4] = map((int)dataart[23],0,255,0,2500);
      data.ch[5] = map((int)dataart[24],0,255,0,1000);
      data.ch[6] = map((int)dataart[25],0,255,0,2500);

 



       // Serial.print("DMX: Univ: ");
      // Serial.print(universe, DEC);
      // Serial.print(", Seq: ");
      // Serial.print(sequence, DEC);
      // Serial.print(", Data (");
      // Serial.print(length, DEC);
      // Serial.print("): ");
        if (length > 16) {
          length = 16;
          tail = true;
        }
        // send out the buffer
        for (int i = 0; i < length; i++)
        {
          // Serial.print(dataart[i], HEX);
          // Serial.print(" ");
        }
        if (tail) {
          // Serial.print("...");
        }
        // Serial.println();
      
      
        // Serial.print("SBUS: Output: ");
      
      
      // send out the buffer
        for (int i = 0; i < length; i++)
        {
          // Serial.print(data.ch[i]);
          // Serial.print(" ");
        }
        if (tail) {
          // Serial.print("...");
        }
  // Serial.println();


}

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
      pinMode(ONBOARD_LED,OUTPUT);
      digitalWrite(ONBOARD_LED,HIGH);

  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_tx.Begin();

  bool connected = false;

  while(!connected) {

   connected = ConnectWifi();

   if(!connected) {
    // Serial.println("Could not connect to WIFI - will try again");
    delay(1000);
   }
    
  }

 

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
}


void loop () {


  if (WiFi.status() != WL_CONNECTED) {
        // Serial.println("LOST CONNECTION");

    digitalWrite(32, LOW);

      bool new_connect = false;
    while(!new_connect){
      new_connect = ConnectWifi();
    }
    
  } else {


    artnet.read();


    /* Set the SBUS TX data to the received data */
    sbus_tx.data(data);
    /* Write the data to the servos */
    sbus_tx.Write();
  }
}
