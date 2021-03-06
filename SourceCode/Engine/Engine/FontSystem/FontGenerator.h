// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <MemoryManagement\Singleton.h>
#include <Containers\Strings.h>

namespace Engine
{
	using namespace Containers;

	namespace FontSystem
	{
		class FONTSYSTEM_API FontGenerator
		{
		public:
			FontGenerator(void);
			~FontGenerator(void);

			void LoadFont(const byte *Data, uint32 Size);

			void Generate(const WString &Path);
		};
	}
}

#endif