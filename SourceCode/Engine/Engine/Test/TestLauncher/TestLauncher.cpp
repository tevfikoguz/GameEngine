﻿#include <Containers\DynamicString.h>
#include <Common\PrimitiveTypes.h>
#include <MemoryManagement\Allocator\DefaultAllocator.h>

using namespace Engine::Common;

typedef Engine::Containers::DynamicString<char8> String;
typedef Engine::Containers::DynamicString<char16> WString;

String Get()
{
	return "OK";
}

void main()
{
	String str1 = "Omid123";
	String str2 = "Ali";


	str1 = Get();



	WString str3 = L"سلام123";
}






//class A
//{
//
//public:
//	int m_x;
//	A(int x) :
//		m_x(x)
//	{
//
//	}
//};

//uint64 desiredSize = sizeof(A);
//uint64 testCount = 100000000;
//
//DefaultAllocator defaultAllocator;
//FixedSizeAllocator fixedAllocator(desiredSize, testCount);
//DynamicSizeAllocator dynamicAllocator(28 * testCount);


//void DoTest(AllocatorBase *Allocator, uint64 DesireSize, uint64 TestCount)
//{
//	std::chrono::nanoseconds startTime = std::chrono::high_resolution_clock::now().time_since_epoch();
//
//	for (uint64 i = 0; i < TestCount; ++i)
//	{
//		byte *handle = Allocator->Allocate(DesireSize);
//		//Allocator->Deallocate(handle);
//	}
//
//	std::chrono::nanoseconds endTime = std::chrono::high_resolution_clock::now().time_since_epoch();
//
//	std::cout << "For (" << TestCount << ") took " << ((endTime - startTime).count() / 1000000000.0F) << "s\n";
//}

//SharedMemory<A> GetTest()
//{
//	return NewSharedMemory<A>(defaultAllocator, 3);
//}

//void main()
//{
//	SharedMemory<int> a = NewSharedMemory<int>(defaultAllocator, 1);
//
//
//	//SharedMemory<A> b(A(1));
//	//SharedMemory<int> b(2);
//
//	//if (a != 1)
//	//	b = a;
//
//	//if (a == b)
//	//	a = b;
//
//	//a = 54;
//
//	//std::cout << "Start Test for DefaultAllocator" << "\n";
//	//DoTest(&defaultAllocator, desiredSize, testCount);
//	//std::cout << "Finish Test for DefaultAllocator" << "\n";
//
//	//std::cout << "Start Test for FixedSizeAllocator" << "\n";
//	//DoTest(&fixedAllocator, 1, testCount);
//	//std::cout << "Finish Test for FixedSizeAllocator" << "\n";
//
//	//std::cout << "Start Test for DynamicSizeAllocator" << "\n";
//	//DoTest(&dynamicAllocator, desiredSize, testCount);
//	//std::cout << "Finish Test for DynamicSizeAllocator" << "\n";
//
//
//	_mm_pause();
//}