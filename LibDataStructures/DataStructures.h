#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>


struct Poruke {
	char* data;
	Poruke* next;
};

struct Klijenti {
	char* name;
	Klijenti* next;
};

struct Grupe {
	char* name;
	Grupe* next;
	Poruke* frontElement;
	Poruke* rearElement;
	Klijenti* klijenti;
	HANDLE hSemaphore;
};



bool GetElementFromList(char* name, Grupe* head);
Grupe* AddGroupToList(char* groupName,char* socketString, Grupe** head);
void AddClientToGroup(char* groupName,char* socketString, Grupe** head);
void PrintListGruops(Grupe* head);
void PrintListClients(Grupe* head);
Grupe* FindGroup(char* socketString, Grupe* head);
void AddMessageToQueue(char* message,Grupe** head);
bool MessagesInQueue(Grupe* head);
void RemoveMessageFromQueue(Grupe* head);
void RemoveClientFromList(char* socketString, Grupe** head);
//void RemoveMessageFromQueue(Grupe** currentSocket);
//void DisplayQueue(Grupe* head);
//Grupe* GetSocketFromList(char* socketString, Grupe* head);