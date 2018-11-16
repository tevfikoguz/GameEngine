// Copyright 2016-2017 ?????????????. All Rights Reserved.
#include <Rendering\RenderingManager.h>
#include <Rendering\Texture.h>
#include <Rendering\Program.h>
#include <Rendering\Mesh.h>
#include <Rendering\Window.h>
#include <Rendering\Private\Allocators.h>
#include <Rendering\Private\OpenGL\OpenGLDevice.h>
#include <Rendering\Private\ShaderCompiler\Compiler.h>
#include <Rendering\Private\Commands\Command.h>

namespace Engine
{
	// OK
	//vertProgram =  "#version 330 core\n"
	// "layout (location = 0) in vec3 aPos;\n"
	// "void main()\n"
	// "{\n"
	// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	//	"}\0";;
	//fragProgram = "#version 330 core\n"
	// "out vec4 FragColor;\n"
	// "void main()\n"
	// "{\n"
	// "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	// "}\n\0";


	namespace Rendering
	{
		using namespace Private;
		using namespace Private::OpenGL;
		using namespace Private::ShaderCompiler;

#define CHECK_DEVICE() Assert(m_Device != nullptr, "m_Device cannot be null")
#define CHECK_CALL(Experssion) if (!(Experssion)) Assert(false, m_Device->GetLastError());
#define ALLOCATE_ARRAY(Type, Count) ReinterpretCast(Type*, AllocateMemory(&Allocators::RenderingSystemAllocator, Count * sizeof(Type)))
#define ALLOCATE(Type) ALLOCATE_ARRAY(Type, 1)

		DeviceInterfarce::DeviceInterfarce(Type Type) :
			m_Type(Type),
			m_Device(nullptr),
			m_Textures(&Allocators::RenderingSystemAllocator),
			m_Programs(&Allocators::RenderingSystemAllocator),
			m_Windows(&Allocators::RenderingSystemAllocator),
			m_Commands(&Allocators::RenderingSystemAllocator)
		{
		}

		DeviceInterfarce::~DeviceInterfarce(void)
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

		void DeviceInterfarce::Initialize(void)
		{
			InitializeDevice();
		}

		void DeviceInterfarce::SetSampleCount(uint8 Count)
		{
			CHECK_DEVICE();

			m_Device->SetSampleCount(Count);
		}

		void DeviceInterfarce::SetForwardCompatible(bool Value)
		{
			CHECK_DEVICE();

			m_Device->SetForwardCompatible(Value);
		}

		void DeviceInterfarce::SetClearColor(Color Color)
		{
			CHECK_DEVICE();

			m_Device->SetClearColor(Color);
		}

		void DeviceInterfarce::SetClearFlags(IDevice::ClearFlags Flags)
		{
			CHECK_DEVICE();

			m_Device->SetClearFlags(Flags);
		}

		Texture *DeviceInterfarce::CreateTexture2D(const byte * Data, uint32 Width, uint32 Height)
		{
			CHECK_DEVICE();

			Texture::Handle handle;
			CHECK_CALL(m_Device->CreateTexture2D(Data, Width, Height, handle));

			Texture *texture = ALLOCATE(Texture);
			new (texture) Texture(m_Device, handle);

			m_Textures.Add(texture);

			return texture;
		}

		void DeviceInterfarce::DestroyTexture2D(Texture *Texture)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyTexture2D(Texture->GetHandle()));
			Texture->~Texture();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Texture);
		}

		Program *DeviceInterfarce::CreateProgram(const String &Shader)
		{
			static Compiler compiler;

			CHECK_DEVICE();

			String vertProgram;
			String fragProgram;
			compiler.Compile(m_Type, Shader, vertProgram, fragProgram);

			//vertProgram =  "#version 330 core\n"
   // "layout (location = 0) in vec3 aPos;\n"
   // "void main()\n"
   // "{\n"
   // "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			//	"}\0";;
			//fragProgram = "#version 330 core\n"
   // "out vec4 FragColor;\n"
   // "void main()\n"
   // "{\n"
   // "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
   // "}\n\0";

			Program::Handle handle;
			CHECK_CALL(m_Device->CreateProgram(vertProgram.GetValue(), fragProgram.GetValue(), handle));

			Program *program = ALLOCATE(Program);
			new (program) Program(m_Device, handle);

			m_Programs.Add(program);

			return program;
		}

		void DeviceInterfarce::DestroyProgram(Program *Program)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyProgram(Program->GetHandle()));
			Program->~Program();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Program);
		}

		Mesh *DeviceInterfarce::CreateMesh(IDevice::MeshInfo *Info, IDevice::BufferUsages Usage)
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

		void DeviceInterfarce::DestroyMesh(Mesh *Mesh)
		{
			//CHECK_DEVICE();

			//CHECK_CALL(m_Device->DestroyBuffer(Mesh->GetVertices().GetHandle()));
			//CHECK_CALL(m_Device->DestroyBuffer(Mesh->GetUVs().GetHandle()));
			//Mesh->~Mesh();
			//DeallocateMemory(&Allocators::RenderingSystemAllocator, Mesh);
		}

		Window *DeviceInterfarce::CreateWindow(uint16 Width, uint16 Height, cstr Title)
		{
			CHECK_DEVICE();

			Window::Handle handle;
			CHECK_CALL(m_Device->CreateWindow(Width, Height, Title, handle));

			Window *window = ALLOCATE(Window);
			new (window) Window(m_Device, handle);

			m_Windows.Add(window);

			return window;
		}

		void DeviceInterfarce::DestroyWindow(Window *Window)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->DestroyWindow(Window->GetHandle()));
			Window->~Window();
			DeallocateMemory(&Allocators::RenderingSystemAllocator, Window);
		}

		void DeviceInterfarce::DrawMesh(Mesh *Mesh, Program *Program)
		{
			CHECK_DEVICE();

			CHECK_CALL(m_Device->BindProgram(Program->GetHandle()));

			for (uint16 i = 0; i < Mesh->GetSubMeshCount(); ++i)
			{
				Mesh::SubMesh &subMesh = Mesh->GetSubMeshes()[i];

				CHECK_CALL(m_Device->BindBuffer(subMesh.GetBuffer().GetHandle()));

				m_Device->Draw(IDevice::DrawModes::Triangles, subMesh.GetIndexCount());
			}

		}

		void DeviceInterfarce::BeginRender(void)
		{
			CHECK_DEVICE();

			m_Device->Clear();

			for each (auto command in m_Commands)
				command->Execute(m_Device);
		}

		void DeviceInterfarce::EndRender(void)
		{
			CHECK_DEVICE();

			for each (auto window in m_Windows)
				m_Device->SwapBuffers(window->GetHandle());

			m_Device->PollEvents();
		}

		void DeviceInterfarce::InitializeDevice(void)
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
	}
}
