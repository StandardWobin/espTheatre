#include "sbus.h"
#include <ArtnetWifi.h>
#define ONBOARD_LED 1

ArtnetWifi artnet;

const char *ssid = "*";
const char *password = "*";

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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *dataart)
{

  digitalWrite(33, HIGH);
  bool debug = false;
  bool tail = false;

  // copy from ARTNET to ESP32 DMX Interface

  data.ch[0] = map((int)dataart[20], 0, 255, 352, 1696);
  data.ch[1] = map((int)dataart[21], 0, 255, 352, 1696);
  data.ch[3] = map((int)dataart[22], 0, 255, 352, 1696);
  data.ch[4] = map((int)dataart[23], 0, 255, 0, 2500);
  data.ch[5] = map((int)dataart[24], 0, 255, 0, 1000);
  data.ch[6] = map((int)dataart[25], 0, 255, 0, 2500);

  if (length > 16)
  {
    length = 16;
    tail = true;
  }
  // send out the buffer
  for (int i = 0; i < length; i++)
  {
    // Serial.print(dataart[i], HEX);
    // Serial.print(" ");
  }
  if (tail)
  {
    // Serial.print("...");
  }

  for (int i = 0; i < length; i++)
  {
  }
  if (tail)
  {
  }
}

void setup()
{
  /* Serial to display data */
  Serial.begin(115200);
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, HIGH);

  while (!Serial)
  {
  }
  /* Begin the SBUS communication */
  sbus_tx.Begin();

  bool connected = false;

  while (!connected)
  {

    connected = ConnectWifi();

    if (!connected)
    {
      // Serial.println("Could not connect to WIFI - will try again");
      delay(1000);
    }
  }

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    // Serial.println("LOST CONNECTION");

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

    // write sbus
    sbus_tx.data(data);
    sbus_tx.Write();
  }
}
