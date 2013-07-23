#include <SPI.h>
#include <Ethernet.h>
#include <LeweiTcpClient.h>
#include <EEPROM.h>


#define LW_USERKEY "yourapikey"
#define LW_GATEWAY "01"

#define LED_PIN 3//plug led in the d3,gnd->gnd


//byte mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
//IPAddress ip(192,168,1, 15);
//IPAddress mydns(8,8,8,8);
//IPAddress gw(192,168,1,1);
//IPAddress subnet(255,255,255,0);
 
LeweiTcpClient *client;

void setup()
{
  Serial.begin(9600);
  //you can use 3 ways to init your network
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
  UserFunction uf2 (ledOn,"turnLedOn");
  client->addUserFunction(uf2);
  UserFunction uf3 (ledOff,"turnLedOff");
  client->addUserFunction(uf3);
  
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
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    client->keepOnline();
}


//function functionIWrote use 1 parameter,on the website,it point to "p1"
void functionIWrote(char * p1)
{
  client->setRevCtrlMsg("true","message to server");
//  client->sendSensorValue("tcp1",pi);
  Serial.println(p1);
}


//function without parameter
void ledOn()
{
  client->setRevCtrlMsg("true","on");
  digitalWrite(LED_PIN,HIGH);
}
void ledOff()
{
  client->setRevCtrlMsg("true","off");
  digitalWrite(LED_PIN,LOW);
}