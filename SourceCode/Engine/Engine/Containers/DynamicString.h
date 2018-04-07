// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <Common\StringUtility.h>
#include <MemoryManagement\Allocator\AllocatorBase.h>
#include <Platform\PlatformMemory.h>
#include <Containers\Vector.h>
#include <Containers\Private\Allocators.h>

namespace Engine
{
	using namespace Platform;

	namespace Containers
	{
		using namespace Private;

		template<typename T>
		class DynamicString
		{
		public:
			typedef T CharType;

		public:
			DynamicString(void) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
			}

			DynamicString(const T Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				SetValue(&Value, 1);
			}

			DynamicString(const T *Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				SetValue(Value);
			}

			template<typename T>
			DynamicString(const DynamicString<T> &Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				SetValue(Value.m_String);
			}

			DynamicString(const DynamicString<T> &Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				SetValue(Value.m_String);
			}

			template<typename T>
			DynamicString(DynamicString<T> &&Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				Move(Value);
			}

			DynamicString(DynamicString<T> &&Value) :
				m_String(nullptr),
				m_Length(0),
				m_Capacity(0)
			{
				Move(Value);
			}

			~DynamicString(void)
			{
				SetValue(nullptr, 0);
			}

			INLINE DynamicString<T> Replace(const DynamicString<T> &OldValue, const DynamicString<T> &NewValue) const
			{
				T *result = Allocate(m_Length + (m_Length * (NewValue.m_Length - 1)) + 1);

				uint32 newIndex = 0;
				for (uint32 i = 0; i < m_Length; ++i)
				{
					if (Compare(i, OldValue))
					{
						PlatformMemory::Copy(NewValue.m_String, 0, result, newIndex, NewValue.m_Length);

						i += OldValue.m_Length - 1;
						newIndex += NewValue.m_Length;

						continue;
					}

					result[newIndex++] = m_String[i];
				}

				result[newIndex] = StringUtility::Character<T, '\0'>::Value;

				DynamicString<T> value(result);

				DeallocateMemory(&Allocators::DynamicStringAllocator, result);

				return value;
			}

			INLINE DynamicString<T> SubString(uint32 StartIndex) const
			{
				return SubString(StartIndex, m_Length - StartIndex);
			}

			INLINE DynamicString<T> SubString(uint32 StartIndex, uint32 Length) const
			{
				Assert(StartIndex < m_Length, "StartIndex must be less than m_Length");
				Assert(Length != 0, "Length cannot be zero");

				DynamicString<T> newValue;
				newValue.SetValue(&m_String[StartIndex], Length);
				return newValue;
			}

			INLINE Vector<DynamicString<T>> Split(const DynamicString<T> &Splitter) const
			{
				Vector<DynamicString<T>> result;

				int32 prevIndex = 0;
				int32 index = 0;
				while ((index = FirstIndexOf(Splitter, index)) != -1)
				{
					result.Add(SubString(prevIndex, index - prevIndex));

					prevIndex = index + Splitter.m_Length;
					++index;
				}

				if (prevIndex != m_Length - 1)
					result.Add(SubString(prevIndex, m_Length - prevIndex));

				return result;
			}

			INLINE int32 FirstIndexOf(const DynamicString<T> &Value, uint32 StartIndex = 0) const
			{
				Assert(StartIndex < m_Length, "StartIndex must be less than m_Length");

				if (Value.m_Length == 0)
					return -1;

				for (uint32 i = StartIndex; i < m_Length; ++i)
					if (Compare(i, Value))
						return i;

				return -1;
			}

			INLINE int32 LastIndexOf(const DynamicString<T> &Value, uint32 StartIndex = 0) const
			{
				Assert(StartIndex < m_Length, "StartIndex must be less than m_Length");

				if (Value.m_Length == 0)
					return -1;

				if (StartIndex == 0)
					StartIndex = m_Length - 1;

				for (int32 i = StartIndex - Value.m_Length; i >= 0; --i)
					if (Compare(i, Value))
						return i;

				return -1;
			}

			INLINE bool StartsWith(const DynamicString<T> &Value) const
			{
				return (LastIndexOf(Value, Value.m_Length) != -1);
			}

			INLINE bool EndsWith(const DynamicString<T> &Value) const
			{
				return (FirstIndexOf(Value, m_Length - Value.m_Length) != -1);
			}

			INLINE bool Contains(const DynamicString<T> &Value) const
			{
				return (FirstIndexOf(Value, 0) != -1);
			}

			INLINE DynamicString<T> & operator = (const T Value)
			{
				SetValue(&Value, 1);

				return *this;
			}

			INLINE DynamicString<T> & operator = (const T *Value)
			{
				SetValue(Value);

				return *this;
			}

			INLINE DynamicString<T> & operator = (const DynamicString<T> &Value)
			{
				if (m_String != Value.m_String)
					SetValue(Value.m_String);

				return *this;
			}

			INLINE DynamicString<T> & operator = (DynamicString<T> &&Value)
			{
				if (m_String != Value.m_String)
					Move(Value);

				return *this;
			}

			INLINE DynamicString<T> & operator += (const T Value)
			{
				Append(&Value, 1);

				return *this;
			}

