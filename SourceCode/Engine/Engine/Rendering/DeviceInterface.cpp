// Copyright 2016-2017 ?????????????. All Rights Reserved.
#include <Rendering\DeviceInterface.h>
#include <Rendering\RenderingManager.h>
#include <Rendering\Texture.h>
#include <Rendering\Program.h>
#include <Rendering\Mesh.h>
#include <Rendering\Window.h>
#include <Rendering\Private\Allocators.h>
#include <Rendering\Private\OpenGL\OpenGLDevice.h>
#include <Rendering\Private\ShaderCompiler\Compiler.h>
#include <Rendering\Private\Commands\Command.h>



#include <Utility\HighResolutionTime.h>

namespace Engine
{
	namespace Rendering
	{
		using namespace Private;
		using namespace Private::OpenGL;
		using namespace Private::ShaderCompiler;

#define CHECK_DEVICE() Assert(m_Device != nullptr, "m_Device cannot be null")
#define CHECK_CALL(Experssion) if (!(Experssion)) Assert(false, m_Device->GetLastError());
#define ALLOCATE_ARRAY(Type, Count) ReinterpretCast(Type*, AllocateMemory(&Allocators::RenderingSystemAllocator, Count * sizeof(Type)))
#define ALLOCATE(Type) ALLOCATE_ARRAY(Type, 1)

		DeviceInterface::DeviceInterface(Type Type) :
			m_Type(Type),
			m_Device(nullptr),
			m_Textures(&Allocators::RenderingSystemAllocator),
			m_Programs(&Allocators::RenderingSystemAllocator),
			m_Windows(&Allocators::RenderingSystemAllocator),
			m_Commands(&Allocators::RenderingSystemAllocator)
		{
		}

		DeviceInterface::~DeviceInterface(void)
		{
			for each (auto item in m_Textures)
				DestroyTexture2D(item);

			for each (auto item in m_Programs)
				DestroyProgram(item);

			for each (auto item in m_Windows)
				DestroyWindow(item);

			if (m_Device != nullptr)
			{
				m_Device->~IDevice();
				DeallocateMemory(&Allocators::RenderingSystemAllocator, m_Device);
			}
		}

		void DeviceInterface::Initialize(void)
		{
			InitializeDevice();
		}

		void DeviceInterface::SetSampleCount(uint8 Count)
		{
			CHECK_DEVICE();

			m_Device->SetSampleCount(Count);
		}

		void DeviceInterface::SetForwardCompatible(bool Value)
		{
			CHECK_DEVICE();

			m_Device->SetForwardCompatible(Value);
		}

		void DeviceInterface::SetClearColor(Color Color)
		{
			CHECK_DEVICE();

			m_Device->SetClearColor(Color);
		}

		void DeviceInterface::SetClearFlags(IDevice::ClearFlags Flags)
		{
			CHECK_DEVICE();

			m_Device->SetClearFlags(Flags);
		}

		Texture *DeviceInterface::CreateTexture2D(const byte * Data, uint32 Width, uint32 Height)
		{
			CHECK_DEVICE();

			Texture::Handle handle;
			CHECK_CALL(m_Device->CreateTexture2D(Data, Width, Height, handle));

			Texture *texture = ALLOCATE(Texture);
			new (texture) Texture(m_Device, handle);

			m_Textures.Add(texture);

			return texture;
		}

