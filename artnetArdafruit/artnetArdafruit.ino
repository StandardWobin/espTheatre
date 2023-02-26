#include <ArtnetWifi.h>
#include <Adafruit_NeoPixel.h>

#define PIN        5
#define NUMPIXELS 16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 200


const char* ssid = "X-X";
const char* password = "X";


ArtnetWifi artnet;



void setup() {

  Serial.begin(115200);

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

  
  pixels.begin();
}

void loop() {


  


  if (WiFi.status() != WL_CONNECTED) {
        Serial.println("LOST CONNECTION");

    digitalWrite(32, LOW);

      bool new_connect = false;
    while(!new_connect){
      new_connect = ConnectWifi();
    }
    
  } else {



        artnet.read();
        
        
      


  



  }






}


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


    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to " );
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(IPAddress(WiFi.localIP()));



  } else {
    Serial.println("");
    Serial.println("Connection failed.");



  }
  
  return state;
}




void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* dataart)
{


  pixels.clear();

  bool debug = false;

  bool tail = false;

  // copy from ARTNET to ESP32 DMX Interface
  


    for(int i = 0; i < NUMPIXELS; i++) {

          pixels.setPixelColor(i, pixels.Color((int)dataart[i*3], (int)dataart[(i*3)+1], (int)dataart[(i*3)+2]));

    }


    pixels.show();




}
