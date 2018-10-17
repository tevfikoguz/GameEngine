
#include <Common\BitwiseUtils.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <Rendering\RenderingManager.h>
#include <Rendering\Color.h>
#include <Rendering\IDevice.h>
#include <ResourceSystem\ResourceManager.h>
#include <Profiler\RealtimeProfiler.h>
#include <Profiler\Profiling.h>


using namespace Engine::Common;
using namespace Engine::MemoryManagement::Allocator;
using namespace Engine::Rendering;
using namespace Engine::ResourceSystem;
using namespace Engine::Profiler;

const int WIDTH = 1024;
const int HEIGHT = 768;
const float ASPECT_RATIO = (float)WIDTH / HEIGHT;

void main()
{
	RealtimeProfiler::Create(RootAllocator::GetInstance());

	RenderingManager *rendering = RenderingManager::Create(RootAllocator::GetInstance());
	ResourceManager *resources = ResourceManager::Create(RootAllocator::GetInstance());

	DeviceInterfarce *device = rendering->CreateDevice(DeviceInterfarce::Type::OpenGL);

	device->Initialize();
	device->SetSampleCount(4);
	device->SetForwardCompatible(true);
	device->SetProfilingEnabled(true);

	Window *window = device->CreateWindow(WIDTH, HEIGHT, "Test Rendering");

	TextureResource tex = resources->Load<Texture>("WOOD.png");
	TextResource text = resources->Load<Text>("data.txt");
	ProgramResource shader = resources->Load<Program>("Shader.shader");



	const float32 vertexBufferData[] = {
		-1, 1, -1,
		1, 1, -1,
		-1, -1, -1
	};

	const float32 uvBufferData[] = {
		0, 0,
		1, 0,
		0, 1
	};

	Mesh *mesh = device->CreateMesh(vertexBufferData, sizeof(vertexBufferData) / sizeof(float32), uvBufferData, sizeof(uvBufferData) / sizeof(float32), IDevice::BufferUsages::StaticDraw);


	device->SetClearColor(Color(255, 0, 0));

	while (!window->ShouldClose())
	{
		//BeginProfilerFrame();

		//ProfileScope("BeginRender");
		device->BeginRender();

		device->DrawMesh(mesh, *shader);

		//ProfileScope("BeginRender");
		device->EndRender();

		//EndProfilerFrame();
	}

	ResourceManager::Destroy();
	RenderingManager::Destroy();
}