		void DeviceInterface::DestroyTexture2D(Texture *Texture)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyTexture2D(Texture->GetHandle()));
			Texture->~Texture();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Texture);
		}

		Program *DeviceInterface::CreateProgram(const String &Shader)
		{
			static Compiler compiler;

			CHECK_DEVICE();

			String vertProgram;
			String fragProgram;
			compiler.Compile(m_Type, Shader, vertProgram, fragProgram);

			Program::Handle handle;
			CHECK_CALL(m_Device->CreateProgram(vertProgram.GetValue(), fragProgram.GetValue(), handle));

			Program *program = ALLOCATE(Program);
			new (program) Program(m_Device, handle);

			m_Programs.Add(program);

			return program;
		}

		void DeviceInterface::DestroyProgram(Program *Program)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyProgram(Program->GetHandle()));
			Program->~Program();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Program);
		}

		Mesh *DeviceInterface::CreateMesh(MeshInfo *Info, IDevice::BufferUsages Usage)
		{
			CHECK_DEVICE();

			Mesh::SubMesh *subMeshes = ALLOCATE_ARRAY(Mesh::SubMesh, Info->SubMeshCount);

			for (uint16 i = 0; i < Info->SubMeshCount; ++i)
			{
				GPUBuffer::Handle handle;

				auto &subMeshInfo = Info->SubMeshes[i];

				CHECK_CALL(m_Device->CreateMesh(&subMeshInfo, Usage, handle));

				new (&subMeshes[i]) Mesh::SubMesh(GPUBuffer(m_Device, handle, subMeshInfo.VertexCount), subMeshInfo.IndexCount);
			}

			Mesh *mesh = ALLOCATE(Mesh);
			new (mesh) Mesh(subMeshes, Info->SubMeshCount);
			return mesh;
		}

		void DeviceInterface::DestroyMesh(Mesh *Mesh)
		{
			//CHECK_DEVICE();

			//CHECK_CALL(m_Device->DestroyBuffer(Mesh->GetVertices().GetHandle()));
			//CHECK_CALL(m_Device->DestroyBuffer(Mesh->GetUVs().GetHandle()));
			//Mesh->~Mesh();
			//DeallocateMemory(&Allocators::RenderingSystemAllocator, Mesh);
		}

		Window *DeviceInterface::CreateWindow(uint16 Width, uint16 Height, cstr Title)
		{
			CHECK_DEVICE();

			Window::Handle handle;
			CHECK_CALL(m_Device->CreateWindow(Width, Height, Title, handle));

			Window *window = ALLOCATE(Window);
			new (window) Window(m_Device, handle);

			m_Windows.Add(window);

			return window;
		}

		void DeviceInterface::DestroyWindow(Window *Window)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyWindow(Window->GetHandle()));
			Window->~Window();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Window);
		}

		void DeviceInterface::DrawMesh(Mesh *Mesh, Program *Program)
		{
			CHECK_DEVICE();

			SupplyProgramPresetConstants(Program);

			CHECK_CALL(m_Device->BindProgram((Program == nullptr ? 0 : Program->GetHandle())));

			for (uint16 i = 0; i < Mesh->GetSubMeshCount(); ++i)
			{
				Mesh::SubMesh &subMesh = Mesh->GetSubMeshes()[i];

				CHECK_CALL(m_Device->BindBuffer(subMesh.GetBuffer().GetHandle()));

				m_Device->Draw(IDevice::DrawModes::Triangles, subMesh.GetIndexCount());
			}
		}

		void DeviceInterface::BeginRender(void)
		{
			CHECK_DEVICE();

			m_Device->Clear();

			for each (auto command in m_Commands)
				command->Execute(m_Device);
		}

		void DeviceInterface::EndRender(void)
		{
			CHECK_DEVICE();

			for each (auto window in m_Windows)
				m_Device->SwapBuffers(window->GetHandle());

			m_Device->PollEvents();
		}

		void DeviceInterface::InitializeDevice(void)
		{
			switch (m_Type)
			{
			case Type::OpenGL:
			{
				m_Device = ALLOCATE(OpenGLDevice);
				new (m_Device) OpenGLDevice;
			} break;
			}

			CHECK_DEVICE();

			CHECK_CALL(m_Device->Initialize());
		}

		void DeviceInterface::SupplyProgramPresetConstants(Program * Program)
		{
			static Utility::HighResolutionTime timer;

			const StringList &constants = Program->GetConstants();

			for each (const String &constant in constants)
			{
				if (constant == "_Time")
					m_Device->SetProgramVector3(Program->GetHandle(), constant, Vector3F(timer.GetTime().GetSeconds(), 0, 0));
			}
		}
	}
}