// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#include <Common\PrimitiveTypes.h>

#ifndef PLATFORM_MEMORY_H
#define PLATFORM_MEMORY_H

namespace Engine
{
	using namespace Common;

	namespace Platform
	{
		class PlatformMemory
		{
		public:
			static byte *Allocate(uint64 Size);

			static void Free(byte *Address);

			static void Set(byte *Address, int32 Value, uint64 Size);

			static void Copy(const byte *Source, byte *Destination, uint64 Size);
		};
	}
}

#endif