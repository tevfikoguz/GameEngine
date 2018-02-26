// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>

namespace Engine
{
	using namespace MemoryManagement::Allocator;

	namespace Rendering
	{
		namespace Private
		{
			class RENDERING_API Allocators
			{
			public:
				static DynamicSizeAllocator RenderingSystemAllocator;
			};
		}
	}
}

#endif