			INLINE DynamicString<T> & operator += (const T *Value)
			{
				Append(Value);

				return *this;
			}

			INLINE DynamicString<T> & operator += (const DynamicString<T> &Value)
			{
				Append(Value.m_String);

				return *this;
			}

			INLINE bool operator == (const T *Value) const
			{
				if (m_String == Value)
					return true;

				uint32 length = StringUtility::GetLength(Value);

				if (m_Length != length)
					return false;

				return StringUtility::AreEquals(m_String, Value);
			}

			INLINE bool operator == (const DynamicString<T> &Value) const
			{
				if (m_String == Value.m_String)
					return true;

				if (m_Length != Value.m_Length)
					return false;

				return StringUtility::AreEquals(m_String, Value.m_String);
			}

			INLINE bool operator != (const T *Value) const
			{
				return !(*this == Value);
			}

			INLINE bool operator != (const DynamicString<T> &Value) const
			{
				return !(*this == Value);
			}

			INLINE const T *GetValue(void) const
			{
				return m_String;
			}

			INLINE uint32 GetLength(void) const
			{
				return m_Length;
			}

		private:
			INLINE bool Compare(uint32 Index, const DynamicString<T> &Value) const
			{
				for (uint32 j = 0; j < Value.m_Length; ++j)
					if (m_String[Index + j] != Value.m_String[j])
						return false;

				return true;
			}

			INLINE void SetValue(const T *Value)
			{
				SetValue(Value, StringUtility::GetLength(Value));
			}

			INLINE void SetValue(const T *Value, uint32 Length)
			{
				if (Length == 0)
				{
					Deallocate();
					m_String = nullptr;
					m_Length = 0;
					m_Capacity = 0;
					return;
				}
				else if (Length > m_Capacity)
				{
					Deallocate();
					m_Capacity = Length;

					m_String = Allocate(sizeof(T) * (Length + 1));
				}

				m_Length = Length;

				PlatformMemory::Copy((byte*)Value, (byte*)m_String, sizeof(T) * m_Length);
				m_String[m_Length] = StringUtility::Character<T, '\0'>::Value;
			}

			INLINE void Move(DynamicString<T> &Value)
			{
				Deallocate();

				m_String = Value.m_String;
				m_Length = Value.m_Length;
				m_Capacity = Value.m_Capacity;

				Value.m_String = nullptr;
				Value.m_Length = 0;
				Value.m_Capacity = 0;
			}

			INLINE void Append(const T *Value)
			{
				Append(Value, StringUtility::GetLength(Value));
			}

			INLINE void Append(const T *Value, uint32 Length)
			{
				if (Length == 0)
					return;

				uint32 newLength = m_Length + Length;
				uint32 newSize = sizeof(T) * (newLength + 1);

				bool allocateNewBuffer = (newLength > m_Capacity);

				T *newMemory = (allocateNewBuffer ? Allocate(newSize) : m_String);

				uint32 size = sizeof(T) * m_Length;

				if (allocateNewBuffer && m_String != nullptr)
					PlatformMemory::Copy((byte*)m_String, 0, (byte*)newMemory, 0, size);

				PlatformMemory::Copy((byte*)Value, 0, (byte*)newMemory, size, sizeof(T) * (Length));
				newMemory[newLength] = StringUtility::Character<T, '\0'>::Value;

				if (allocateNewBuffer)
				{
					Deallocate();

					m_String = newMemory;
					m_Capacity = newLength;
				}

				m_Length = newLength;
			}

			INLINE void Deallocate(void)
			{
				if (m_String != nullptr)
					DeallocateMemory(&Allocators::DynamicStringAllocator, m_String);
			}

			T *Allocate(uint32 Size) const
			{
				return reinterpret_cast<T*>(AllocateMemory(&Allocators::DynamicStringAllocator, Size));
			}

			template<typename T>
			INLINE friend DynamicString<T> operator + (const T LeftValue, const DynamicString<T> &RightValue)
			{
				DynamicString<T> value(LeftValue);
				value += RightValue;
				return value;
			}

			template<typename T>
			INLINE friend DynamicString<T> operator + (const T *LeftValue, const DynamicString<T> &RightValue)
			{
				DynamicString<T> value(LeftValue);
				value += RightValue;
				return value;
			}

			template<typename T>
			INLINE friend DynamicString<T> operator + (const DynamicString<T> &LeftValue, const T RightValue)
			{
				DynamicString<T> value(LeftValue);
				value += RightValue;
				return value;
			}

			template<typename T>
			INLINE friend DynamicString<T> operator + (const DynamicString<T> &LeftValue, const T *RightValue)
			{
				DynamicString<T> value(LeftValue);
				value += RightValue;
				return value;
			}

			template<typename T>
			INLINE friend DynamicString<T> operator + (const DynamicString<T> &LeftValue, const DynamicString<T> &RightValue)
			{
				DynamicString<T> value(LeftValue);
				value += RightValue;
				return value;
			}

		private:
			T * m_String;
			uint32 m_Length;
			uint32 m_Capacity;
		};
	}
}

#endif