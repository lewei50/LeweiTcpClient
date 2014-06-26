#include <SPI.h>
#include <Ethernet.h>
#include <LeweiTcpClient.h>
#include <EEPROM.h>
#include <IRremoteLite.h> 

#define LW_USERKEY "yourapikey"
#define LW_GATEWAY "01"

IRsend irsend; 

//use PROGMEM to defined the 'large' variables into flash,not using the ram
PROGMEM unsigned int rawCodes_ac_open[200] = {4397,4368,561,1578,560,537,557,1581,584,1582,584,486,558,537,584,1554,560,538,558,510,558,1607,557,513,585,511,556,1585,581,1583,583,486,557,1609,556,513,558,538,557,512,558,1607,558,1583,557,1608,558,1608,557,1580,559,1606,559,1581,556,1609,557,512,559,537,558,512,557,539,557,512,558,1607,558,1582,558,537,584,1555,558,1607,556,514,559,537,556,513,558,537,556,514,558,1606,559,511,583,513,558,1582,557,1608,558,1580,585,5154,4419,4347,584,1555,558,539,557,1581,584,1581,571,501,577,516,571,1568,557,541,581,487,557,1608,557,513,558,538,557,1582,583,1582,584,487,557,1608,557,511,611,486,558,510,558,1607,557,1609,556,1583,583,1582,583,1555,558,1607,558,1607,559,1582,557,537,585,486,558,510,558,540,558,509,557,1608,558,1608,557,512,559,1606,559,1581,558,537,581,490,558,510,587,511,557,511,559,1606,559,510,569,527,558,1582,560,1604,561,1605,561};
PROGMEM unsigned int rawCodes_ac_close[200] ={4193,4096,595,1451,589,432,588,1453,589,1454,587,432,587,434,563,1475,591,433,564,458,586,1453,587,434,587,433,563,1476,590,1454,587,434,565,1475,566,456,563,1476,590,1453,587,1452,564,1477,566,457,585,1453,567,1477,587,1452,564,460,584,434,564,457,562,460,562,1475,587,434,588,433,562,1476,589,1455,563,1476,588,433,586,436,562,459,586,433,563,458,609,410,563,458,586,433,587,1454,587,1453,563,1477,564,1477,565,1476,581,4858,4237,4074,593,1453,562,460,563,1477,563,1477,565,458,587,434,584,1454,590,433,562,458,587,1453,588,436,584,436,562,1476,588,1453,590,434,562,1480,586,435,586,1453,565,1476,589,1453,589,1451,565,461,583,1453,589,1453,588,1454,564,457,587,434,587,433,587,435,561,1479,586,435,562,458,588,1452,590,1452,565,1476,564,458,587,436,584,433,562,458,588,432,562,461,585,434,584,435,589,1451,588,1453,588,1453,590,1453,590,1451,590};

//byte mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
//IPAddress ip(192,168,1, 15);
//IPAddress mydns(8,8,8,8);
//IPAddress gw(192,168,1,1);
//IPAddress subnet(255,255,255,0);
 
LeweiTcpClient *client;

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT); 
  
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
  
  UserFunction uf1(openAc,"openMyAC");
  client->addUserFunction(uf1);
  UserFunction uf2 (closeAc,"closeMyAc");
  client->addUserFunction(uf2);
  //client->easySetupMode(true);
}

void loop()
{
    client->keepOnline();
}


//function functionIWrote use 1 parameter,on the website,it point to "p1"
void functionIWrote(char * p1)
{
  client->setRevCtrlMsg("true","ok");
  Serial.println(p1);
}


//function without parameter
void openAc(char* p1)
{
  client->setRevCtrlMsg("true","o");
  client->checkFreeMem();
  irsend.sendRaw(rawCodes_ac_open,200, 38);  
}

void closeAc(char* p1)
{
  client->setRevCtrlMsg("true","c");
  client->checkFreeMem();
  irsend.sendRaw(rawCodes_ac_close,200, 38);   
}




