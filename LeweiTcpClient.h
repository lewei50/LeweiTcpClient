/*
Serial-wifi version,1.0,use for arduino uno(and others) attached with serial-wifi shield
*/

#ifndef LeweiTcpClient_h
#define LeweiTcpClient_h
#include <Arduino.h>
#include <WString.h>

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
	//void (*userFunctionAddr3)(char*,char*,char*);
	//void (*userFunctionAddr4)(char*,char*,char*,char*);
	//void (*userFunctionAddr5)(char*,char*,char*,char*,char*);
	const char *userFunctionName;
	UserFunctionNode*next;
};

class UserFunction
{
	public:
		UserFunction(void (*callfuct)(),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*),const char *userFunctionName);
		UserFunction(void (*callfuct)(char*,char*),const char *userFunctionName);
		//UserFunction(void (*callfuct)(char*,char*,char*),const char *userFunctionName);
		//UserFunction(void (*callfuct)(char*,char*,char*,char*),const char *userFunctionName);
		//UserFunction(void (*callfuct)(char*,char*,char*,char*,char*),const char *userFunctionName);
		void (*userFunctionAddr0)();
		void (*userFunctionAddr1)(char*);
		void (*userFunctionAddr2)(char*,char*);
		//void (*userFunctionAddr3)(char*,char*,char*);
		//void (*userFunctionAddr4)(char*,char*,char*,char*);
		//void (*userFunctionAddr5)(char*,char*,char*,char*,char*);
		const char *userFunctionName;
	private:
		UserFunction *next;
		friend class LeweiTcpClient;
};


class LeweiTcpClient
{
	UserFunctionNode*head;
	public:
		char * aliveString;
		char * commandString;
		LeweiTcpClient( const char *userKey,const char *gatewayNo);
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
		//void connentTcpServer();
		void execute(void (*callfuct)());
		void execute(void (*callfuct)(char*),char* p1);
		void execute(void (*callfuct)(char*,char*),char* p1,char* p2);
		//void execute(void (*callfuct)(char*,char*,char*),char* p1,char* p2,char* p3);
		//void execute(void (*callfuct)(char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4);
		//void execute(void (*callfuct)(char*,char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4,char* p5);


		void addUserFunction(UserFunction &uFunction);
		void setRevCtrlMsg(char* execResult,char* msg);
		char* strToChar(String str);

	private:
		const char *_userKey;
		const char *_gatewayNo;
		String _clientStr;
		long _starttime;
		int _postInterval;
		String _sensorValueStr;
		
		int _recieveTimeout ;
		long _recieveStartTime;
		
		void sendOnlineCommand();
		void getResponse();
		void setupDefaultValue();
		char* getParaValue(String &orig,String paraName);
		String getParaValueStr(String &orig,String paraName);
		
		char* _revCtrlResult;
		char* _revCtrlMsg;
};

#endif
