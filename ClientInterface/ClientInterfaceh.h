#pragma once
#include "../LibTCPConnection/TCPConnection.h"

void Connect(SOCKET connectSocket, char* groupName);
void SendMsg(SOCKET connectSocket, char* message);
void Disconnect(SOCKET connectSocket);