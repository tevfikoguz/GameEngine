﻿
#include <Networking\NetworkManager.h>
#include <Networking\Connection.h>
#include <Networking\ServerConnection.h>
#include <Threading\Thread.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>
#include <iostream>

using namespace Engine::Common;
using namespace Engine::Networking;
using namespace Engine::Platform;
using namespace Engine::MemoryManagement::Allocator;

DynamicSizeAllocator alloc("Network", &RootAllocator::GetInstance(), 2048);

const byte PROTOCOL_IDENTIFIER[]{ 10, 112, 255, 0, 34, 23, 6 , 73, 251, 134, 211, 136, 65, 2, 7, 3 };
const int8 PROTOCOL_IDENTIFIER_SIZE = sizeof(PROTOCOL_IDENTIFIER);
const uint8 RECEIVE_BUFFER_SIZE = 128;
const uint8 PORT_NUMBER = 30001;

void GetError()
{
	PlatformNetwork::Errors error = PlatformNetwork::GetLastError();


}

void Server()
{
	Connection server(&alloc, PROTOCOL_IDENTIFIER, PROTOCOL_IDENTIFIER_SIZE, RECEIVE_BUFFER_SIZE, RECEIVE_BUFFER_SIZE);

	server.Listen(PORT_NUMBER);

	static char toClientBuffer[] = "I read you.";

	byte buffer[RECEIVE_BUFFER_SIZE];
	uint32 receivedLen = 0;

	while (true)
	{
		if (server.Receive(buffer, sizeof(buffer), receivedLen) && receivedLen != 0)
		{
			std::cout << "Server : " << buffer << " received from client\n";

			server.Send(reinterpret_cast<const byte*>(toClientBuffer), sizeof(toClientBuffer));
		}
	}
}

void Client()
{
	Connection client(&alloc, PROTOCOL_IDENTIFIER, PROTOCOL_IDENTIFIER_SIZE, RECEIVE_BUFFER_SIZE, RECEIVE_BUFFER_SIZE);

	client.Connect(Address(127, 0, 0, 1, PORT_NUMBER));

	static char toServerBuffer[] = "Do you read me ?";

	byte buffer[RECEIVE_BUFFER_SIZE];
	uint32 receivedLen = 0;

	while (true)
	{
		if (!client.Send(reinterpret_cast<const byte*>(toServerBuffer), sizeof(toServerBuffer)))
		{
			GetError();
			return;
		}

		if (client.Receive(buffer, sizeof(buffer), receivedLen) && receivedLen != 0)
			std::cout << "Client : " << buffer << " received from server\n";
	}
}

void main()
{
	NetworkManager::GetInstance();

	Engine::Threading::Thread serverThread;
	serverThread.Initialize(Server, 512, nullptr);

	Engine::Threading::Thread clientThread;
	clientThread.Initialize(Client, 512, nullptr);

	system("pause");
}