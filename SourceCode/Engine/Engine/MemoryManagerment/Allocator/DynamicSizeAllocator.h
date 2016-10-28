// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#include <MemoryManagerment\Allocator\AllocatorBase.h>

#ifndef DYNAMIC_SIZE_ALLOCATOR_H
#define DYNAMIC_SIZE_ALLOCATOR_H

namespace Engine
{
	namespace MemoryManagement
	{
		namespace Allocator
		{
			class DynamicSizeAllocator : public AllocatorBase
			{
			public:
				DynamicSizeAllocator(uint32 ReserveSize);

			public:
				HandleInfo *Allocate(uint32 Size);

				void Deallocate(HandleInfo *Handle) override;

				void Update(void);

			private:
				HandleInfo *GetFirstHandle(void) const;

			private:
				uint32 m_ReserveSize;
				HandleInfo *m_LastHandleInfo;
			};
		}
	}
}

#endif