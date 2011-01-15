/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ReefAngel_Wifi.h"
#include <ReefAngel_Globals.h>


#ifdef wifi

void WebResponse (const prog_uchar *response, long strsize)
{
//  P(WebHeaderMsg) = SERVER_HEADER_HTML;
//  printP(WebHeaderMsg);
    P(WebBodyMsg) = SERVER_HEADER_HTML;
    printP(WebBodyMsg);
    Serial.print(strsize,DEC);
    P(WebBodyMsg1) = SERVER_HEADER3;
    printP(WebBodyMsg1);
    printP(response);
}

void printP(const prog_uchar *str)
{
    char a;
    do
    {
        a=pgm_read_byte(str++);
        if (a!=0) Serial.print(a);
    }
    while (a!=0);
}

void PROGMEMprint(const prog_uchar str[])
{
    char c;
    if(!str) return;
    while((c = pgm_read_byte(str++)))
        Serial.print(c,BYTE);
}

void pushbuffer(byte inStr)
{

	m_pushback[m_pushbackindex]=inStr;
	m_pushback[m_pushbackindex+1]=0;
	//memcpy(&m_pushback[0], &m_pushback[1], 31);
	//m_pushback[30]=inStr;
	//m_pushback[31]=0;
	//if (~reqtype)
	//{
	if (reqtype>0 && reqtype<128)
	{
		if (authStr[m_pushbackindex]==inStr) m_pushbackindex++; else m_pushbackindex=0;
		if (authStr[m_pushbackindex]==0) auth=true;
		//if (m_pushbackindex>0) Serial.println(m_pushbackindex,DEC);
		//if (m_pushbackindex>0) Serial.println(test,DEC);
	}
	else
	{
		m_pushbackindex++;
		if (m_pushbackindex==32) m_pushbackindex=0;
		if (reqtype>128)
		{
		    if (inStr==' ')
		    {
		        reqtype=256-reqtype;
		    }
		    else
		    {
		        weboption*=10;
		        weboption+=inStr-'0';
		    }
		}
		else
		{
            if (strncmp("GET / ", m_pushback, 6)==0) reqtype=1;
            if (strncmp("GET /wifi", m_pushback, 9)==0) reqtype=2;
            if (strncmp("GET /r", m_pushback, 6)==0) reqtype=3;
		}
	}
}

void processHTTP()
{
    bIncoming=true;
    timeout=millis();
    while (bIncoming)
    {
		if (millis()-timeout>100)
		{
			bIncoming=false;
			//for (int a=0;a<32;a++) pushbuffer(0);
		}
		if (Serial.available()>0)
		{
			pushbuffer(Serial.read());
			timeout=millis();
		}
    }
	if (authStr[0]==0) auth=true;
    if (auth)
    {
      //Serial.println(reqtype,DEC);
		auth=false;
		if (reqtype==1)
		{
		  P(WebBodyMsg) = SERVER_DEFAULT;
		  WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
		}
		if (reqtype==2)
		{
		  P(WebBodyMsg) = SERVER_HEADER_HTML;
		  printP(WebBodyMsg);
		  Serial.print(sizeof(SERVER_RA) - 1,DEC);
		  P(WebBodyMsg1) = SERVER_HEADER3;
		  printP(WebBodyMsg1);
		  PROGMEMprint(SERVER_RA);
		}
		if (reqtype==3)
		{
			byte o_relay=weboption/10;
			byte o_type=weboption%10;
			if (o_type==0)
			{
			  bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
			  bitClear(ReefAngel.Relay.RelayMaskOff,o_relay-1);
			}
			if (o_type==1)
			{
			  bitSet(ReefAngel.Relay.RelayMaskOn,o_relay-1);
			  bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
			}
			if (o_type==2)
			{
			  bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
			  bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
			}
			ReefAngel.Relay.Write();
		  char temp[6];
		  int s=76;

		  s+=intlength(ReefAngel.Params.Temp1);
		  s+=intlength(ReefAngel.Params.Temp2);
		  s+=intlength(ReefAngel.Params.Temp3);
		  s+=intlength(ReefAngel.Params.PH);
		  s+=intlength(ReefAngel.Relay.RelayData);
		  s+=intlength(ReefAngel.Relay.RelayMaskOn);
		  s+=intlength(ReefAngel.Relay.RelayMaskOff);
		  P(WebBodyMsg) = SERVER_HEADER_XML;
		  printP(WebBodyMsg);
		  Serial.print(s);
		  P(WebBodyMsg1) = SERVER_HEADER3;
		  printP(WebBodyMsg1);
		  Serial.print("<RA><T1>");
		  Serial.print(ReefAngel.Params.Temp1);
		  Serial.print("</T1><T2>");
		  Serial.print(ReefAngel.Params.Temp2);
		  Serial.print("</T2><T3>");
		  Serial.print(ReefAngel.Params.Temp3);
		  Serial.print("</T3><PH>");
		  Serial.print(ReefAngel.Params.PH);
		  Serial.print("</PH><R>");
		  Serial.print(ReefAngel.Relay.RelayData,DEC);
		  Serial.print("</R><RON>");
		  Serial.print(ReefAngel.Relay.RelayMaskOn,DEC);
		  Serial.print("</RON><ROFF>");
		  Serial.print(ReefAngel.Relay.RelayMaskOff,DEC);
		  Serial.print("</ROFF><ATOLOW>");
		  Serial.print(ReefAngel.LowATO.IsActive());
		  Serial.print("</ATOLOW><ATOHIGH>");
		  Serial.print(ReefAngel.HighATO.IsActive());
		  Serial.print("</ATOHIGH></RA>");
		}
    }
    else
    {
		if (reqtype>0)
		{
		  P(WebBodyMsg) = SERVER_DENY;
		  printP(WebBodyMsg);
		}
    }
	Serial.flush();
	m_pushbackindex=0;
    reqtype=0;
    weboption=0;
}

char GetC(int c)
{
	return pgm_read_byte(c+EncodingChars);
}

void ConvertC(char* strIn, char* strOut, byte len)
{
	strOut[0] = GetC(strIn[0]>>2);
	strOut[1] = GetC(((strIn[0]&0x03)<<4)|((strIn[1]&0xf0)>>4));
	strOut[2] = len>1?GetC(((strIn[1]&0x0f)<<2)|((strIn[2]&0xc0)>>6)):'=';
	strOut[3] = len>2?GetC(strIn[2]&0x3f ):'=';
}

void WifiAuthentication(char* userpass)
{
	char* authPtr;
	int len = strlen(userpass);
	int authPtrSize = ((len + 2) / 3) << 2;
	authPtr = (char*)malloc(authPtrSize + 1);
	char* authPtr1 = authPtr;
	while (len > 0)
	{
		ConvertC(userpass, authPtr1, min(len,3));
		authPtr1 += 4;
		userpass += 3;
		len -= 3;
	}
	*(authPtr + authPtrSize) = 0;
	strcpy(authStr,authPtr);
	free(authPtr);
	Serial.println(authStr);
}

#endif  // wifi

void pingSerial()
{
#ifdef wifi
    if ( Serial.available() > 0 ) processHTTP();
#endif  // wifi
}
