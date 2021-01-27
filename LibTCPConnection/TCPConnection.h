#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <conio.h>
#include <string.h>


#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27016"

bool InitializeWindowsSockets();

bool ConnectSocketInitialization(SOCKET* connectSocket, char** argv, PCSTR port);
bool ClientVerification(char* message, int verificationMode);
int SendFunction(SOCKET sendSocket, char* sendBuffer, int len);
int GetMessageLen(char* message);
int ReceiveFunction(SOCKET recvSocket, char* recvBuffer, int len);

int ListenSocketDefiniton(SOCKET* listenSocket, PCSTR port);
bool SocketNonBlockingMode(SOCKET* acceptedSocket, SOCKET* listenSocket);
int ShutdownFunction(SOCKET* acceptedSocket);