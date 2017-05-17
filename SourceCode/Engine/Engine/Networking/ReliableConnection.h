// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#include <Networking\Connection.h>

#ifndef RELIABLE_CLIENT_CONNECTION_H
#define RELIABLE_CLIENT_CONNECTION_H

namespace Engine
{
	using namespace Common;
	using namespace MemoryManagement::Allocator;

	namespace Networking
	{
		class NETWORKING_API ReliableConnection : public Connection
		{
		public:
			ReliableConnection(AllocatorBase *Allocator, const byte *Identifier, uint8 IdentifierLength, uint32 SendBufferSize, uint32 ReceiveBufferSize);

		protected:
			virtual bool SendMessage(const byte *Buffer, uint32 BufferLength) override;
			virtual	void ReceiveMessage(byte *Buffer, uint32 BufferLength) override;

		private:
			uint32 m_SequenceNumebr;
			uint32 m_RemoteSequenceNumebr;
		};
	}
}

#endif