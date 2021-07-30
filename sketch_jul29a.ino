#include <DS18B20.h>
#include <UIPEthernet.h>

byte mac[] = { 0xDE, 0x05, 0xB6, 0x27, 0x39, 0x19 }; // random MAC

IPAddress ip(172, 16, 0, 70);
IPAddress dnServer(172, 16, 0, 11);
IPAddress gateway(172, 16, 0, 1);
IPAddress subnet(255, 255, 255, 128);

DS18B20 ds(4); // DS18B20 at pin 4
EthernetServer server(10050); // Zabbix port
EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop()
{
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) {
    //Serial.println(ds.getTempC());
    String c = client.readString();
    client.print("ZBXD\x01");
    
    if(c.endsWith("agent.ping") == 1){
      byte responseBytes [] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '1'}; 
      client.write(responseBytes, 9);  
    }else if(c.endsWith("env.temp") == 1){
      byte responseBytes [] = {(byte) String(ds.getTempC()).length(), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      client.write(responseBytes, 8);
      client.print(ds.getTempC());
      Serial.println(ds.getTempC());
    }else {
      byte responseBytes [] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      client.write(responseBytes, 8);
      client.print("ZBX_NOTSUPPORTED");
    }
  }

  delay(10);
}
