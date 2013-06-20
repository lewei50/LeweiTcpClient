
#include <LeweiTcpClient.h>

UserFunction::UserFunction(void (*callfuct)(),const char *uFunctionName)
{
	userFunctionAddr0=callfuct;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	userFunctionAddr3=NULL;
	userFunctionAddr4=NULL;
	userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=callfuct;
	userFunctionAddr2=NULL;
	userFunctionAddr3=NULL;
	userFunctionAddr4=NULL;
	userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}
UserFunction::UserFunction(void (*callfuct)(char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=callfuct;
	userFunctionAddr3=NULL;
	userFunctionAddr4=NULL;
	userFunctionAddr5=NULL;
	userFunctionName=uFunctionName;
	next = NULL;
}


UserFunction::UserFunction(void (*callfuct)(char*,char*,char*),const char *uFunctionName)
{
	userFunctionAddr0=NULL;
	userFunctionAddr1=NULL;
	userFunctionAddr2=NULL;
	userFunctionAddr3=callfuct;
	userFunctionAddr4=NULL;
	userFunctionAddr5=NULL;
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
	userFunctionAddr5=NULL;
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



LeweiTcpClient::LeweiTcpClient(const char *userKey,const char *gatewayNo)
{
	_userKey = userKey;
	_gatewayNo = gatewayNo;
	setupDefaultValue();
	//IPAddress tcpServer(42,121,128,216); //tcp.lewei50.com's ip
	//IPAddress uploadServer(42,121,128,216);// "open.lewei50.com";
	
	Ethernet.begin(_mac);
	Serial.println(Ethernet.localIP());
	//Ethernet.begin(mac, ip);
	delay(1000);
	String clientStr="";
	//_postInterval = 5000;
	//_starttime = millis();
	keepOnline();
}

LeweiTcpClient::LeweiTcpClient(const char *userKey,const char *gatewayNo,byte mac[])
{
	_userKey = userKey;
	_gatewayNo = gatewayNo;
	setupDefaultValue();
	
	Ethernet.begin(mac);
	Serial.println(Ethernet.localIP());
	delay(1000);
	
	String clientStr="";
	keepOnline();
}


LeweiTcpClient::LeweiTcpClient( const char *userKey,const char *gatewayNo,byte mac[],IPAddress ip,IPAddress dns,IPAddress gw,IPAddress subnet)
{
	_userKey = userKey;
	_gatewayNo = gatewayNo;
	setupDefaultValue();
	Ethernet.begin(mac,ip,dns,gw,subnet);
	Serial.println(Ethernet.localIP());
	delay(1000);
	String clientStr="";
	keepOnline();
}

void LeweiTcpClient::setupDefaultValue()
{
	head = NULL;
	//tcpServer = IPAddress(42,121,128,216); //tcp.lewei50.com's ip
	//uploadServer = IPAddress(121,197,10,140);// "open.lewei50.com";
	//char tcpServer[] = "tcp.lewei50.com";
	//char uploadServer[] = "open.lewei50.com";
	
	byte _mac[] = {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};
	_postInterval = 10000;//server setting is 60000
	_starttime = millis();
	String tcpServerStr = "tcp.lewei50.com";
	tcpServerStr.toCharArray(tcpServer,16);
	tcpServerStr = NULL;
	String uploadServerStr = "open.lewei50.com";
	uploadServerStr.toCharArray(uploadServer,17);
	uploadServerStr = NULL;
	bIsConnecting = false;
	
	int len=strlen(_gatewayNo)+strlen(_userKey)+51;
	aliveString=(char *)malloc(len);	
	snprintf(aliveString, len, "{\"method\":\"update\",\"gatewayNo\":\"%s\",\"userkey\":\"%s\"}&^!", _gatewayNo, _userKey);
	

	setRevCtrlMsg("false","function not binded");

}

char* LeweiTcpClient::strToChar(String str)
{
	char* c = (char*)malloc(str.length()+1);
	if(!c)
	{
		Serial.print("strToChar::");
		Serial.println(str);
		Serial.println("strToChar::malloc failed");
		return NULL;
	}
	str.toCharArray(c,str.length()+1);
	return c;
}

void LeweiTcpClient::sendOnlineCommand()
{
		Serial.print("::sendOnlineCommand.current net state:");
		Serial.println(_clientRevCtrl.status());
		/*
		_clientRevCtrl.print("{\"method\":\"update\",\"gatewayNo\":\""+String(_gatewayNo)+"\",\"userkey\":\""+String(_userKey)+"\"}&^!");
		delay(1000);
		*/
		
		_clientRevCtrl.print(aliveString);
		/*
		
		String connStr = "{\"method\":\"update\",\"gatewayNo\":\""+String(_gatewayNo)+"\",\"userkey\":\""+String(_userKey)+"\"}&^!";
		char* c = strToChar(connStr);
//		char* c = (char*)malloc(connStr.length()+1);
//		if(!c)Serial.println("sendOnlineCommand::malloc failed");
//		else Serial.println("sendOnlineCommand::malloc success");
//		connStr.toCharArray(c,connStr.length()+1);
		if(c)
		{
			_clientRevCtrl.print(c);
		}
			free(c);
			c = NULL;
		connStr = "";
		*/
		
}

void LeweiTcpClient::keepOnline()
{
	getResponse();
		if (_clientRevCtrl.connected()) 
		{
			if ((millis()-_starttime) > _postInterval)
			{
				_starttime = millis();
					sendOnlineCommand();
			}
		}
		else
		{
			if(!bIsConnecting)
			{
				connentTcpServer();
			}
		}
		/*
		if (_clientRevCtrl.connected()) 
		{
		}
		else
		{
			Serial.println("keepOnline::disconnected...reconnect");
			Serial.println(_clientRevCtrl.status());
			delay(1000);
			connentTcpServer();
		}
		*/
}

void LeweiTcpClient::getResponse()
{
	if (_clientRevCtrl.available())
	{
		char c = _clientRevCtrl.read();
		_clientStr += c;
	}
	else if(_clientStr.length()>0)
	{
		//char* retMessage = "function not binded";
		Serial.print("message from server:");
		Serial.println(_clientStr);
		checkFreeMem();
		String functionName = getParaValueStr(_clientStr,"f");
			char* p1 = getParaValue(_clientStr,"p1");
			char* p2 = getParaValue(_clientStr,"p2");
			char* p3 = getParaValue(_clientStr,"p3");
			char* p4 = getParaValue(_clientStr,"p4");
			char* p5 = getParaValue(_clientStr,"p5");
			_clientStr = NULL;
		checkFreeMem();
		if(!functionName.equals(""))//here comes user defined command
		{
  		Serial.println("functionName is:");
			Serial.println(functionName);
			//countUserFunction();
			UserFunctionNode * current = head;
			while(current!=NULL)
			{
//				Serial.println(functionName);
//				Serial.println(current->userFunctionName);
				if(functionName.equals(current->userFunctionName))
				{
					setRevCtrlMsg("true","execute function");
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
					else 
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
		}
  		Serial.println("4");
			free(p1);free(p2);free(p3);free(p4);free(p5);
			p1=p2=p3=p4=p5=NULL;
			//free(p1);free(p2);
			//p1=p2=NULL;

			functionName = NULL;
		
			int len=strlen(_revCtrlResult)+strlen(_revCtrlMsg)+63;
			//Serial.println(len);
			commandString=(char *)malloc(len);	
			snprintf(commandString, len, "{\"method\":\"response\",\"result\":{\"successful\":%s,\"message\":\"%s\"}}&^!", _revCtrlResult, _revCtrlMsg);
			//Serial.println(commandString);
			_clientRevCtrl.print(commandString);
			free(commandString);
			commandString = NULL;
	/*
		_clientStr="";
		_clientStr+=;
		_clientStr+="";
		if(_revCtrlMsg!="")_clientStr+=_revCtrlMsg;
		else _clientStr+=retMessage;
		_clientStr+="\"}}&^!";
						Serial.println(10);
		char* c = strToChar(_clientStr);
						Serial.println(11);
		if(c)
		{
			_clientRevCtrl.print(c);
			Serial.print(c);
		}
			free(c);
			c = NULL;
						Serial.println(12);
						*/
		/*
		char* c = (char*)malloc(_clientStr.length()+1);
		_clientStr.toCharArray(c,_clientStr.length()+1);
		_clientRevCtrl.print(c);
		Serial.print(c);
		free(c);
		c = NULL;
		*/
		
		Serial.println("response to server.");
		
		setRevCtrlMsg("false","function not binded");
		_clientStr = NULL;
		
	}
}

char* LeweiTcpClient::getParaValue(String &orig,String paraName)
{
		//Serial.print("getParaValue ");
		//Serial.println(paraName);
		int functionNameStartPos = orig.indexOf("\""+paraName+"\":\"");
		if(functionNameStartPos<0)return NULL;
		int functionNameEndPos = orig.indexOf("\"",functionNameStartPos+4+paraName.length());
		String functionName = orig.substring(functionNameStartPos+4+paraName.length(),functionNameEndPos);
		
		char* c = strToChar(functionName);
		  //char* c = (char*)malloc(functionName.length()+1);
			//functionName.toCharArray(c,functionName.length()+1);
		//Serial.print("g---otParaValue ");
		//Serial.println(c);
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

void LeweiTcpClient::directResponse(String respStr)
{
	if(_clientRevCtrl.connected())
	{
		respStr +="&^!";
		char* c = (char*)malloc(respStr.length()+1);
		respStr.toCharArray(c,respStr.length()+1);
		_clientRevCtrl.print(c);
		Serial.println(c);
		free(c);
		c = NULL;
	}
	
}

void LeweiTcpClient::connentTcpServer()
{
	bIsConnecting = true;
	Serial.print("connecting...");
	
	_clientRevCtrl.stop();
	if (_clientRevCtrl.connect(tcpServer, 9960))
	{
		Serial.println("connected");
		//delay(800);
		sendOnlineCommand();
		/*
		String connStr = "{\"method\":\"update\",\"gatewayNo\":\""+String(_gatewayNo)+"\",\"userkey\":\""+String(_userKey)+"\"}&^!";
		char* c = (char*)malloc(connStr.length()+1);
		connStr.toCharArray(c,connStr.length()+1);
		_clientRevCtrl.print(c);
		free(c);
		c = NULL;
		connStr = "";
		*/
		
	}
	else 
	{
		_clientRevCtrl.stop();
		// if you didn't get a connection to the server:
		
		Serial.println("connect failed");
		Serial.println(_clientRevCtrl.status());
	}
	bIsConnecting = false;
}

void LeweiTcpClient::setRevCtrlMsg(char* execResult,char* msg)
{
	_revCtrlResult = execResult;
	_revCtrlMsg = msg;
}

void LeweiTcpClient::sendSensorValue(String sensorName,String sensorValue)
{
	/*
	//using tcp.lewei50.com to upload data
	*/
	//Serial.print("connecting...");
	//Serial.println(tcpServer);
	if (_clientRevCtrl.connected())
	{
		Serial.println("sending data111");		
		
		/*
			int len=sensorName.length()+sensorValue.length()+57;
			//Serial.println(len);
			commandString=(char *)malloc(len);	
			snprintf(commandString, len, "{\"method\": \"upload\", \"data\":[{\"Name\":\"%s\",\"Value\":%s\"\"}]}&^!", sensorName, sensorValue);
			Serial.println(commandString);
			//_clientRevCtrl.print(commandString);
			free(commandString);
			commandString = NULL;
			*/
			
		
		checkFreeMem();
		String connStr = "{\"method\": \"upload\", \"data\":[{\"Name\":\"";
		connStr+=sensorName;
		connStr+="\",\"Value\":\"";
		connStr+=sensorValue;
		connStr+="\"}]}&^!";
		//String connStr = "\"method\": \"upload\", \"data\":[{\"Name\":\"\",\"Value\":\"\"}]}&^!";
		Serial.print("connStr.length():");	
		Serial.println(connStr.length());	
		checkFreeMem();
		char* c = (char*)malloc(connStr.length()+1);
		if(c)
		{
			connStr.toCharArray(c,connStr.length()+1);
			_clientRevCtrl.print(c);
			free(c);
			c = NULL;
		}
		else
		{
			Serial.println("sendSensorValue::malloc failed");
		}
		connStr = NULL;
		
		checkFreeMem();
		
	}
	else 
	{
		//_clientRevCtrl.stop();
		// if you didn't get a connection to the server:
		
		Serial.println("data send failed");
	}
	
	
	
	/*
	//using open.lewei50.com to upload data
	if (_clientUpload.connect(uploadServer, 80))
	{
		Serial.println("clientUploader connected");
		
		String connStr ="POST /api/V1/gateway/UpdateSensors/01 HTTP/1.1\r\nuserkey:";
		connStr +=_userKey;
		connStr +="\r\nHost:open.lewei50.com\r\nContent-Length:";
		connStr +=(24+sensorName.length()+sensorValue.length());
		connStr +="\r\nConnection: close\r\n\r\n[{\"Name\":\"";
		connStr +=sensorName;
		connStr +="\",\"Value\":\"";
		connStr +=sensorValue;
		connStr +="\"}]";
		Serial.println(connStr);
		
		char* c = (char*)malloc(connStr.length()+1);
		connStr.toCharArray(c,connStr.length()+1);
		_clientUpload.print(c);
		free(c);
		c = NULL;
		connStr = "";
		_clientUpload.stop();
	}
	else
	{
		// if you couldn't make a connection:
		Serial.println("connection failed");
		Serial.println("disconnecting.");
		_clientUpload.stop();
	}
	
	*/
	
}

void LeweiTcpClient::checkFreeMem()
{
		for(int i = 1024;i>0;i--)
		{
			char* c = (char*)malloc(i);
			if(c)
			{
				free(c);
				c=NULL;
				Serial.print("M:");
				Serial.println(i);
				break;
			}
		}
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


void LeweiTcpClient::execute(void (*callfuct)())
{
	Serial.println("exec no para");
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



void LeweiTcpClient::addUserFunction(UserFunction &uFunction)
{
	UserFunctionNode *n1,*n2;
	n2 = (UserFunctionNode*) new(UserFunctionNode);
	
	
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
	else 
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

