
#include <LeweiTcpClient.h>

UserFunction::UserFunction(void (*callfuct)(),const char *uFunctionName)
{
	userFunctionAddr0=callfuct;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	//userFunctionAddr3=NULL;
	//userFunctionAddr4=NULL;
	//userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=callfuct;
	userFunctionAddr2=NULL;
	//userFunctionAddr3=NULL;
	//userFunctionAddr4=NULL;
	//userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=callfuct;
	//userFunctionAddr3=NULL;
	//userFunctionAddr4=NULL;
	//userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}

/*
UserFunction::UserFunction(void (*callfuct)(char*,char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	userFunctionAddr3=callfuct;
	userFunctionAddr4=NULL;
	//userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*,char*,char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	userFunctionAddr3=NULL;
	userFunctionAddr4=callfuct;
	//userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*,char*,char*,char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	userFunctionAddr3=NULL;
	userFunctionAddr4=NULL;
	userFunctionAddr5=callfuct;
	userFunctionName=uFunctionName;
	next = NULL;
}
*/


LeweiTcpClient::LeweiTcpClient(const char *userKey,const char *gatewayNo)
{
	_userKey = userKey;
	_gatewayNo = gatewayNo;
	setupDefaultValue();
	
	keepOnline();
}


void LeweiTcpClient::setupDefaultValue()
{
	head = NULL;
	
	_postInterval = 10000;//server setting is 60000
	_starttime = millis();
	
	int len=strlen(_gatewayNo)+strlen(_userKey)+51;
	aliveString=(char *)malloc(len);	
	snprintf(aliveString, len, "{\"method\":\"update\",\"gatewayNo\":\"%s\",\"userkey\":\"%s\"}&^!", _gatewayNo, _userKey);
	
	setRevCtrlMsg("false","function not bind");

}

char* LeweiTcpClient::strToChar(String str)
{
	char* c = (char*)malloc(str.length()+1);
	if(!c)
	{
		return NULL;
	}
	str.toCharArray(c,str.length()+1);
	return c;
}

void LeweiTcpClient::sendOnlineCommand()
{
		Serial.write(aliveString);
}

void LeweiTcpClient::keepOnline()
{
	getResponse();
	if ((millis()-_starttime) > _postInterval)
	{
		_starttime = millis();
			sendOnlineCommand();
	}
}

void LeweiTcpClient::getResponse()
{
  while (Serial.available()>0) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    _clientStr += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
  }
  if(_clientStr.indexOf('&^!')>0)
  {
	  if(_clientStr.length()>0)
		{
			String functionName = getParaValueStr(_clientStr,"f");
			if(!functionName.equals(""))//here comes user defined command
			{
				char* p1 = getParaValue(_clientStr,"p1");
				char* p2 = getParaValue(_clientStr,"p2");
				//char* p3 = getParaValue(_clientStr,"p3");
				//char* p4 = getParaValue(_clientStr,"p4");
				//char* p5 = getParaValue(_clientStr,"p5");
				UserFunctionNode * current = head;
				while(current!=NULL)
				{
					if(functionName.equals(current->userFunctionName))
					{
						/*
						if(p5!=NULL)
						{
							//Serial.println(5);
							execute(current->userFunctionAddr5,p1,p2,p3,p4,p5);
						}
						else 
							if(p4!=NULL)
						{
							//Serial.println(4);
							execute(current->userFunctionAddr4,p1,p2,p3,p4);
						}
						else 
							if(p3!=NULL)
						{
							//Serial.println(3);
							execute(current->userFunctionAddr3,p1,p2,p3);
						}
						else */
							if(p2!=NULL)
						{
							//Serial.println(2);
							execute(current->userFunctionAddr2,p1,p2);
						}
						else if(p1!=NULL)
						{
							//Serial.println(1);
							execute(current->userFunctionAddr1,p1);
						}
						else
						{
							//Serial.println(0);
							execute(current->userFunctionAddr0);
						}
						//execute(current->userFunctionAddr);
						break;
					}
					current = current->next;
				}
				//free(p1);free(p2);free(p3);free(p4);free(p5);
				//p1=p2=p3=p4=p5=NULL;
				free(p1);free(p2);
				p1=p2=NULL;
			}
	
			functionName = NULL;
		
			int len=strlen(_revCtrlResult)+strlen(_revCtrlMsg)+63;
			commandString=(char *)malloc(len);	
			snprintf(commandString, len, "{\"method\":\"response\",\"result\":{\"successful\":%s,\"message\":\"%s\"}}&^!", _revCtrlResult, _revCtrlMsg);
			
			Serial.print(commandString);
			free(commandString);
			commandString = NULL;
			
			setRevCtrlMsg("false","function not set");
			
			
		}
		_clientStr = "";
		_clientStr = NULL;
	}
}

char* LeweiTcpClient::getParaValue(String &orig,String paraName)
{
		int functionNameStartPos = orig.indexOf("\""+paraName+"\":\"");
		if(functionNameStartPos<0)return NULL;
		int functionNameEndPos = orig.indexOf("\"",functionNameStartPos+4+paraName.length());
		String functionValue = orig.substring(functionNameStartPos+4+paraName.length(),functionNameEndPos);
		
		char* c = strToChar(functionValue);
		return c;
}

String LeweiTcpClient::getParaValueStr(String &orig,String paraName)
{
		int functionNameStartPos = orig.indexOf("\""+paraName+"\":\"");
		if(functionNameStartPos<0)return NULL;
		int functionNameEndPos = orig.indexOf("\"",functionNameStartPos+4+paraName.length());
		String functionName = orig.substring(functionNameStartPos+4+paraName.length(),functionNameEndPos);
		
		return functionName;
}




