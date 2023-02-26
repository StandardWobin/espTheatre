#include <ArtnetWifi.h>
#include <Arduino.h>
#include <esp_dmx.h>
// Example, transmit all received ArtNet messages (DMX) out of the serial port in plain text.





int transmitPin = 17;
int receivePin = 16;
int enablePin = 21;
dmx_port_t dmxPort = 1;
byte datadmx[DMX_MAX_PACKET_SIZE];
int packetCounter = 44;
byte incrementValue = 0;


const char* ssid = "agj";
const char* password = "kreuzzugang!";


//Wifi settings
// const char* ssid = "MagentaWLAN-TTX9";
// const char* password = "61986585827922876861";

// const char* ssid = "FRITZ!Box Fon WLAN 7390";
// const char* password = "3223026829687110";



WiFiUDP UdpSend;
ArtnetWifi artnet;

// connect to wifi – returns true if successful or false if not
bool ConnectWifi(void) 
{
  bool state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
    Serial.print("Try connected to " );
    Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }

    if(i % 2 == 0) {
          digitalWrite(32, HIGH);
    } else {
          digitalWrite(32, LOW);

    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to " );
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(IPAddress(WiFi.localIP()));
              digitalWrite(32, HIGH);
                    digitalWrite(33, LOW);


  } else {
    Serial.println("");
    Serial.println("Connection failed.");
              digitalWrite(32, LOW);
                                  digitalWrite(33, LOW);


  }
  
  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* dataart)
{

      digitalWrite(33, HIGH);


  bool debug = false;

  bool tail = false;

  // copy from ARTNET to ESP32 DMX Interface
   for (int i = 0; i < 512; i++)
  {

    datadmx[i] = (int)dataart[i];
  }





  if(debug)  {


             Serial.print("DMX: Univ: ");
      Serial.print(universe, DEC);
      Serial.print(", Seq: ");
      Serial.print(sequence, DEC);
      Serial.print(", Data (");
      Serial.print(length, DEC);
      Serial.print("): ");
        if (length > 16) {
          length = 16;
          tail = true;
        }
        // send out the buffer
        for (int i = 0; i < length; i++)
        {
          Serial.print(dataart[i], HEX);
          Serial.print(" ");
        }
        if (tail) {
          Serial.print("...");
        }
        Serial.println();
      
      
        Serial.print("DMX: Output: ");
      
      
      // send out the buffer
        for (int i = 0; i < length; i++)
        {
          Serial.print(datadmx[i]);
          Serial.print(" ");
        }
        if (tail) {
          Serial.print("...");
        }
  Serial.println();

  }







  
}

void setup()
{
  // set-up serial for debug output
  Serial.begin(115200);

    // red
    pinMode(33, OUTPUT);
    
    // blue
    pinMode(32, OUTPUT);
    
    digitalWrite(33, HIGH);
    digitalWrite(32, HIGH);
    
    delay(100);
    digitalWrite(32, LOW);

  

  bool connected = false;

  while(!connected) {

   connected = ConnectWifi();

   if(!connected) {
    Serial.println("Could not connect to WIFI - will try again");
    delay(1000);
   }
    
  }

 

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();






  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);

  /* Now we can install the DMX driver! We'll tell it which DMX port to use and
    how big our DMX packet is expected to be. Typically, we'd pass it a handle
    to a queue, but since we are only transmitting DMX, we don't need a queue.
    We can write `NULL` where we'd normally put our queue handle. We'll also
    pass some interrupt priority information. The interrupt priority can be set
    to 1. */
  int queueSize = 0;
  int interruptPriority = 1;
  dmx_driver_install(dmxPort, DMX_DEFAULT_INTR_FLAGS);



datadmx[1]=255;
  
}

void loop()
{
  // we call the read function inside the loop
 



  if (WiFi.status() != WL_CONNECTED) {
        Serial.println("LOST CONNECTION");

    digitalWrite(32, LOW);

      bool new_connect = false;
    while(!new_connect){
      new_connect = ConnectWifi();
    }
    
  } else {



         dmx_write(dmxPort, datadmx, DMX_MAX_PACKET_SIZE);

    dmx_send(dmxPort, DMX_MAX_PACKET_SIZE);

    Serial.println(datadmx[1]);
         artnet.read();

   // dmx_wait_send(dmxPort, DMX_PACKET_TIMEOUT_TICK);
      digitalWrite(33, LOW);


  }




}
