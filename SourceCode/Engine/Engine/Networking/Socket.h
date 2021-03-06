// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include <Common\PrimitiveTypes.h>
#include <Platform\PlatformNetwork.h>
#include <Networking\Address.h>

namespace Engine
{
	using namespace Common;
	using namespace Platform;

	namespace Networking
	{
		class NETWORKING_API Socket
		{
		public:
			Socket(void);

			~Socket(void);

			bool Open(void);

			bool Bind(uint16 port);

			bool SetNonBlocking(bool Enabled);

			void Close(void);

			bool Send(const Address &DestinationAddress, const byte *Buffer, uint32 Length);

			bool Receive(Address &SenderAddress, byte *Buffer, uint32 BufferLength, uint32 &ReceivedLength);

			bool IsOpen(void) const
			{
				return (m_Handle != 0);
			}

		private:
			PlatformNetwork::Handle m_Handle;
		};
	}
}

#endif