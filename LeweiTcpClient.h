/*
normal version,1.0,use for arduino mega 2560 with EasySetupMode
*/

#ifndef LeweiTcpClient_h
#define LeweiTcpClient_h

#include <Ethernet.h>
#include <EEPROM.h>

/*
LeweiTcpClient.h Library for tcp.lewei50.com to reverse control and upload data
Create by yangbo
gyangbo@gmail.com
2013.6.4
*/

//this structure is to hold the user's function's address and the name configured on the website
struct UserFunctionNode
{
	void (*userFunctionAddr0)();
	void (*userFunctionAddr1)(char*);
	void (*userFunctionAddr2)(char*,char*);
	void (*userFunctionAddr3)(char*,char*,char*);
	void (*userFunctionAddr4)(char*,char*,char*,char*);
	void (*userFunctionAddr5)(char*,char*,char*,char*,char*);
	const char *userFunctionName;
	UserFunctionNode*next;
};

class UserFunction
{
	public:
		UserFunction(void (*callfuct)(),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*,char*),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*,char*,char*),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*,char*,char*,char*),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*,char*,char*,char*,char*),const char *userFunctionName);
		void (*userFunctionAddr0)();
		void (*userFunctionAddr1)(char*);
		void (*userFunctionAddr2)(char*,char*);
		void (*userFunctionAddr3)(char*,char*,char*);
		void (*userFunctionAddr4)(char*,char*,char*,char*);
		void (*userFunctionAddr5)(char*,char*,char*,char*,char*);
		const char *userFunctionName;
	private:
		UserFunction *next;
		friend class LeweiTcpClient;
};


struct UserSwitchNode
{
	//void (*userSwitchOnFunctionAddr)();
	//void (*userSwitchOffFunctionAddr)();
	void (*userSwitchFunctionAddr)(char*);
	char * p1;
	const char *userSwitchId;
	boolean userSwitchState;
	UserSwitchNode*next;
};

class UserSwitch
{
	public:
		UserSwitch(void (*uSwitchFunctionAddr)(char*),const char *uSwtichId,boolean uSwitchState);
		//void (*userSwitchOnFunctionAddr)();
		//void (*userSwitchOffFunctionAddr)();
		void (*userSwitchFunctionAddr)(char*);
		char * p1;
		const char *userSwitchId;
		boolean userSwitchState;
	private:
		UserSwitch *next;
		friend class LeweiTcpClient;
};


class LeweiTcpClient
{
	UserFunctionNode*head;
	UserSwitchNode*switchHead;
	
	public:
		char* tcpServer;
		char * aliveString;
		char * commandString;
		LeweiTcpClient( const char *userKey,const char *gatewayNo);
		//LeweiTcpClient( const char *userKey,const char *gatewayNo,byte mac[]);
		LeweiTcpClient( const char *userKey,const char *gatewayNo,byte mac[],IPAddress ip,IPAddress dns,IPAddress gw,IPAddress subnet);
		void keepOnline();
		void appendSensorValue(String sensorName,String sensorValue);
		void appendSensorValue(String sensorName,int sensorValue);
		void appendSensorValue(String sensorName,float sensorValue);
		void appendSensorValue(String sensorName,double sensorValue);
		void appendSensorValue(String sensorName,long sensorValue);
		void sendSensorValue(String sensorName,String sensorValue);
		void sendSensorValue(String sensorName,int sensorValue);
		void sendSensorValue(String sensorName,float sensorValue);
		void sendSensorValue(String sensorName,double sensorValue);
		void sendSensorValue(String sensorName,long sensorValue);
		void connentTcpServer();
		void execute(void (*callfuct)());
		void execute(void (*callfuct)(char*),char* p1);
		void execute(void (*callfuct)(char*,char*),char* p1,char* p2);
		void execute(void (*callfuct)(char*,char*,char*),char* p1,char* p2,char* p3);
		void execute(void (*callfuct)(char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4);
		void execute(void (*callfuct)(char*,char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4,char* p5);

		void executeStrCmd(String cmdStr);

		void addUserFunction(UserFunction &uFunction);
		void setRevCtrlMsg(char* execResult,char* msg);
		void setRevCtrlData(char* data);
		void directResponse(String respStr);
		
		
		void addUserSwitch(UserSwitch &uSwitch);
		
		char* strToChar(String str);
		
		void checkFreeMem();
		void easySetupMode(boolean bEasyMode);

	private:
		char *_userKey;
		char *_gatewayNo;
		byte _mac[];
		String _clientStr;
		long _starttime;
		int _postInterval;
		String _sensorValueStr;
		EthernetClient _clientRevCtrl;//connect to tcp.lewei50.com and keep alive
		
		void sendOnlineCommand();
		void getResponse();
		void setupDefaultValue();
		char* getParaValue(String &orig,String paraName);
		String getParaValueStr(String &orig,String paraName);
		
		char* _revCtrlResult;
		char* _revCtrlMsg;
		char* _revCtrlData;
		
		void sendUserSwitchState();
		void updateUserSwitchState(char* p1,char * p2);
		
		boolean _bEasyMode;
		void writeRom(String value);
		void readRom();
		void listenServer();
		void softwareReset();

		EthernetServer server;
};

#endif
