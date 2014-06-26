#include <SPI.h>
#include <Ethernet.h>
#include <LeweiTcpClient.h>
#include <EEPROM.h>

#include <dht11.h>

#define LW_USERKEY "yourapikey"
#define LW_GATEWAY "01"


dht11 DHT11;
//DHT11 vcc pin->+5v
//DHT11 data pin->d2
//DHT11 gnd pin->gnd
#define DHT11PIN 2

//byte mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
//IPAddress ip(192,168,1, 15);
//IPAddress mydns(8,8,8,8);
//IPAddress gw(192,168,1,1);
//IPAddress subnet(255,255,255,0);
 
LeweiTcpClient *client;

long starttime;
int postInterval = 10000;
void setup()
{
  Serial.begin(9600);
  //you can use 2 ways to init your network
  //1.simplest
  client = new LeweiTcpClient(LW_USERKEY, LW_GATEWAY);
  //2.full setting for your network
  //client = new LeweiTcpClient(LW_USERKEY, LW_GATEWAY,mac,ip,mydns,gw,subnet);
  
  
  //test1 is the function you write below to handle your program
  //"testFunction" is the function name you set on website:http://www.lewei50.com/
  //on "controll command manager" menu
  //we test transfer 1-5 para from website(you defined on it) to your arduino
  //enjoy it
  
  UserFunction uf1(functionIWrote,"functionNameDefinedOnWeb");
  client->addUserFunction(uf1);
//  UserFunction uf2 (test2,"getAllSensors");
//  client->addUserFunction(uf2);
  
  /*
  enable easySetupMode will open the port 80 of this board,
  you can setup your apikey and gateway number via http://your_arduino_ip/
  the key and number will be burned into board's eeprom
  after setting you need to restart your arduino.
  after reboot,you can comment this line for fasten your board.
  this library will will use apikey and gateway number in eeprom FIRST,if it exsit.
  you can send empty value from the browser to wipe the data in eeprom.
  */
  //client->easySetupMode(true);
  starttime = millis();
}

void loop()
{
    client->keepOnline();
    if ((millis()-starttime) > postInterval)
    {
      sendDht11Value();
      starttime = millis();
    }
}


//function functionIWrote use 1 parameter,on the website,it point to "p1"
void functionIWrote(char * p1)
{
  client->setRevCtrlMsg("true","message to server");
//  client->sendSensorValue("tcp1",pi);
  Serial.println(p1);
}


//function without parameter
void test2()
{
  client->setRevCtrlMsg("true","test");
}

void sendDht11Value()
{
  //read DHT11 sensor
      int chk = DHT11.read(DHT11PIN);
      switch (chk)
      {
        case DHTLIB_OK: 
                    //Serial.print("*** start data collection ");
                    //Serial.println(DHT11.temperature);
                    //sending data
                    //you need to setup sensors named "temperature" and "humidity" on the web before you store the value.
                    client->appendSensorValue("temperature",(String)DHT11.temperature);
                    //you can append more sensors here,and use sendSensorValue to send
                    client->sendSensorValue("humidity",(String)DHT11.humidity);
                    break;
                    /*
        case DHTLIB_ERROR_CHECKSUM: 
                    Serial.println("Checksum error"); 
                    break;
        case DHTLIB_ERROR_TIMEOUT: 
                    Serial.println("Time out error,check your pin"); 
                    break;
        default: 
                    Serial.println("Unknown error"); 
                    break;
                    */
      }
}
