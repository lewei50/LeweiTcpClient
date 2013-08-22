#include <LeweiTcpClient.h>

#define LW_USERKEY "your api key"
#define LW_GATEWAY "01"
//long starttime;
//int postInterval = 60000;

LeweiTcpClient *client;

void setup()
{
  Serial.begin(115200);//we recommended using 38400 bps(need to set the same as your wifi shield)
  //starttime = millis();
  client = new LeweiTcpClient(LW_USERKEY, LW_GATEWAY);
  UserFunction uf1(test1,"testFunction");
  client->addUserFunction(uf1);
  UserFunction uf2 (test2,"testFunction2");
  client->addUserFunction(uf2);
}

void loop()
{ 
client->keepOnline();
    
//    if ((millis()-starttime) > postInterval)
//    {
//      String val = "1000";
//      client->appendSensorValue("temperature",11);
//      you can append more sensors here,and use sendSensorValue to send
//      //client->sendSensorValue("dsm501",val);
//      starttime = millis();
//    }
 }

void test1(char * p1)
{
  client->setRevCtrlMsg("true","message to myServer");
  //client->sendSensorValue("dsm501","100");
}
//function test2 use 2 parameter,on the website,it point to "p1,p2"
void test2(char * p1,char * p2)
{
  client->setRevCtrlMsg("true","finished function2");
  //client->sendSensorValue("dsm501",101.23);
}

