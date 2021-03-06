// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <ResourceSystem\ResourceHandle.h>

namespace Engine
{
	namespace Rendering
	{
		class Texture;
		class Program;
		class Mesh;
	}

	namespace FontSystem
	{
		class Font;
	}

	namespace ResourceSystem
	{
		class Text;

		template<typename T>
		class Resource
		{
		public:
			Resource(void) :
				m_Resource(nullptr)
			{
			}

			Resource(ResourceHandle<T> *Resource) :
				m_Resource(Resource)
			{
				m_Resource->Grab();
			}

			Resource(const Resource<T> &Other) :
				m_Resource(Other.m_Resource)
			{
				m_Resource->Grab();
			}

			~Resource(void)
			{
				m_Resource->Drop();
			}

			ResourceHandle<T> *GetData(void)
			{
				return m_Resource;
			}

			ResourceHandle<T> *operator *(void)
			{
				if (m_Resource == nullptr)
					return nullptr;

				return m_Resource;
			}

			T *operator ->(void)
			{
				return *m_Resource;
			}

		private:
			ResourceHandle<T> *m_Resource;
		};

		typedef Resource<Rendering::Texture> TextureResource;
		typedef Resource<Rendering::Program> ProgramResource;
		typedef Resource<Rendering::Mesh> MeshResource;
		typedef Resource<Text> TextResource;
		typedef Resource<FontSystem::Font> FontResource;
	}
}

#endif