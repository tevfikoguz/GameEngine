#ifndef PAIRSOCKET_H
#define PAIRSOCKET_H

#include "SocketBase.h"

namespace NetworkEngine
{
	class NETWORK_ENGINE_API PushSocket : public SocketBase
	{
	public:
		PushSocket(Context *Context, int BufferSize);
	};
}

#endif