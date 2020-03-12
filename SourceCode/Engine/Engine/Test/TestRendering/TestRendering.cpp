
#include <Common\BitwiseUtils.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <Rendering\RenderingManager.h>
#include <Rendering\Color.h>
#include <Rendering\IDevice.h>
#include <ResourceSystem\ResourceManager.h>
#include <Profiler\RealtimeProfiler.h>
#include <Profiler\Profiling.h>
#include <MathContainers\MathContainers.h>
#include <Rendering\Material.h>
#include <Platform\PlatformFile.h>
#include <GameObjectSystem\SceneManager.h>
#include <Utility\HighResolutionTime.h>
#include <Utility\Window.h>
#include <Utility\FileSystem.h>
#include <FreeType\include\ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <iostream>


using namespace Engine::Common;
using namespace Engine::MemoryManagement::Allocator;
using namespace Engine::Rendering;
using namespace Engine::ResourceSystem;
using namespace Engine::Profiler;
using namespace Engine::Containers;
using namespace Engine::MathContainers;
using namespace Engine::Platform;
using namespace Engine::GameObjectSystem;
using namespace Engine::Utility;

const int WIDTH = 1024;
const int HEIGHT = 768;
const float ASPECT_RATIO = (float)WIDTH / HEIGHT;


void main()
{
	FileSystem::SetWorkingPath(L"D:\\Projects\\GameEngineAssets");

	RealtimeProfiler::Create(RootAllocator::GetInstance());
	RenderingManager* rendering = RenderingManager::Create(RootAllocator::GetInstance());
	DeviceInterface* device = rendering->CreateDevice(DeviceInterface::Type::OpenGL);
	SceneManager* sceneMgr = SceneManager::Create(RootAllocator::GetInstance());

	Window window("Test Rendering");
	window.Initialize();
	window.SetIsVisible(true);
	window.SetMaximumSize({ WIDTH, HEIGHT });
	window.SetMinimumSize({ WIDTH, HEIGHT });
	window.SetSize({ WIDTH, HEIGHT });
	window.SetTitle("Test Rendering");
	RenderContext* context = device->CreateContext(&window);
	device->SetContext(context);

	device->Initialize();


	Window window1("Test Rendering 1");
	window1.Initialize();
	window1.SetIsVisible(true);
	window1.SetSize({ WIDTH, HEIGHT });
	window1.SetMaximumSize({ WIDTH, HEIGHT });
	window1.SetMinimumSize({ WIDTH, HEIGHT });
	window1.SetTitle("Test Rendering 1");
	RenderContext* context1 = device->CreateContext(&window1);


	ResourceManager* resources = ResourceManager::Create(RootAllocator::GetInstance());
	ProgramResource shader = resources->Load<Program>("TextShader.shader");

	//Mesh *mesh = device->CreateMesh(&meshInfo, IDevice::BufferUsages::StaticDraw);

	Material characterMat;
	characterMat.SetQueue(RenderQueues::HUD);
	Pass pass(*shader);
	auto st = pass.GetRenderState();
	st.CullMode = IDevice::CullModes::None;
	st.DepthTestFunction = IDevice::TestFunctions::Never;
	pass.SetRenderState(st);
	characterMat.AddPass(pass);

	float32 fps = 0;
	uint32 frameCount = 0;
	uint64 nextCheckTime = HighResolutionTime::GetTime().GetMilliseconds() + 1000;

	Matrix4F projection;
	projection.MakeOrthographicProjectionMatrix(WIDTH, HEIGHT, 0.1, 1000);

	while (!window.ShouldClose())
	{
		PlatformWindow::PollEvents();

		Matrix4F idMat = Matrix4F::Identity;
		idMat.SetScale({ 0.1F, 0.1F, 0.1F });
		idMat.SetPosition({ -300, 0, -100 });

		idMat = projection * idMat;

		device->SetContext(context);

		device->BeginRender();

		//device->DrawMesh(mesh, idMat, &characterMat);

		device->EndRender();


		device->SetContext(context1);

		device->BeginRender();

		//device->DrawMesh(mesh, idMat, &characterMat);

		device->EndRender();

		uint64 time = HighResolutionTime::GetTime().GetMilliseconds();

		++frameCount;

		if (time >= nextCheckTime)
		{
			fps = frameCount;
			frameCount = 0;
			nextCheckTime = time + 1000;



			std::cout << fps << std::endl;
		}
	}
}