// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef MESH_INFO_H
#define MESH_INFO_H

#include <Rendering\Vertex.h>
#include <Containers\Vector.h>
#include <Rendering\Mesh.h>

namespace Engine
{
	using namespace Common;
	using namespace Containers;

	namespace Rendering
	{
		struct SubMeshInfo
		{
		public:
			SubMeshInfo(void) :
				Type(Mesh::SubMesh::PolygonTypes::TriangleStrip),
				Layout(Mesh::SubMesh::VertexLayouts::Position)
			{
			}

			SubMeshInfo(AllocatorBase *Allocator) :
				Type(Mesh::SubMesh::PolygonTypes::TriangleStrip),
				Layout(Mesh::SubMesh::VertexLayouts::Position),
				Vertices(Allocator, 0),
				Indices(Allocator, 0)
			{
			}

			SubMeshInfo(const SubMeshInfo &Other) :
				Type(Other.Type),
				Layout(Other.Layout),
				Vertices(Other.Vertices),
				Indices(Other.Indices)
			{
			}

			SubMeshInfo &operator =(const SubMeshInfo &Other)
			{
				Type = Other.Type;
				Layout = Other.Layout;
				Vertices = Other.Vertices;
				Indices = Other.Indices;

				return *this;
			}

			static uint16 GetLayoutIndex(Mesh::SubMesh::VertexLayouts Element);

		public:
			Mesh::SubMesh::PolygonTypes Type;
			Mesh::SubMesh::VertexLayouts Layout;

			Vector<Vertex> Vertices;
			Vector<uint32> Indices;
		};

		struct MeshInfo
		{
		public:
			MeshInfo(void)
			{
			}

			MeshInfo(AllocatorBase *Allocator) :
				SubMeshes(Allocator, 0)
			{
			}

			~MeshInfo(void)
			{
				for each (auto mesh in SubMeshes)
					DeallocateMemory(SubMeshes.GetAllocator(), mesh);

				SubMeshes.Clear();
			}

		public:
			Vector<SubMeshInfo*> SubMeshes;
		};
	}
}

#endif