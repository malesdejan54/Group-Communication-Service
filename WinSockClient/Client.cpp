
#include "../LibTCPConnection/TCPConnection.h"
#include "../ClientInterface/ClientInterfaceh.h"


DWORD WINAPI FunctionRECV(LPVOID lpParam);

int __cdecl main(int argc, char** argv)
{
	SOCKET connectSocket = INVALID_SOCKET;
	int id = 0;
	int option = 0;
	char groupNameLength[DEFAULT_BUFLEN];
	memset(groupNameLength, 0, sizeof(int));
	char* groupName = NULL;
	char message[DEFAULT_BUFLEN];


	

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	if (ConnectSocketInitialization(&connectSocket, argv, DEFAULT_PORT) == false)
	{
		WSACleanup();
		return 1;
	}

	///////////////////
	HANDLE hRecvThread;
	hRecvThread = CreateThread(NULL, NULL, &FunctionRECV, &connectSocket, NULL, NULL);
	//////////////////////

	while (id != 1)
	{
		printf(">>>>>>>>>>---MENU---<<<<<<<<<<\n");
		printf("1 --> JOIN GROUP\n");
		printf("2 --> EXIT\n");
		printf("--------------------\n");
		option = _getch();

		switch (option - 48)
		{
		case 1:
			printf("Enter the length of group name :");
			gets_s(groupNameLength, DEFAULT_BUFLEN);

			if (ClientVerification(groupNameLength, 0) == false)
			{
				continue;
			}

			groupName = (char*)malloc(atoi(groupNameLength) + 1);	// + 1 for '\0'
			printf("Enter Group Name (max length of group name is: %d):", atoi(groupNameLength));
			gets_s(groupName, atoi(groupNameLength) + 1);
			if (ClientVerification(groupName, 1) == false)
			{
				continue;
			}


			//if all verifications are ok, client send group and message to server
			Connect(connectSocket, groupName);

			while (id != 1)
			{
				printf(">>>>>>>>>>---MENU---<<<<<<<<<<\n");
				printf("1 --> Send mesage to group : \n");
				printf("2 --> EXIT\n");
				printf("--------------------\n");
				option = _getch();
				switch (option - 48)
				{
				case 1:
					printf("Enter message:");
					memset(message, 0, DEFAULT_BUFLEN);
					gets_s(message, DEFAULT_BUFLEN);
					if (ClientVerification(message, 1) == false)
					{
						continue;
					}

					SendMsg(connectSocket, message);

					break;

				case 2:
					id = 1;
					Disconnect(connectSocket);
					printf("Exit sucessfully.\n");
					break;
				default:
					printf("Please enter a valid number (1 for send message or 0 for exit)\n");
					break;

				}

			}
			break;
		case 2:
			id = 1;
			Disconnect(connectSocket);
			printf("Exit sucessfully.\n");
			break;
		default:
			printf("Please enter a valid number (1 for send message or 0 for exit)\n");
			break;
		}
		printf("\n\n");

	}

	if (groupName != NULL)
		free(groupName);

	closesocket(connectSocket);
	CloseHandle(hRecvThread);
	WSACleanup();
	return 0;
}

DWORD WINAPI FunctionRECV(LPVOID lpParam)
{
	printf("Client is ready for messages from group.\n");
	int iResult;
	SOCKET connectSocket = *(SOCKET*)lpParam;

	FD_SET set;
	timeval timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;

	do
	{
		char messageSize[4];

		FD_ZERO(&set);
		FD_SET(connectSocket, &set);

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
			//receieve message length
			iResult = ReceiveFunction(connectSocket, messageSize, sizeof(int));
		}
		if (iResult <= 0)
		{
			closesocket(connectSocket);
			break;
		}


		int len = *(int*)messageSize;
		char* recvBuffer = (char*)malloc(len - 4);
		memset(recvBuffer, 0, len - 4);

		iResult = select(0, &set, NULL, NULL, &timeVal);
		if (iResult > 0)
		{
			//receive message
			iResult = ReceiveFunction(connectSocket, recvBuffer, len - 4);
			printf("MESSAGE FROM SERVER: %s\n", recvBuffer);
			printf("--------------------------------\n");
		}
		if (iResult <= 0)
		{
			closesocket(connectSocket);
			printf("Error: %ld\n", WSAGetLastError());
			free(recvBuffer);
			break;
		}

		free(recvBuffer);
	} while (1);

	return 0;
}

