﻿
#include <Networking\NetworkManager.h>
#include <Networking\ReliableConnection.h>
#include <Threading\Thread.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>
#include <iostream>

using namespace Engine::Common;
using namespace Engine::Networking;
using namespace Engine::Platform;
using namespace Engine::MemoryManagement::Allocator;

DynamicSizeAllocator alloc("Network", RootAllocator::GetInstance(), 1000000);

const byte PROTOCOL_IDENTIFIER[]{ 10, 112, 255, 0, 34, 23, 6 , 73, 251, 134, 211, 136, 65, 2, 7, 3 };
const int8 PROTOCOL_IDENTIFIER_SIZE = sizeof(PROTOCOL_IDENTIFIER);
const uint32 BUFFER_SIZE = 128;
const uint16 PORT_NUMBER = 30001;

void GetError()
{
	PlatformNetwork::Errors error = PlatformNetwork::GetLastError();


}

void Server(void *args)
{
	ReliableConnection server(&alloc, PROTOCOL_IDENTIFIER, PROTOCOL_IDENTIFIER_SIZE, BUFFER_SIZE, BUFFER_SIZE);

	server.Listen(PORT_NUMBER);

	static char8 toClientBuffer[] = "I read you.";

	byte buffer[BUFFER_SIZE];
	uint32 receivedLen = 0;

	ReliableConnection::SequenceNumber number = 0;

	while (true)
	{
		if (server.Receive(buffer, sizeof(buffer), receivedLen) && receivedLen != 0)
		{
			ReliableConnection::MessageStatus status = server.GetMessageStatus(number);

			std::cout << buffer << "\n";

			server.Send(ReinterpretCast(byte*, toClientBuffer), sizeof(toClientBuffer), number);
		}


		ReinterpretCast(Engine::Threading::Thread*, args)->Sleep(1000);
	}
}

void Client(void *args)
{
	ReliableConnection client(&alloc, PROTOCOL_IDENTIFIER, PROTOCOL_IDENTIFIER_SIZE, BUFFER_SIZE, BUFFER_SIZE);

	client.Connect(Address(127, 0, 0, 1, PORT_NUMBER));

	static char8 toServerBuffer[] = "Do you read me ?";

	byte buffer[BUFFER_SIZE];
	uint32 receivedLen = 0;

	ReliableConnection::SequenceNumber number;

	while (true)
	{
		if (!client.Send(ReinterpretCast(byte*, toServerBuffer), sizeof(toServerBuffer), number))
		{
			GetError();
			return;
		}

		if (client.Receive(buffer, sizeof(buffer), receivedLen) && receivedLen != 0)
			std::cout << buffer << "\n";

		ReinterpretCast(Engine::Threading::Thread*, args)->Sleep(1000);
	}
}

void main()
{
	NetworkManager::GetInstance();

	Engine::Threading::Thread serverThread;
	serverThread.Initialize(Server, 512, &serverThread);

	Engine::Threading::Thread clientThread;
	clientThread.Initialize(Client, 512, &clientThread);

	system("pause");
}