// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Rendering\Pass.h>
#include <Platform\PlatformMemory.h>

namespace Engine
{
	using namespace Platform;

	namespace Rendering
	{
		using namespace Private;

		Pass::Pass(ProgramHandle * Program)
		{
			SetProgram(Program);
		}

		void Pass::SetRenderState(const IDevice::State & State)
		{
			PlatformMemory::Copy(&State, &m_RenderState, 1);
		}

		bool Pass::SetFloat32(const String &Name, float32 Value)
		{
			return SetConstantValue(Name, Value);
		}

		bool Pass::SetVector2(const String & Name, const Vector2F & Value)
		{
			return SetConstantValue(Name, Value);
		}

		bool Pass::SetVector3(const String & Name, const Vector3F & Value)
		{
			return SetConstantValue(Name, Value);
		}

		bool Pass::SetVector4(const String & Name, const Vector4F & Value)
		{
			return SetConstantValue(Name, Value);
		}

		bool Pass::SetColor(const String &Name, Color Value)
		{
			return SetConstantValue(Name, Vector4F(Value.GetFloat32R(), Value.GetFloat32G(), Value.GetFloat32B(), Value.GetFloat32A()));
		}

		bool Pass::SetMatrix4(const String &Name, const Matrix4F &Value)
		{
			return SetConstantValue(Name, Value);
		}

		bool Pass::SetTexture(const String & Name, const Texture * Value)
		{
			return SetConstantValue(Name, ReinterpretCast(void*, ConstCast(Texture*, Value)));
		}

		bool Pass::SetTexture(const String & Name, const TextureHandle * Value)
		{
			return SetConstantValue(Name, ReinterpretCast(void*, ConstCast(TextureHandle*, Value)));
		}

		ProgramHandle * Pass::GetProgram(void)
		{
			if (**m_Program != m_LastProgramPtr)
				m_LastProgramPtr = **m_Program;

			return m_Program;
		}

		void Pass::SetProgram(ProgramHandle * Program)
		{
			m_Program = Program;

			if (m_Program == nullptr)
				return;

			m_LastProgramPtr = **m_Program;
		}

		bool Pass::SetConstantValue(const String & Name, const AnyDataType & Value)
		{
			for (uint32 i = 0; i < m_Constants.GetSize(); ++i)
			{
				auto &constant = m_Constants[i];

				if (constant.Name != Name)
					continue;

				constant.Value = Value;
				return true;
			}

			m_Constants.Add({ Name, Value });

			return true;
		}
	}
}