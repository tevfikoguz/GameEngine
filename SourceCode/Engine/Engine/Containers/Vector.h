// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <Common\PrimitiveTypes.h>
#include <MemoryManagement\Allocator\AllocatorBase.h>
#include <Platform\PlatformMemory.h>

namespace Engine
{
	using namespace Common;
	using namespace Platform;
	using namespace MemoryManagement::Allocator;

	namespace Containers
	{
		template<typename T>
		class Vector
		{
		public:
			typedef T ItemType;

			class Iterator
			{
			public:
				Iterator(T *Pointer) :
					m_Pointer(Pointer)
				{
				}

				Iterator operator++(void)
				{
					++m_Pointer;
					return *this;
				}

				Iterator operator--(void)
				{
					--m_Pointer;
					return *this;
				}

				bool operator==(const Iterator &Other) const
				{
					return (m_Pointer == Other.m_Pointer);
				}

				bool operator!=(const Iterator &Other) const
				{
					return !operator==(Other);
				}

				T &operator*(void)
				{
					return *m_Pointer;
				}

			private:
				T *m_Pointer;
			};

			class ConstIterator
			{
			public:
				ConstIterator(T *Pointer) :
					m_Pointer(Pointer)
				{
				}

				ConstIterator operator++(void)
				{
					++m_Pointer;
					return *this;
				}

				ConstIterator operator--(void)
				{
					--m_Pointer;
					return *this;
				}

				bool operator==(const ConstIterator &Other) const
				{
					return (m_Pointer == Other.m_Pointer);
				}

				bool operator!=(const ConstIterator &Other) const
				{
					return !operator==(Other);
				}

				const T &operator*(void) const
				{
					return *m_Pointer;
				}

			private:
				T *m_Pointer;
			};

		public:
			Vector(void) :
				m_Capacity(0),
				m_Size(0),
				m_Items(nullptr),
				m_Allocator(nullptr)
			{
			}

			~Vector(void)
			{
				if (m_Items == nullptr)
					return;

				Deallocate();
			}

			INLINE void Add(const T &Item)
			{
				uint32 index = Extend(1);

				m_Items[index] = Item;
			}

			INLINE void AddRange(const T *Items, uint32 Count)
			{
				uint32 index = Extend(Count);

				PlatformMemory::Copy(Items, 0, m_Items, index, Count);
			}

			INLINE void AddRange(const Vector<T> &Other)
			{
				AddRange(Other.m_Items, Other.m_Size);
			}

			INLINE void AddRange(const Vector<T> &&Other)
			{
				AddRange(Other.m_Items, Other.m_Size);
			}

			INLINE void Insert(uint32 Index, const T &Item)
			{
				Assert(Index < m_Size, "Index cannot be greater-equal with m_Size");

				Extend(1);

				int indexToInsert = Index + 1;

				PlatformMemory::Copy(m_Items, Index, m_Items, indexToInsert, m_Size - indexToInsert);

				m_Items[Index] = Item;
			}

			INLINE void Remove(const T &Item)
			{
			}

			INLINE void RemoveAt(uint32 Index)
			{
				Assert(Index < m_Size, "Index cannot be greater-equal with m_Size");

				int indexToMove = Index + 1;

				if (indexToMove < m_Size)
					PlatformMemory::Copy(m_Items, indexToMove, m_Items, Index, m_Size - indexToMove);

				--m_Size;
			}

			INLINE void Clear()
			{
				m_Size = 0;
			}

			INLINE bool Contains(const T &Item) const
			{
			}

			INLINE Iterator GetBegin(void)
			{
				return Iterator(m_Items);
			}

			INLINE ConstIterator GetBegin(void) const
			{
				return ConstIterator(m_Items);
			}

			INLINE Iterator GetEnd(void)
			{
				return Iterator(m_Items + m_Size);
			}

			INLINE ConstIterator GetEnd(void) const
			{
				return ConstIterator(m_Items + m_Size);
			}

			INLINE Iterator begin()
			{
				return GetBegin();
			}

			INLINE ConstIterator begin(void) const
			{
				return GetBegin();
			}

			INLINE Iterator end(void)
			{
				return GetEnd();
			}

			INLINE ConstIterator end(void) const
			{
				return GetEnd();
			}

			T &operator[](uint32 Index)
			{
				Assert(Index < m_Size, "Index cannot be greater-equal with m_Size");

				return m_Items[Index];
			}

			const T &operator[](uint32 Index) const
			{
				Assert(Index < m_Size, "Index cannot be greater-equal with m_Size");

				return m_Items[Index];
			}

			INLINE uint32 GetCapacity(void) const
			{
				return m_Capacity;
			}

			INLINE uint32 GetSize(void) const
			{
				return m_Size;
			}

			INLINE const T *GetData(void) const
			{
				return m_Items;
			}

		private:
			INLINE uint32 Extend(uint32 Count)
			{
				if (m_Size + Count <= m_Capacity)
				{
					++m_Size;
					return (m_Size - Count);
				}

				Reacllocate(m_Capacity + Count);

				m_Size = m_Capacity;

				return (m_Size - Count);
			}

			INLINE void Reacllocate(uint32 Count)
			{
				T *newMem = Allocate(Count);

				if (m_Items == nullptr)
				{
					m_Capacity = Count;
					m_Items = newMem;
					return;
				}

				PlatformMemory::Copy(m_Items, newMem, m_Size);

				DeallocateMemory(m_Allocator, m_Items);

				m_Capacity = Count;
				m_Items = newMem;
			}

			INLINE T *Allocate(uint32 Count)
			{
				if (m_Allocator == nullptr)
				{
					static DynamicSizeAllocator allocator("Default Vector Allocator", RootAllocator::GetInstance(), 1024 * 1024);

					m_Allocator = &allocator;
				}

				return reinterpret_cast<T*>(AllocateMemory(m_Allocator, Count * sizeof(T)));
			}

			INLINE void Deallocate(void)
			{
				m_Capacity = 0;
				m_Size = 0;
				DeallocateMemory(m_Allocator, m_Items);
			}

		private:
			uint32 m_Capacity;
			uint32 m_Size;
			T *m_Items;
			AllocatorBase *m_Allocator;
		};
	}
}

#endif