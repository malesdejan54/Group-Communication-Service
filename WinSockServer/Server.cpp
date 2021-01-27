
#include "../LibTCPConnection/TCPConnection.h"
#include "../LibDataStructures/DataStructures.h"





DWORD WINAPI FunctionRecv(LPVOID lpParam);
DWORD WINAPI FunctionSendMessages(LPVOID lpParam);

Grupe* grupaHead = NULL;
HANDLE hSendThread;
HANDLE hRecvThread;

int  main(void)
{
	int iResult;
	int retListenSocket;
	SOCKET acceptedSocket = INVALID_SOCKET;						
	SOCKET listenSocket = INVALID_SOCKET;


	retListenSocket = ListenSocketDefiniton(&listenSocket, DEFAULT_PORT);
	if (retListenSocket == 1)
	{
		printf("Initialization socket failed.\n");
		return 1;
	}
	else
	{
		printf("Server initialized. Waiting for clients...\n");
	}

	//initialization structures
	//klijentHead = NULL;

	int counter = 0;
	do
	{
		if (SocketNonBlockingMode(&acceptedSocket, &listenSocket) == true)
		{
			printf("Client accepted.\n");
		}
		else
		{
			return 1;
		}
	
		//make thread for reciving messages from client // za svkakog klijenta poseban trhread
		hRecvThread = CreateThread(NULL, NULL, &FunctionRecv, &acceptedSocket, NULL, NULL);


	} while (1);

	// shutdown accepted socket
	iResult = ShutdownFunction(&acceptedSocket);
	if (iResult == 1)
	{
		return iResult;
	}

	//close thread's handle
	CloseHandle(hRecvThread);
	CloseHandle(hSendThread);
	


	//close all sockets
	closesocket(acceptedSocket);
	closesocket(listenSocket);

	WSACleanup();
	return 0;

}


DWORD WINAPI FunctionRecv(LPVOID lpParam)
{
	printf("Server ready for reciving messages from client.\n");

	SOCKET acceptedSocket = *(SOCKET*)lpParam;
	int iResult = 0;

	FD_SET set;
	timeval timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;

	do
	{
		char messageSize[4];

		FD_ZERO(&set);
		FD_SET(acceptedSocket, &set);

		/////////////////
		//comand
		char commandSize[2];
		iResult = select(0, &set, NULL, NULL, &timeVal);
		if (iResult == SOCKET_ERROR)
		{
			printf("select failed: %ld\n", WSAGetLastError());
		}

		if (iResult == 0)
		{
			Sleep(1000);
			continue;
		}


		if (iResult > 0)
		{
			//receive message length /// ZAsto pozivam ovio kad ne koristmo u opste duzinu od poruke
			iResult = ReceiveFunction(acceptedSocket, commandSize,2);

		}

		if (commandSize[0] == 'e')
		{
			char* socketString = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);
			/// zasto ovde ide 10 (to je neki radix)
			_itoa((int)acceptedSocket, socketString, 10);
			//proveriti u kojoj se grupi dati soket nalazi
			Grupe* selectedGroup = FindGroup(socketString, grupaHead);
			RemoveClientFromList(socketString, &selectedGroup);
			printf("\nClient with socket : %s is shutdown !", socketString);
			//close thread's handle


		}
		if (iResult <= 0)
		{
			closesocket(acceptedSocket);
			break;
		}

	
		//char command = *commandSize;
		/////////////////
		//if (commandSize[1] == 'j')
		///////////////



		iResult = select(0, &set, NULL, NULL, &timeVal);
		if (iResult == SOCKET_ERROR)
		{
			printf("select failed: %ld\n", WSAGetLastError());
		}

		if (iResult == 0)
		{
			Sleep(1000);
			continue;
		}


		if (iResult > 0)
		{
//receive message length ///drugi param: je adresa u koju ce da se smesti ono sto ce se primiti,a treci velicina sto cu da primim
			iResult = ReceiveFunction(acceptedSocket, messageSize, sizeof(int));
		}
		if (iResult <= 0)
		{
			closesocket(acceptedSocket);
			break;
		}


		int len = *(int*)messageSize;
		char* recvBuffer = (char*)malloc(len - 6);// prvi bit slovo j ili s drugi bit '\0' i jos 4 za duzinu 

		iResult = select(0, &set, NULL, NULL, &timeVal);
		if (iResult > 0)
		{
			iResult = ReceiveFunction(acceptedSocket, recvBuffer, len - 6);

			//char* group = NULL;
			
			char* socketString = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);
			/// zasto ovde ide 10 (to je neki radix)
			_itoa((int)acceptedSocket, socketString, 10);



			if (commandSize[0] == 'j')
			{

				printf("GROUP: %s\n", recvBuffer);

				printf("--------------------------------\n");

				//// input data in structures
				if (GetElementFromList(recvBuffer, grupaHead) == false)
				{
					//add new topic
					//rec= treca ,grupa(head) * na rokise
				 Grupe* currentGroupe= AddGroupToList(recvBuffer, socketString, &grupaHead);
				 AddClientToGroup(recvBuffer, socketString, &grupaHead);
					hSendThread = CreateThread(NULL, NULL, &FunctionSendMessages, currentGroupe, NULL, NULL);
				}
				else
				{
					if (grupaHead != NULL)
					{
						//add client to group
						AddClientToGroup(recvBuffer, socketString, &grupaHead);
					}
				}

			/*	printf("//////////////////////");
				PrintListGruops(grupaHead);
				printf("//////////////////////");

				printf("\\\\\\\\\\\\\\\\\\\\\\\\\\");
				PrintListClients(grupaHead);
				printf("\\\\\\\\\\\\\\\\\\\\\\\\\\");*/


			}
			else if (commandSize[0] == 's')
			{
				printf("\n");
				printf("Messsage from client %s: %s\n",socketString ,recvBuffer);

				printf("--------------------------------\n");

				//proveriti u kojoj se grupi dati soket nalazi
				 
				Grupe* selectedGroup = FindGroup(socketString, grupaHead);
				AddMessageToQueue(recvBuffer,&selectedGroup);

			}
			
		}
		if (iResult <= 0)
		{
			closesocket(acceptedSocket);
			printf("Error: %ld\n", WSAGetLastError());
			free(recvBuffer);
			break;
		}

	} while (1);

	return 0;
}

DWORD WINAPI FunctionSendMessages(LPVOID lpParam)
{
	
	Grupe* selectedGroupe = (Grupe*)lpParam;
	int iResult = NULL;

	do {
		//char* socketString = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);
		//_itoa((int)acceptedSocket, socketString, 10);

		char* message = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);
		char* sendBuffer = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN + 4);		//+4 for message length


		if (MessagesInQueue(selectedGroupe))
		{
			Klijenti* currentK = selectedGroupe->klijenti;
			while (currentK != NULL)
			{
				*((int*)sendBuffer) = (int)strlen(message);
				strcpy(sendBuffer + 4, selectedGroupe->rearElement->data);

				int socketNumber = _atoi64(currentK->name);
				SOCKET acceptedSocket = (SOCKET)socketNumber;
				iResult = SendFunction(acceptedSocket, sendBuffer, *((int*)sendBuffer));

				currentK = currentK->next;
				if (iResult == SOCKET_ERROR)
				{
					printf("Send failed with error: %d\n", WSAGetLastError());
					closesocket(acceptedSocket);
					break;
				}


			}

			RemoveMessageFromQueue(selectedGroupe);
		}
		else
		{
			Sleep(5000);
		}

		free(message);
		free(sendBuffer);

	} while (1);

	return 0;
}