void LeweiTcpClient::setRevCtrlMsg(char* execResult,char* msg)
{
	_revCtrlResult = execResult;
	_revCtrlMsg = msg;
}


void LeweiTcpClient::appendSensorValue(String sensorName,String sensorValue)
{
	_sensorValueStr +="{\"Name\":\"";
	_sensorValueStr +=sensorName;
	_sensorValueStr +="\",\"Value\":\"";
	_sensorValueStr +=sensorValue;
	_sensorValueStr +="\"},";
}
void LeweiTcpClient::appendSensorValue(String sensorName,int sensorValue)
{
	appendSensorValue(sensorName,String(sensorValue));
}
void LeweiTcpClient::appendSensorValue(String sensorName,float sensorValue)
{
	char s[10];
	int val_len = snprintf(s, 10,"%d.%02u", (int)sensorValue, (int)(abs(sensorValue)*100+0.5) % 100);
	appendSensorValue(sensorName,(String)s);
}

void LeweiTcpClient::appendSensorValue(String sensorName,double sensorValue)
{
	char s[10];
	int val_len = snprintf(s, 10,"%d.%02u", (int)sensorValue, (int)(abs(sensorValue)*100+0.5) % 100);
	appendSensorValue(sensorName,(String)s);
}
void LeweiTcpClient::appendSensorValue(String sensorName,long sensorValue)
{
	appendSensorValue(sensorName,(String)sensorValue);
}

void LeweiTcpClient::sendSensorValue(String sensorName,String sensorValue)
{
	
	String connStr = "{\"method\": \"upload\", \"data\":[";
	connStr+=_sensorValueStr;
	connStr+="{\"Name\":\"";
	connStr+=sensorName;
	connStr+="\",\"Value\":\"";
	connStr+=sensorValue;
	connStr+="\"}]}&^!";
	Serial.print(connStr);
	
	connStr = NULL;
	_sensorValueStr = "";
}



void LeweiTcpClient::sendSensorValue(String sensorName,int sensorValue)
{
	sendSensorValue(sensorName,String(sensorValue));
}


void LeweiTcpClient::sendSensorValue(String sensorName,float sensorValue)
{
	char s[10];
	int val_len = snprintf(s, 10,"%d.%02u", (int)sensorValue, (int)(abs(sensorValue)*100+0.5) % 100);
	sendSensorValue(sensorName,(String)s);
	
}

void LeweiTcpClient::sendSensorValue(String sensorName,double sensorValue)
{
	char s[10];
	int val_len = snprintf(s, 10,"%d.%02u", (int)sensorValue, (int)(abs(sensorValue)*100+0.5) % 100);
	sendSensorValue(sensorName,(String)s);
	
}
void LeweiTcpClient::sendSensorValue(String sensorName,long sensorValue)
{
	sendSensorValue(sensorName,(String)sensorValue);
	
}


void LeweiTcpClient::execute(void (*callfuct)())
{
	//Serial.println("exec no para");
    callfuct();
}
void LeweiTcpClient::execute(void (*callfuct)(char*),char* p1)
{
    callfuct(p1);
}
void LeweiTcpClient::execute(void (*callfuct)(char*,char*),char* p1,char* p2)
{
    callfuct(p1,p2);
}

/*
void LeweiTcpClient::execute(void (*callfuct)(char*,char*,char*),char* p1,char* p2,char* p3)
{
    callfuct(p1,p2,p3);
}

void LeweiTcpClient::execute(void (*callfuct)(char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4)
{
    callfuct(p1,p2,p3,p4);
}
void LeweiTcpClient::execute(void (*callfuct)(char*,char*,char*,char*,char*),char* p1,char* p2,char* p3,char* p4,char* p5)
{
    callfuct(p1,p2,p3,p4,p5);
}
*/


void LeweiTcpClient::addUserFunction(UserFunction &uFunction)
{
	UserFunctionNode *n1,*n2;
	n2 = (UserFunctionNode*) new(UserFunctionNode);
	
	/*
	if(uFunction.userFunctionAddr5!=NULL)
	{
		//Serial.println("reg addr5.");
		n2->userFunctionAddr5 = uFunction.userFunctionAddr5;
	}
	else 
		if(uFunction.userFunctionAddr4!=NULL)
	{
		//Serial.println("reg addr4.");
		n2->userFunctionAddr4 = uFunction.userFunctionAddr4;
	}
	else 
		if(uFunction.userFunctionAddr3!=NULL)
	{
		//Serial.println("reg addr3.");
		n2->userFunctionAddr3 = uFunction.userFunctionAddr3;
	}
	else */
		if(uFunction.userFunctionAddr2!=NULL)
	{
		//Serial.println("reg addr2.");
		n2->userFunctionAddr2 = uFunction.userFunctionAddr2;
	}
	else if(uFunction.userFunctionAddr1!=NULL)
	{
		//Serial.println("reg addr1.");
		n2->userFunctionAddr1 = uFunction.userFunctionAddr1;
	}
	else if(uFunction.userFunctionAddr0!=NULL)
	{
		//Serial.println("reg addr0.");
		n2->userFunctionAddr0 = uFunction.userFunctionAddr0;
	}
	
	n2->userFunctionName = uFunction.userFunctionName;
		//Serial.println(uFunction.userFunctionName);
	if(head==NULL)
	{
		head = n2;
		n2->next = NULL;
	}
	else
	{
		n1 = head;
		while(n1->next != NULL)
		{
			n1= n1->next;
		}
		n1->next = n2;
		n2->next = NULL;
	}
}

