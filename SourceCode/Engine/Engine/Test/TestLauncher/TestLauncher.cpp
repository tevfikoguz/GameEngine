﻿//#include <Containers\Strings.h>
//#include <Common\PrimitiveTypes.h>
//#include <MemoryManagement\Allocator\RootAllocator.h>
//#include <MemoryManagement\Allocator\DefaultAllocator.h>
//#include <MemoryManagement\Allocator\DynamicSizeAllocator.h>
//
//using namespace Engine::Common;
//
//
//static Engine::MemoryManagement::Allocator::DynamicSizeAllocator allocator1("Allocator 1", &Engine::MemoryManagement::Allocator::RootAllocator::GetInstance(), 10000);
//
//Engine::Containers::ConstString Get()
//{
//	return Engine::Containers::ConstString(&allocator1, "OK");
//}
//
//void main()
//{
//	static Engine::MemoryManagement::Allocator::DynamicSizeAllocator allocator1("Allocator 1", &Engine::MemoryManagement::Allocator::RootAllocator::GetInstance(), 10000);
//	static Engine::MemoryManagement::Allocator::DynamicSizeAllocator allocator2("Allocator 2", &Engine::MemoryManagement::Allocator::RootAllocator::GetInstance(), 10000);
//
//	Engine::Containers::WString str1(&allocator1, L"Omid123");
//	str1 = L"1";
//	str1 = L"12345678";
//
//	Engine::Containers::ConstString cstr(&allocator2, Get());
//
//	Engine::Containers::ConstString cstr1(&allocator1, Get());
//
//	/*for (int i = 0; i < 10000000; ++i)
//	{
//		WString str1(&allocator1, L"Omid123");
//		const WString str2(&allocator1, L"سلام123");
//
//		str1 = str1;
//
//		if (str2 != str1)
//		{
//			str1 = L' ' + str1 + L" " + str1 + str1 + L' ' + L" ";
//		}
//
//
//
//		WString str3(&allocator2, str1);
//
//		str3 += L"aa";
//	}*/
//}
//
////
////
////
////
////
//////class A
//////{
//////
//////public:
//////	int m_x;
//////	A(int x) :
//////		m_x(x)
//////	{
//////
//////	}
//////};
////
//////uint64 desiredSize = sizeof(A);
//////uint64 testCount = 100000000;
//////
//////DefaultAllocator defaultAllocator;
//////FixedSizeAllocator fixedAllocator(desiredSize, testCount);
//////DynamicSizeAllocator dynamicAllocator(28 * testCount);
////
////
//////void DoTest(AllocatorBase *Allocator, uint64 DesireSize, uint64 TestCount)
//////{
//////	std::chrono::nanoseconds startTime = std::chrono::high_resolution_clock::now().time_since_epoch();
//////
//////	for (uint64 i = 0; i < TestCount; ++i)
//////	{
//////		byte *handle = Allocator->Allocate(DesireSize);
//////		//Allocator->Deallocate(handle);
//////	}
//////
//////	std::chrono::nanoseconds endTime = std::chrono::high_resolution_clock::now().time_since_epoch();
//////
//////	std::cout << "For (" << TestCount << ") took " << ((endTime - startTime).count() / 1000000000.0F) << "s\n";
//////}
////
//////SharedMemory<A> GetTest()
//////{
//////	return NewSharedMemory<A>(defaultAllocator, 3);
//////}
////
//////void main()
//////{
//////	SharedMemory<int> a = NewSharedMemory<int>(defaultAllocator, 1);
//////
//////
//////	//SharedMemory<A> b(A(1));
//////	//SharedMemory<int> b(2);
//////
//////	//if (a != 1)
//////	//	b = a;
//////
//////	//if (a == b)
//////	//	a = b;
//////
//////	//a = 54;
//////
//////	//std::cout << "Start Test for DefaultAllocator" << "\n";
//////	//DoTest(&defaultAllocator, desiredSize, testCount);
//////	//std::cout << "Finish Test for DefaultAllocator" << "\n";
//////
//////	//std::cout << "Start Test for FixedSizeAllocator" << "\n";
//////	//DoTest(&fixedAllocator, 1, testCount);
//////	//std::cout << "Finish Test for FixedSizeAllocator" << "\n";
//////
//////	//std::cout << "Start Test for DynamicSizeAllocator" << "\n";
//////	//DoTest(&dynamicAllocator, desiredSize, testCount);
//////	//std::cout << "Finish Test for DynamicSizeAllocator" << "\n";
//////
//////
//////	_mm_pause();
//////}