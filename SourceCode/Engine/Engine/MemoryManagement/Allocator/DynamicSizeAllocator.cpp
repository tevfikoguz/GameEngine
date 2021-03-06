// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>
#include <MemoryManagement\Allocator\MemoryHeader.h>
#include <Debugging\Debug.h>

namespace Engine
{
	namespace MemoryManagement
	{
		namespace Allocator
		{
			DynamicSizeAllocator::DynamicSizeAllocator(cstr Name, AllocatorBase *Parent, uint64 ReserveSize) :
				CustomAllocator(Name, Parent, ReserveSize)
			{
			}

			byte *DynamicSizeAllocator::GetFromFreeList(MemoryHeader *Header, uint64 Size)
			{
				Assert(Header != nullptr, "Header cannot be null");
				Assert(Size != 0, "Size must be positive");

				MemoryHeader *header = Header;
				while (header != nullptr)
				{
					if (header->Size >= Size)
						break;

					header = header->Previous;
				}

				if (header != nullptr)
				{
					ReallocateHeader(header);

					return GetAddressFromHeader(header);
				}

				return nullptr;
			}
		}
	}
}