// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <Rendering\NativeType.h>
#include <Rendering\Color.h>
#include <Containers\Strings.h>
#include <MathContainers\MathContainers.h>
#include <Rendering\DataType.h>
#include <Containers\AnyDataType.h>

namespace Engine
{
	using namespace Containers;

	namespace Rendering
	{
		class Texture;

		class RENDERING_API Program : public NativeType
		{
			friend class DeviceInterface;

		public:
			typedef int32 ConstantHandle;

			struct ConstantInfo
			{
			public:
				String Name;
				AnyDataType Value;
			};

			struct ConstantData : public ConstantInfo
			{
			public:
				DataType::Types Type;
				ConstantHandle Handle;
			};

			typedef Vector<ConstantInfo> ConstantInfoList;
			typedef Vector<ConstantData> ConstantDataList;

		private:
			Program(IDevice *Device, Handle Handle);

		public:
			bool SetFloat32(Program::ConstantHandle Handle, float32 Value);
			bool SetVector2(Program::ConstantHandle Handle, const Vector2F &Value);
			bool SetVector3(Program::ConstantHandle Handle, const Vector3F &Value);
			bool SetVector4(Program::ConstantHandle Handle, const Vector4F &Value);
			bool SetMatrix4(Program::ConstantHandle Handle, const Matrix4F &Value);
			bool SetTexture(Program::ConstantHandle Handle, const Texture *Value);

			bool SetFloat32(const String &Name, float32 Value);
			bool SetVector2(const String &Name, const Vector2F &Value);
			bool SetVector3(const String &Name, const Vector3F &Value);
			bool SetVector4(const String &Name, const Vector4F &Value);
			bool SetMatrix4(const String &Name, const Matrix4F &Value);
			bool SetTexture(const String &Name, const Texture *Value);

			void ApplyConstantValue(const ConstantInfoList &Constants);

			INLINE ConstantDataList &GetConstants(void)
			{
				return m_Constants;
			}

			INLINE const ConstantDataList &GetConstants(void) const
			{
				return m_Constants;
			}

		private:
			ConstantData * GetConstantData(const String &Name);

		private:
			void QueryActiveConstants(void);

		private:
			ConstantDataList m_Constants;
		};

		typedef ResourceSystem::ResourceHandle<Program> ProgramHandle;
	}
}

#endif