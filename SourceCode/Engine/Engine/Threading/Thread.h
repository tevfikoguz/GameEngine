// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef THREAD_H
#define THREAD_H

#include <Common\PrimitiveTypes.h>
#include <Platform\PlatformThread.h>

namespace Engine
{
	using namespace Common;
	using namespace Platform;

	namespace Threading
	{
		class THREADING_API Thread
		{
		public:
			Thread(void);
			~Thread(void);

			void Initialize(PlatformThread::Procedure Procedure, uint32 StackSize, void *Arguments);

			void Wait(void);
			void Join(void);
			void Sleep(uint64 Milliseconds);
			void SetCoreAffinity(uint32 CoreIndex);

		private:
			PlatformThread::Handle m_Handle;
		};
	}
}

#endif