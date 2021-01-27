//// LibDataStructures.cpp : Defines the functions for the static library.
////
//
#include "pch.h"
#include "framework.h"
#include "DataStructures.h"
HANDLE hSemaphore;

bool GetElementFromList(char* name, Grupe* head)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	while (head != NULL && head->name != NULL)
	{
		if (strcmp(head->name, name) == 0)
		{
			//we find the name of topic in topic list
			return true;
		}
		head = head->next;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return false;
}

Grupe* AddGroupToList(char* groupName,char* socketString, Grupe** head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	//alocation memory for topic and queue
	Grupe* newGroup = (Grupe*)malloc(sizeof(Grupe));
	newGroup->frontElement = (Poruke*)malloc(sizeof(Poruke));
	newGroup->rearElement = (Poruke*)malloc(sizeof(Poruke));
	newGroup->klijenti = (Klijenti*)malloc(sizeof(Klijenti));

	newGroup->name = groupName;
	newGroup->next = NULL;
	newGroup->frontElement = NULL;
	newGroup->rearElement = NULL;
	newGroup->klijenti = NULL;

	if (*head == NULL)
	{
		*head = newGroup;
		ReleaseSemaphore(hSemaphore, 1, NULL);
		return *head;

	}
	else
	{
		Grupe* currentG = *head;
		while (currentG->next != NULL)
		{
			currentG = currentG->next;
		}

		currentG->next = newGroup;
		ReleaseSemaphore(hSemaphore, 1, NULL);
		return currentG->next;
	}

	

//	AddClientToGroup(groupName, socketString, &newGroup);
}

void AddClientToGroup(char* groupName,char* socketString, Grupe** head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	Grupe* currentG = *head;
	while (currentG->next != NULL)
	{
		if (currentG->name == groupName)
		{
			break;
		}
		currentG = currentG->next;
	}

	Klijenti* newClient = (Klijenti*)malloc(sizeof(Klijenti));
	newClient->name = socketString;
	newClient->next = NULL;
	
	
	Klijenti** currentK = &currentG->klijenti;

	if (*currentK == NULL)
	{
		*currentK = newClient;
		//printf("\n %s", newClient->name);
	}
	else
	{
		Klijenti* currentK2 = *currentK;
		while (currentK2->next != NULL)
		{
			//printf("\n %s", currentK2->name);
			currentK2 = currentK2->next;
		}

		currentK2->next = newClient;
	}
	




	ReleaseSemaphore(hSemaphore, 1, NULL);
}

void PrintListGruops(Grupe* head)
{
	if (head == NULL)
		printf("\nList of groups is empty.");
	else
	{
		Grupe* currentNode = head;

		while (currentNode != NULL)
		{
			printf("%s ", currentNode->name);
			currentNode = currentNode->next;
		}

	}

	
}

void PrintListClients(Grupe* head)
{
	Grupe* currentG = head;
	Klijenti* currentK = currentG->klijenti;



	if (currentK == NULL)
		printf("\nList of clients is emptyiiiiiiiiiiiiiiiiiiii.");
	else
	{
		

		while (currentK != NULL)
		{
			printf("%s ", currentK->name);
			currentK = currentK->next;
		}

	}


}


Grupe* FindGroup(char* socketString, Grupe* head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	if (head == NULL)
		printf("\nList of groups is empty.");
	else
	{
		Grupe* currentNode = head;

		while (currentNode != NULL)
		{
			
			Klijenti* currentK = currentNode->klijenti;

			if (currentK == NULL)
				printf("\nList of clients is emptyyyyyyyyy.");
			else
			{


				while (currentK != NULL)
				{
					//printf("\n klijent : .%s.", currentK->name);
					//printf("\n soket : .%s.", socketString);
					if (strcmp(currentK->name,socketString)== 0)
					{
						return currentNode;
					}
					currentK = currentK->next;
				}

			}

	

			currentNode = currentNode->next;
		}
		return NULL;
	}


	ReleaseSemaphore(hSemaphore, 1, NULL);
}


void AddMessageToQueue(char* message, Grupe** head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	Grupe* currentM = *head;

	Poruke* newMessage = (Poruke*)malloc(sizeof(Poruke));
	newMessage->data = message;
	newMessage->next = NULL;

	if (currentM->frontElement == NULL)
	{
		currentM->frontElement = newMessage;
		currentM->rearElement = newMessage;
	}
	else
	{
		currentM->rearElement->next = newMessage;
		currentM->rearElement = newMessage;

		if (currentM->frontElement->next == NULL)
		{
			currentM->frontElement->next = newMessage;
		}
	}

	printf("\n Poslednja poruka : %s",currentM->rearElement->data);

	ReleaseSemaphore(hSemaphore, 1, NULL);

}
bool MessagesInQueue(Grupe* head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	Grupe* currentG = head;


	if(currentG->rearElement == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);

}

void RemoveMessageFromQueue(Grupe* head)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	Grupe* currentGroupe = head;

	if ((currentGroupe)->frontElement != NULL)
	{
		Poruke* elementToDelete;
		elementToDelete = currentGroupe->frontElement;
		if (currentGroupe->frontElement == currentGroupe->rearElement)
		{
			currentGroupe->frontElement = NULL;
			currentGroupe->rearElement = NULL;
		}
		else
		{
			currentGroupe->frontElement = currentGroupe->frontElement->next;
		}
		free(elementToDelete);
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
}


void RemoveClientFromList(char* socketString, Grupe** head)
{
	if (*head == NULL)
	{
		return;
	}
	Grupe* currentG = *head;
	Klijenti* prethodni = currentG->klijenti;

	if (prethodni == NULL) //ako je prazna lista
	{
		return;
	}

	Klijenti* pom = prethodni->next;
	 
	if (strcmp(prethodni->name, socketString) == 0)
	{
		currentG->klijenti = pom;	//glava je drugi element
		free(prethodni);	//brisem prvi element
		return;
	}

	while (pom != NULL)
	{

		//printf("\n%d", pom->data);
		if (strcmp(pom->name, socketString) == 0)
		{
			prethodni->next = pom->next;	//glava je drugi element
			free(pom);	//brisem prvi element
			return;
		}

		pom = pom->next;
		prethodni = prethodni->next;

	}

}