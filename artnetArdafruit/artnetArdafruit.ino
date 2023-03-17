#include <ArtnetWifi.h>
#include <Adafruit_NeoPixel.h>

// //////////////////////////////////////
// CHANGE STUFF HERE
// //////////////////////////////////////
// OUTPUT PIN
#define PIN 5
#define NUMPIXELS 300
bool debug = false;
int variant = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "*";
const char *password = "*";

ArtnetWifi artnet;

void setup()
{
  Serial.begin(115200);

  bool connected = false;

  while (!connected)
  {
    connected = ConnectWifi();
    if (!connected)
    {
      Serial.println("Could not connect to WIFI - will try again");
      delay(1000);
    }
  }
  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
  pixels.begin();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("LOST CONNECTION");
    digitalWrite(32, LOW);
    bool new_connect = false;
    while (!new_connect)
    {
      new_connect = ConnectWifi();
    }
  }
  else
  {
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
  Serial.print("Try connected to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(IPAddress(WiFi.localIP()));
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *dataart)
{
  pixels.clear();
  bool tail = false;
  // copy from ARTNET to ESP32 DMX Interface

  if (variant == 0)
  {
    // VARIATN 1. Control indivdually GET VALUE FROMT DMX AND WRITE IT TO EVERY PIXEL
    for (int i = 0; i < NUMPIXELS; i++)
    {
      // when we have more then 169 Pixel we can not adress them indivedually with an 512 DMX. This is the protection;
      int artNetCap = min(i, 169);
      pixels.setPixelColor(i, pixels.Color((int)dataart[artNetCap * 3], (int)dataart[(artNetCap * 3) + 1], (int)dataart[(artNetCap * 3) + 2]));
    }
  }
  else if (variant == 1)
  {
    // VARIATN 2. The Complete strep is controlled by DMX 15 16 17
    for (int i = 0; i < NUMPIXELS; i++)
    {
      int artNetCap = 0;
      pixels.setPixelColor(i, pixels.Color((int)dataart[artNetCap * 3], (int)dataart[(artNetCap * 3) + 1], (int)dataart[(artNetCap * 3) + 2]));
    }
  }

  else if (variant == 3)
  {
    // VARIATN 2. The Complete strep is controlled by DMX 15 16 17
    for (int i = 15; i < NUMPIXELS; i++)
    {
      int artNetCap = min(i, 169);
      pixels.setPixelColor(i - 15, pixels.Color((int)dataart[artNetCap * 3], (int)dataart[(artNetCap * 3) + 1], (int)dataart[(artNetCap * 3) + 2]));
    }
  }

  pixels.show();
}
