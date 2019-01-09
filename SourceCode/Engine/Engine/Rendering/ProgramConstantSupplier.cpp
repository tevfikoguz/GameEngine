// Copyright 2016-2017 ?????????????. All Rights Reserved.
#include <Rendering\ProgramConstantSupplier.h>
#include <Rendering\IDevice.h>
#include <Rendering\Program.h>
#include <Containers\Strings.h>


#include <Utility\HighResolutionTime.h>

namespace Engine
{
	using namespace Containers;

	namespace Rendering
	{
		SINGLETON_DEFINITION(ProgramConstantSupplier)

			ProgramConstantSupplier::ProgramConstantSupplier(void)
		{
			static Utility::HighResolutionTime timer;

			RegisterFloat2Constant("_Time", []() -> AnyDataType
			{
				float32 time = timer.GetTime().GetSeconds();
				float32 sinTime = Mathematics::Sin(time);
				return Vector2F(time, sinTime);
			});
		}

		void ProgramConstantSupplier::RegisterFloatConstant(const String & Name, FetchConstantFunction Function)
		{
			m_Infos[Name] = ConstantSupplierInfo{ DataTypes::Float, std::make_shared<FetchConstantFunction>(Function) };
		}

		void ProgramConstantSupplier::RegisterFloat2Constant(const String & Name, FetchConstantFunction Function)
		{
			m_Infos[Name] = ConstantSupplierInfo{ DataTypes::Float2, std::make_shared<FetchConstantFunction>(Function) };
		}

		void ProgramConstantSupplier::RegisterFloat3Constant(const String & Name, FetchConstantFunction Function)
		{
			m_Infos[Name] = ConstantSupplierInfo{ DataTypes::Float3, std::make_shared<FetchConstantFunction>(Function) };
		}

		void ProgramConstantSupplier::RegisterMatrix4Constant(const String & Name, FetchConstantFunction Function)
		{
			m_Infos[Name] = ConstantSupplierInfo{ DataTypes::Matrix4, std::make_shared<FetchConstantFunction>(Function) };
		}

		void ProgramConstantSupplier::SupplyConstants(IDevice *Device, Program *Program) const
		{
			const uint32 programHandle = Program->GetHandle();
			const StringList &constants = Program->GetConstants();

			for each (const String &constant in constants)
			{
				if (!m_Infos.Contains(constant))
					continue;

				auto &info = m_Infos[constant];

				auto &value = (*info.Function)();

				switch (info.DataType)
				{
				case DataTypes::Float:
					Device->SetProgramFloat32(programHandle, constant, value.Get<float32>());
					break;

				case DataTypes::Float2:
					Device->SetProgramVector2(programHandle, constant, value.Get<Vector2F>());
					break;

				case DataTypes::Float3:
					Device->SetProgramVector3(programHandle, constant, value.Get<Vector3F>());
					break;

				case DataTypes::Matrix4:
					Device->SetProgramMatrix4(programHandle, constant, value.Get<Matrix4F>());
					break;
				}
			}
		}
	}
}