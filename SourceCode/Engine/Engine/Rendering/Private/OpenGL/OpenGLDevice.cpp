// Copyright 2016-2017 ?????????????. All Rights Reserved.
#include <Rendering\Private\OpenGL\OpenGLDevice.h>
#include <Debugging\Debug.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <Platform\PlatformMemory.h>

#include <GL\glew.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

namespace Engine
{
	using namespace Platform;
	using namespace MemoryManagement::Allocator;

	namespace Rendering
	{
		namespace Private
		{
			namespace OpenGL
			{
				const uint8 LAST_ERROR_SIZE = 255;

				DynamicSizeAllocator allocator("OpenGL Device System Allocator", RootAllocator::GetInstance(), 1024 * 1024);

				template<typename BaseType>
				BaseType *Allocate(uint32 Count)
				{
					return reinterpret_cast<BaseType*>(AllocateMemory(&allocator, Count * sizeof(BaseType)));
				}

				template<typename BaseType>
				void Deallocate(BaseType *Ptr)
				{
					DeallocateMemory(&allocator, Ptr);
				}

				OpenGLDevice::OpenGLDevice(void)
				{
					m_LastError = Allocate<char8>(LAST_ERROR_SIZE + 1);
				}

				OpenGLDevice::~OpenGLDevice(void)
				{
					Deallocate(m_LastError);
				}

				bool OpenGLDevice::Initialize(void)
				{
					if (glfwInit() == GLFW_FALSE)
					{
						PlatformMemory::Copy("GLFW initialization failed", m_LastError, 26);

						return false;
					}

					glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
					glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);

					return true;
				}

				void OpenGLDevice::SetSampleCount(uint8 Count)
				{
					glfwWindowHint(GLFW_SAMPLES, Count);
				}

				void OpenGLDevice::SetForwardCompatible(bool Value)
				{
					glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (Value ? GL_TRUE : GL_FALSE));
				}

				void OpenGLDevice::SetProfilingEnabled(bool Value)
				{
					glfwWindowHint(GLFW_OPENGL_PROFILE, (Value ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_ANY_PROFILE));
				}

				bool OpenGLDevice::CreateTexture2D(const byte * Data, uint32 Width, uint32 Height, Texture::Handle &Handle)
				{
					glGenTextures(1, &Handle);
					glBindTexture(GL_TEXTURE_2D, Handle);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, Data);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

					return true;
				}

				bool OpenGLDevice::CreateProgram(cstr VertexShader, cstr FragmentShader, Program::Handle &Handle)
				{
					uint32 vertShaderID = glCreateShader(GL_VERTEX_SHADER);
					uint32 fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

					glShaderSource(vertShaderID, 1, &VertexShader, nullptr);
					glCompileShader(vertShaderID);

					glShaderSource(fragShaderID, 1, &FragmentShader, nullptr);
					glCompileShader(fragShaderID);

					int32 result;
					int32 infoLogLength;

					glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
					glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
					if (infoLogLength != 0)
					{
						glGetShaderInfoLog(vertShaderID, LAST_ERROR_SIZE, nullptr, m_LastError);

						return false;
					}

					glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
					glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
					if (infoLogLength != 0)
					{
						str info = Allocate<char8>(infoLogLength + 1);

						glGetShaderInfoLog(fragShaderID, LAST_ERROR_SIZE, nullptr, m_LastError);

						return false;
					}

					Handle = glCreateProgram();
					glAttachShader(Handle, vertShaderID);
					glAttachShader(Handle, fragShaderID);
					glLinkProgram(Handle);

					glGetProgramiv(Handle, GL_LINK_STATUS, &result);
					glGetProgramiv(Handle, GL_INFO_LOG_LENGTH, &infoLogLength);
					if (infoLogLength != 0)
					{
						glGetProgramInfoLog(Handle, LAST_ERROR_SIZE, nullptr, m_LastError);

						return false;
					}

					glDetachShader(Handle, vertShaderID);
					glDetachShader(Handle, fragShaderID);

					glDeleteShader(vertShaderID);
					glDeleteShader(fragShaderID);

					return true;
				}

				bool OpenGLDevice::CreateWindow(uint16 Width, uint16 Height, cstr Title, Window::Handle &Handle)
				{
					GLFWwindow *window = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);

					if (window == nullptr)
					{
						PlatformMemory::Copy("Window creation failed", m_LastError, 22);

						return false;
					}

					glfwMakeContextCurrent(window);
					glewExperimental = true;

					if (glewInit() != GLEW_OK)
					{
						PlatformMemory::Copy("GLEW initialization failed", m_LastError, 26);

						return false;
					}

					glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

					Handle = reinterpret_cast<Window::Handle>(window);

					return true;
				}
			}
		}
	}
}