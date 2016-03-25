#include <SPI.h>
#include <Ethernet.h>
#include <LeweiTcpClient.h>
#include <EEPROM.h>

byte mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};

IPAddress ip(192,168,2, 15);
IPAddress mydns(8,8,8,8);
IPAddress gw(192,168,2,1);
IPAddress subnet(255,255,255,0);
#define LW_USERKEY "your_api_key"
#define LW_GATEWAY "01"
int relayPin = 13;
LeweiTcpClient *client;
void setup()
{
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT); 
  //you can use 2 ways to init your network
  //1.simplest
  //client = new LeweiTcpClient(LW_USERKEY, LW_GATEWAY);
  //2.full setting for your network
  client = new LeweiTcpClient(LW_USERKEY, LW_GATEWAY,mac,ip,mydns,gw,subnet);
  
  
  //test1 is the function you write below to handle your program
  //"testFunction" is the function name you set on website:http://www.lewei50.com/
  //on "controll command manager" menu
  //we test transfer 1-5 para from website(you defined on it) to your arduino
  //enjoy it
  
  //3 para means changeFunction,function name defined on web,default state
  UserSwitch us1 (test1,"switch01",0);
  client->addUserSwitch(us1);
  
  /*
  enable easySetupMode will open the port 80 of this board,
  you can setup your apikey and gateway number via http://your_arduino_ip/
  the key and number will be burned into board's eeprom
  after set you need to restart your arduino.
  after setup,you can comment this line for fasten your board.
  this library will will use apikey and gateway number in eeprom if you enable easySetupMode .
  you can send empty value from the browser to wipe the data in eeprom.
  */
  //client->easySetupMode(true);
  
}

void loop()
{
    client->keepOnline();
}


//function test1 use 1 parameter,on the website,it point to "p1"
void test1(char * p1)
{
  client->setRevCtrlMsg("true","message to server");
  Serial.println("test function recall");
//  double pi = 3241.59;//*random(10);
//  client->sendSensorValue("tcp1",pi);
  Serial.println(p1);
  if(String(p1).equals("0"))
	{
		digitalWrite(relayPin,LOW);
		Serial.println("on");
	}
	else if(String(p1).equals("1"))
	{
		digitalWrite(relayPin,HIGH);
		Serial.println("off");
	}
  client->checkFreeMem();
}
