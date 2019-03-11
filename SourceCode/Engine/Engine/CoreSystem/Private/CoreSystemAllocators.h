// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#ifndef CORE_SYSTEM_ALLOCATORS_H
#define CORE_SYSTEM_ALLOCATORS_H

#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>
#include <MemoryManagement\Allocator\FrameAllocator.h>

namespace Engine
{
	using namespace MemoryManagement::Allocator;

	namespace CoreSystem
	{
		namespace Private
		{
			class CORESYSTEM_API CoreSystemAllocators
			{
			public:
				static DynamicSizeAllocator CoreSystemAllocator;
			};
		}
	}
}

#endif