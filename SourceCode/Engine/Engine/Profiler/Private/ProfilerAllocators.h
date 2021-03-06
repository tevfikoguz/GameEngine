// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef PROFILER_ALLOCATORS_H
#define PROFILER_ALLOCATORS_H

#include <MemoryManagement\Allocator\FixedSizeAllocator.h>
#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>

namespace Engine
{
	using namespace MemoryManagement::Allocator;

	namespace Profiler
	{
		namespace Private
		{
			class PROFILER_API ProfilerAllocators
			{
			public:
				static DynamicSizeAllocator SampleDataListAllocator;
				static FixedSizeAllocator SampleDataAllocator;
			};
		}
	}
}

#endif