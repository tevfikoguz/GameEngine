// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

namespace Engine
{
	namespace ResourceSystem
	{
		class RESOURCESYSTEM_API Resource
		{
		public:
			enum class Types
			{
				Texture = 0,
				Unknown
			};

		public:
			Resource(void)
			{
			}
		};
	}
}

#endif