// ClientInterface.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "ClientInterfaceh.h"

void Connect(SOCKET connectSocket, char* groupName)
{
	int iResult = 0;

	FD_SET set;						
	timeval timeVal;				
	FD_ZERO(&set);
	FD_SET(connectSocket, &set);
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;



	/*
		Format of messageToSend variable:1 byte for "j" +1 for "\0"
			4 bytes for message length +
			messageCharacterCount bytes for message + 1 '\0'
	*/

	int len = 7 + (int)strlen(groupName);
	char* messageToSend = (char*)malloc(len);
	memset(messageToSend, 0, len);


	*messageToSend = 'j';
	messageToSend[1] = '\0';
	*(int*)(messageToSend + 2) = len; //vrednost na pocetku toga
	strcpy(messageToSend + 6, groupName);

	iResult = SendFunction(connectSocket, messageToSend, len);
	if (iResult == SOCKET_ERROR)
	{
		printf("Send failed with error: %ld\n", WSAGetLastError());
		free(messageToSend);
		closesocket(connectSocket);
		WSACleanup();
	}

	free(messageToSend);
}

void SendMsg(SOCKET connectSocket, char* message)
{
	int iResult = 0;
	int messageCharacterCount = GetMessageLen(message);

	FD_SET set;						
	timeval timeVal;				
	FD_ZERO(&set);
	FD_SET(connectSocket, &set);
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;


	int len = 7 + messageCharacterCount;
	char* messageToSend = (char*)malloc(len);
	memset(messageToSend, 0, len);

	*messageToSend = 's';
	messageToSend[1] = '\0';
	*(int*)(messageToSend + 2) = len; //vrednost na pocetku toga
	strcpy(messageToSend + 6, message);

	iResult = SendFunction(connectSocket, messageToSend, len);
	if (iResult == SOCKET_ERROR)
	{
		printf("Send failed with error: %ld\n", WSAGetLastError());
		free(messageToSend);
		closesocket(connectSocket);
		WSACleanup();
	}

	free(messageToSend);
}

void Disconnect(SOCKET connectSocket)
{
	int iResult = 0;

	FD_SET set;					
	timeval timeVal;				
	FD_ZERO(&set);
	FD_SET(connectSocket, &set);
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;

	int len = 2;
	char* messageToSend = (char*)malloc(len);
	memset(messageToSend, 0, len);

	*messageToSend = 'e';
	messageToSend[1] = '\0';

	iResult = SendFunction(connectSocket, messageToSend, len);
	if (iResult == SOCKET_ERROR)
	{
		printf("Send failed with error: %ld\n", WSAGetLastError());
		free(messageToSend);
		closesocket(connectSocket);
		WSACleanup();
	}

	free(messageToSend);

}