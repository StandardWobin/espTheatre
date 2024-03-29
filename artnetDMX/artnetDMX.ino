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

// Wifi settings
const char *ssid = "*";
const char *password = "*";

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
  Serial.print("Connecting");
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
  bool tail = false;

  Serial.print("DMX: Univ: ");
  Serial.print(universe, DEC);
  Serial.print(", Seq: ");
  Serial.print(sequence, DEC);
  Serial.print(", Data (");
  Serial.print(length, DEC);
  Serial.print("): ");
  for (int i = 0; i < 512; i++)
  {

    datadmx[i] = (int)dataart[i];
  }

  if (length > 16)
  {
    length = 16;
    tail = true;
  }
  // send out the buffer
  for (int i = 0; i < length; i++)
  {
    Serial.print(dataart[i], HEX);
    Serial.print(" ");
  }
  if (tail)
  {
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
  if (tail)
  {
    Serial.print("...");
  }
  Serial.println();

  dmx_write_packet(dmxPort, datadmx, DMX_MAX_PACKET_SIZE);
  dmx_send_packet(dmxPort, DMX_MAX_PACKET_SIZE);
  dmx_wait_send_done(dmxPort, DMX_PACKET_TIMEOUT_TICK);
}

void setup()
{
  // set-up serial for debug output
  Serial.begin(115200);
  ConnectWifi();

  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();


  dmx_config_t dmxConfig = DMX_DEFAULT_CONFIG;
  dmx_param_config(dmxPort, &dmxConfig);
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);

  int queueSize = 0;
  int interruptPriority = 1;
  dmx_driver_install(dmxPort, DMX_MAX_PACKET_SIZE, queueSize, NULL,
                     interruptPriority);

  dmx_set_mode(dmxPort, DMX_MODE_WRITE);

  datadmx[1] = 255;
}

void loop()
{
  artnet.read();
}
