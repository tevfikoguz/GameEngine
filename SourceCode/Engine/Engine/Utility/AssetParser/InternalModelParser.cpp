// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Utility\AssetParser\InternalModelParser.h>
#include <Containers\StringUtility.h>
#include <Common\BitwiseUtils.h>

namespace Engine
{
	using namespace Common;
	using namespace Platform;
	using namespace Containers;
	using namespace Rendering;

	namespace Utility
	{
		namespace AssetParser
		{

			void ParseSubMeshInfo(const ByteBuffer &Buffer, SubMeshInfo &SubMeshInfo, uint64 &Index)
			{
#define READ_VALUE(Type) \
			Buffer.ReadValue<Type>(Index);\
			Index += sizeof(Type);

				SubMeshInfo.Type = (Mesh::SubMesh::PolygonTypes)READ_VALUE(int32);

				SubMeshInfo.Layout = (Mesh::SubMesh::VertexLayouts)READ_VALUE(int32);

				uint32 vertexCount = READ_VALUE(uint32);

				SubMeshInfo.Vertices.Recap(vertexCount);

				for (uint32 j = 0; j < vertexCount; ++j)
				{
					Vector3F pos;
					pos.X = READ_VALUE(float32);
					pos.Y = READ_VALUE(float32);
					pos.Z = READ_VALUE(float32);

					Vector3F norm;
					norm.X = READ_VALUE(float32);
					norm.Y = READ_VALUE(float32);
					norm.Z = READ_VALUE(float32);

					Vector2F uv;
					uv.X = READ_VALUE(float32);
					uv.Y = READ_VALUE(float32);

					SubMeshInfo.Vertices.Add({ pos, norm, uv });
				}

				uint32 idxCount = READ_VALUE(uint32);

				SubMeshInfo.Indices.Recap(idxCount);

				for (uint32 j = 0; j < idxCount; ++j)
				{
					uint32 idx = READ_VALUE(uint32);

					SubMeshInfo.Indices.Add(idx);
				}

#undef READ_VALUE
			}

			void InternalModelParser::Parse(const ByteBuffer &Buffer, MeshInfo &MeshInfo)
			{
				uint64 index = 0;
				uint32 subMeshCount = Buffer.ReadValue<uint32>(index);
				index += sizeof(uint32);

				for (uint32 i = 0; i < subMeshCount; ++i)
				{
					SubMeshInfo *subMeshInfo = ReinterpretCast(SubMeshInfo*, AllocateMemory(MeshInfo.SubMeshes.GetAllocator(), sizeof(SubMeshInfo)));

					Construct(subMeshInfo, MeshInfo.SubMeshes.GetAllocator());

					ParseSubMeshInfo(Buffer, *subMeshInfo, index);

					MeshInfo.SubMeshes.Add(subMeshInfo);
				}
			}

			void InternalModelParser::Parse(const ByteBuffer &Buffer, SubMeshInfo &SubMeshInfo)
			{
				uint64 index = 0;

				uint32 subMeshCount = Buffer.ReadValue<uint32>(index);
				index += sizeof(uint32);

				ParseSubMeshInfo(Buffer, SubMeshInfo, index);
			}

			void InternalModelParser::Dump(ByteBuffer &Buffer, MeshInfo & MeshInfo)
			{
				uint32 desiredSize = 0;
				for each (auto &subMeshPtr in MeshInfo.SubMeshes)
				{
					auto &subMesh = *subMeshPtr;

					desiredSize += sizeof(int32);
					desiredSize += sizeof(int32);

					desiredSize += sizeof(int32);
					desiredSize += subMesh.Vertices.GetSize() * sizeof(Vertex);

					desiredSize += sizeof(int32);
					desiredSize += subMesh.Indices.GetSize() * sizeof(uint32);
				}

				Buffer.Recap(Buffer.GetSize() + desiredSize);

				Buffer.Append(MeshInfo.SubMeshes.GetSize());
				for each (auto &subMeshPtr in MeshInfo.SubMeshes)
				{
					auto &subMesh = *subMeshPtr;

					Buffer.Append((int32)subMesh.Type);
					Buffer.Append((int32)subMesh.Layout);

					Buffer.Append(subMesh.Vertices.GetSize());
					for each (auto &vertex in subMesh.Vertices)
					{
						Buffer.Append(vertex.Position.X);
						Buffer.Append(vertex.Position.Y);
						Buffer.Append(vertex.Position.Z);

						Buffer.Append(vertex.Normal.X);
						Buffer.Append(vertex.Normal.Y);
						Buffer.Append(vertex.Normal.Z);

						Buffer.Append(vertex.UV.X);
						Buffer.Append(vertex.UV.Y);
					}

					Buffer.Append(subMesh.Indices.GetSize());
					for each (auto &index in subMesh.Indices)
						Buffer.Append(index);
				}
			}
		}
	}
}