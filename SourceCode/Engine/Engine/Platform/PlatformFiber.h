// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef PLATFORM_FIBER_H
#define PLATFORM_FIBER_H

#include <Common\PrimitiveTypes.h>

namespace Engine
{
	using namespace Common;

	namespace Platform
	{
		//FlsAlloc
		//FlsFree
		//FlsGetValue
		//FlsSetValue

		class PLATFORM_API PlatformFiber
		{
		public:
			typedef int64 * Handle;
			typedef void(*Procedure)(void*);

		public:
			static Handle Create(Procedure Procedure, uint32 StackSize, void *Arguments);
			static void Delete(Handle Fiber);
			static void Switch(Handle Fiber);

			static void *GetData(void);

			static Handle ConvertThreadToFiber(void *Arguments);
		};
	}
}

#endif