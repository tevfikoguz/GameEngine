// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Rendering\Private\OpenGL\OpenGLDevice.h>
#include <Debugging\Debug.h>
#include <MemoryManagement\Allocator\RootAllocator.h>
#include <Platform\PlatformMemory.h>
#include <Utility\Window.h>
#include <GL\glew.h>

namespace Engine
{
	using namespace Common;
	using namespace Platform;
	using namespace MemoryManagement::Allocator;

	namespace Rendering
	{
		namespace Private
		{
			namespace OpenGL
			{
				const uint16 LAST_ERROR_SIZE = 512;

				DynamicSizeAllocator allocator("OpenGL Device System Allocator", RootAllocator::GetInstance(), MegaByte);

				template<typename BaseType>
				BaseType* Allocate(uint32 Count)
				{
					return ReinterpretCast(BaseType*, AllocateMemory(&allocator, Count * sizeof(BaseType)));
				}

				template<typename BaseType>
				void Deallocate(BaseType* Ptr)
				{
					DeallocateMemory(&allocator, Ptr);
				}

				uint32 GetClearingFlags(IDevice::ClearFlags Flags)
				{
					uint32 flags = 0;

					if (BitwiseUtils::IsEnabled(Flags, IDevice::ClearFlags::ColorBuffer))
						flags |= GL_COLOR_BUFFER_BIT;

					if (BitwiseUtils::IsEnabled(Flags, IDevice::ClearFlags::DepthBuffer))
						flags |= GL_DEPTH_BUFFER_BIT;

					if (BitwiseUtils::IsEnabled(Flags, IDevice::ClearFlags::AccumulationBuffer))
						flags |= GL_ACCUM_BUFFER_BIT;

					if (BitwiseUtils::IsEnabled(Flags, IDevice::ClearFlags::StencilBuffer))
						flags |= GL_STENCIL_BUFFER_BIT;

					return flags;
				}

				uint32 GetBufferUsageFlags(IDevice::BufferUsages Flag)
				{
					switch (Flag)
					{
					case  IDevice::BufferUsages::StreamDraw:
						return GL_STREAM_DRAW;
					case  IDevice::BufferUsages::StreamRead:
						return GL_STREAM_READ;
					case  IDevice::BufferUsages::StreamCopy:
						return GL_STREAM_COPY;
					case IDevice::BufferUsages::StaticDraw:
						return GL_STATIC_DRAW;
					case IDevice::BufferUsages::StaticRead:
						return GL_STATIC_READ;
					case IDevice::BufferUsages::StaticCopy:
						return GL_STATIC_COPY;
					case IDevice::BufferUsages::DynamicDraw:
						return GL_DYNAMIC_DRAW;
					case IDevice::BufferUsages::DynamicRead:
						return GL_DYNAMIC_READ;
					case IDevice::BufferUsages::DynamicCopy:
						return GL_DYNAMIC_COPY;
					}

					return GL_READ_ONLY;
				}

				uint32 GetPolygonType(Mesh::SubMesh::PolygonTypes PolygonType)
				{
					switch (PolygonType)
					{
					case Mesh::SubMesh::PolygonTypes::Lines:
						return GL_LINES;
					case Mesh::SubMesh::PolygonTypes::LineLoop:
						return GL_LINE_LOOP;
					case Mesh::SubMesh::PolygonTypes::LineStrip:
						return GL_LINE_STRIP;

					case Mesh::SubMesh::PolygonTypes::Triangles:
						return GL_TRIANGLES;
					case Mesh::SubMesh::PolygonTypes::TriangleStrip:
						return GL_TRIANGLE_STRIP;
					case Mesh::SubMesh::PolygonTypes::TriangleFan:
						return GL_TRIANGLE_FAN;

					case Mesh::SubMesh::PolygonTypes::Quads:
						return GL_QUADS;
					case Mesh::SubMesh::PolygonTypes::QuadStrip:
						return GL_QUAD_STRIP;

					case Mesh::SubMesh::PolygonTypes::Polygon:
						return GL_POLYGON;
					}

					return GL_LINES;
				}

				uint32 GetFaceOrdering(IDevice::FaceOrders Order)
				{
					switch (Order)
					{
					case IDevice::FaceOrders::Clockwise:
						return GL_CW;
					case IDevice::FaceOrders::CounterClockwise:
						return GL_CCW;
					}

					return GL_CW;
				}

				uint32 GetCullingMode(IDevice::CullModes Modes)
				{
					if (BitwiseUtils::IsEnabled(Modes, IDevice::CullModes::Front) && BitwiseUtils::IsEnabled(Modes, IDevice::CullModes::Back))
						return GL_FRONT_AND_BACK;

					if (BitwiseUtils::IsEnabled(Modes, IDevice::CullModes::Front))
						return GL_FRONT;

					if (BitwiseUtils::IsEnabled(Modes, IDevice::CullModes::Back))
						return GL_BACK;

					return GL_NONE;
				}

				uint32 GetTestFunction(IDevice::TestFunctions Function)
				{
					switch (Function)
					{
					case IDevice::TestFunctions::Never:
						return GL_NEVER;
					case IDevice::TestFunctions::Less:
						return GL_LESS;
					case IDevice::TestFunctions::LessEqual:
						return GL_LEQUAL;
					case IDevice::TestFunctions::Equal:
						return GL_EQUAL;
					case IDevice::TestFunctions::NotEqual:
						return GL_NOTEQUAL;
					case IDevice::TestFunctions::GreaterEqual:
						return GL_GEQUAL;
					case IDevice::TestFunctions::Greater:
						return GL_GREATER;
					case IDevice::TestFunctions::Always:
						return GL_ALWAYS;
					}

					return GL_NEVER;
				}

				uint32 GetStencilingOperation(IDevice::StencilOperations Operation)
				{
					switch (Operation)
					{
					case IDevice::StencilOperations::Keep:
						return GL_KEEP;
					case IDevice::StencilOperations::Zero:
						return GL_ZERO;
					case IDevice::StencilOperations::Replace:
						return GL_REPLACE;
					case IDevice::StencilOperations::Increament:
						return GL_INCR;
					case IDevice::StencilOperations::IncreamentWrap:
						return GL_INCR_WRAP;
					case IDevice::StencilOperations::Decreament:
						return GL_DECR;
					case IDevice::StencilOperations::DecreamentWrap:
						return GL_DECR_WRAP;
					case IDevice::StencilOperations::Invert:
						return GL_INVERT;
					}

					return GL_KEEP;
				}

				uint32 GetBlendingEquation(IDevice::BlendEquations Equation)
				{
					switch (Equation)
					{
					case IDevice::BlendEquations::Add:
						return GL_FUNC_ADD;
					case IDevice::BlendEquations::Subtract:
						return GL_FUNC_SUBTRACT;
					case IDevice::BlendEquations::ReverseSubtract:
						return GL_FUNC_REVERSE_SUBTRACT;
					case IDevice::BlendEquations::Min:
						return GL_MIN;
					case IDevice::BlendEquations::Max:
						return GL_MAX;
					}

					return GL_FUNC_ADD;
				}

				uint32 GetBlendingFunction(IDevice::BlendFunctions Function)
				{
					switch (Function)
					{
					case IDevice::BlendFunctions::Zero:
						return GL_ZERO;
					case IDevice::BlendFunctions::One:
						return GL_ONE;
					case IDevice::BlendFunctions::SourceColor:
						return GL_SRC_COLOR;
					case IDevice::BlendFunctions::OneMinusSourceColor:
						return GL_ONE_MINUS_SRC_COLOR;
					case IDevice::BlendFunctions::DestinationColor:
						return GL_DST_COLOR;
					case IDevice::BlendFunctions::OneMinusDestinationColor:
						return GL_ONE_MINUS_DST_COLOR;
					case IDevice::BlendFunctions::SourceAlpha:
						return GL_SRC_ALPHA;
					case IDevice::BlendFunctions::OneMinusSourceAlpha:
						return GL_ONE_MINUS_SRC_ALPHA;
					case IDevice::BlendFunctions::DestinationAlpha:
						return GL_DST_ALPHA;
					case IDevice::BlendFunctions::OneMinusDestinationAlpha:
						return GL_ONE_MINUS_DST_ALPHA;
					case IDevice::BlendFunctions::ConstantColor:
						return GL_CONSTANT_COLOR;
					case IDevice::BlendFunctions::OneMinusConstantColor:
						return GL_ONE_MINUS_CONSTANT_COLOR;
					case IDevice::BlendFunctions::ConstantAlpha:
						return GL_CONSTANT_ALPHA;
					case IDevice::BlendFunctions::OneMinusConstantAlpha:
						return GL_ONE_MINUS_CONSTANT_ALPHA;
					}

					return GL_ZERO;
				}

				uint32 GetTextureInternalFormat(Texture::Formats Format)
				{
					switch (Format)
					{
					case Texture::Formats::R8:
						return GL_R8;
					case Texture::Formats::R16:
						return GL_R16;
					case Texture::Formats::R32:
						return GL_R32UI;
					case Texture::Formats::R16F:
						return GL_R16F;
					case Texture::Formats::R32F:
						return GL_R32F;
					case Texture::Formats::RG8:
						return GL_RG8;
					case Texture::Formats::RG16:
						return GL_RG16;
					case Texture::Formats::RG32:
						return GL_RG32UI;
					case Texture::Formats::RG16F:
						return GL_RG16F;
					case Texture::Formats::RG32F:
						return GL_RG32F;
					case Texture::Formats::RGB8:
						return GL_RGB8;
					case Texture::Formats::RGB16:
						return GL_RGB16UI;
					case Texture::Formats::RGB32:
						return GL_RGB32UI;
					case Texture::Formats::RGB16F:
						return GL_RGB16F;
					case Texture::Formats::RGB32F:
						return GL_RGB32F;
					case Texture::Formats::RGBA8:
						return GL_RGBA8;
					case Texture::Formats::RGBA16:
						return GL_RGBA16;
					case Texture::Formats::RGBA32:
						return GL_RGBA32UI;
					case Texture::Formats::RGBA16F:
						return GL_RGBA16F;
					case Texture::Formats::RGBA32F:
						return GL_RGBA32F;
					case Texture::Formats::Depth16:
						return GL_DEPTH_COMPONENT16;
					case Texture::Formats::Depth24:
						return GL_DEPTH_COMPONENT24;
					case Texture::Formats::Depth32:
						return GL_DEPTH_COMPONENT32;
					case Texture::Formats::Depth32F:
						return GL_DEPTH_COMPONENT32F;
					case Texture::Formats::Stencil24F:
						return GL_DEPTH24_STENCIL8;
					case Texture::Formats::Stencil32F:
						return GL_DEPTH32F_STENCIL8;
					}

					return GL_RGBA8;
				}

				uint32 GetTextureFormat(Texture::Formats Format)
				{
					switch (Format)
					{
					case Texture::Formats::R8:
						return GL_RED;
					case Texture::Formats::R16:
						return GL_RED;
					case Texture::Formats::R32:
						return GL_RED;
					case Texture::Formats::R16F:
						return GL_RED;
					case Texture::Formats::R32F:
						return GL_RED;
					case Texture::Formats::RG8:
						return GL_RG;
					case Texture::Formats::RG16:
						return GL_RG;
					case Texture::Formats::RG32:
						return GL_RG;
					case Texture::Formats::RG16F:
						return GL_RG;
					case Texture::Formats::RG32F:
						return GL_RG;
					case Texture::Formats::RGB8:
						return GL_RGB;
					case Texture::Formats::RGB16:
						return GL_RGB;
					case Texture::Formats::RGB32:
						return GL_RGB;
					case Texture::Formats::RGB16F:
						return GL_RGB;
					case Texture::Formats::RGB32F:
						return GL_RGB;
					case Texture::Formats::RGBA8:
						return GL_RGBA;
					case Texture::Formats::RGBA16:
						return GL_RGBA;
					case Texture::Formats::RGBA32:
						return GL_RGBA;
					case Texture::Formats::RGBA16F:
						return GL_RGBA;
					case Texture::Formats::RGBA32F:
						return GL_RGBA;
					case Texture::Formats::Depth16:
						return GL_DEPTH_COMPONENT;
					case Texture::Formats::Depth24:
						return GL_DEPTH_COMPONENT;
					case Texture::Formats::Depth32:
						return GL_DEPTH_COMPONENT;
					case Texture::Formats::Depth32F:
						return GL_DEPTH_COMPONENT;
					case Texture::Formats::Stencil24F:
						return GL_DEPTH_STENCIL;
					case Texture::Formats::Stencil32F:
						return GL_DEPTH_STENCIL;
					}

					return GL_RGBA;
				}

				uint32 GetTexturePixelType(Texture::Formats Format)
				{
					switch (Format)
					{
					case Texture::Formats::R8:
						return GL_UNSIGNED_BYTE;
					case Texture::Formats::R16:
						return GL_UNSIGNED_SHORT;
					case Texture::Formats::R32:
						return GL_UNSIGNED_INT;
					case Texture::Formats::R16F:
						return GL_HALF_FLOAT;
					case Texture::Formats::R32F:
						return GL_FLOAT;
					case Texture::Formats::RG8:
						return GL_UNSIGNED_BYTE;
					case Texture::Formats::RG16:
						return GL_UNSIGNED_SHORT;
					case Texture::Formats::RG32:
						return GL_UNSIGNED_INT;
					case Texture::Formats::RG16F:
						return GL_HALF_FLOAT;
					case Texture::Formats::RG32F:
						return GL_FLOAT;
					case Texture::Formats::RGB8:
						return GL_UNSIGNED_BYTE;
					case Texture::Formats::RGB16:
						return GL_UNSIGNED_INT;
					case Texture::Formats::RGB32:
						return GL_UNSIGNED_INT;
					case Texture::Formats::RGB16F:
						return GL_HALF_FLOAT;
					case Texture::Formats::RGB32F:
						return GL_FLOAT;
					case Texture::Formats::RGBA8:
						return GL_UNSIGNED_BYTE;
					case Texture::Formats::RGBA16:
						return GL_UNSIGNED_SHORT;
					case Texture::Formats::RGBA32:
						return GL_UNSIGNED_INT;
					case Texture::Formats::RGBA16F:
						return GL_HALF_FLOAT;
					case Texture::Formats::RGBA32F:
						return GL_FLOAT;
					case Texture::Formats::Depth16:
						return GL_UNSIGNED_SHORT;
					case Texture::Formats::Depth24:
						return GL_UNSIGNED_INT;
					case Texture::Formats::Depth32:
						return GL_UNSIGNED_INT;
					case Texture::Formats::Depth32F:
						return GL_FLOAT;
					case Texture::Formats::Stencil24F:
						return GL_UNSIGNED_INT_24_8;
					case Texture::Formats::Stencil32F:
						return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
					}

					return GL_UNSIGNED_BYTE;
				}

				uint32 GetWrapMode(Texture::WrapModes Mode)
				{
					switch (Mode)
					{
					case Texture::WrapModes::Clamp:
						return GL_CLAMP;
					case Texture::WrapModes::Repeat:
						return GL_REPEAT;
					case Texture::WrapModes::ClampToEdge:
						return GL_CLAMP_TO_EDGE;
					case Texture::WrapModes::MirroredRepeat:
						return GL_MIRRORED_REPEAT;
					}

					return GL_CLAMP;
				}

				uint32 GetMinifyFilter(Texture::MinifyFilters Filter)
				{
					switch (Filter)
					{
					case Texture::MinifyFilters::Nearest:
						return GL_NEAREST;
					case Texture::MinifyFilters::Linear:
						return GL_LINEAR;
					case Texture::MinifyFilters::NearestMipMapNearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case Texture::MinifyFilters::LinearMipMapNearest:
						return GL_LINEAR_MIPMAP_NEAREST;
					case Texture::MinifyFilters::NearestMipMapLinear:
						return GL_NEAREST_MIPMAP_LINEAR;
					case Texture::MinifyFilters::LinearMipMapLinear:
						return GL_LINEAR_MIPMAP_LINEAR;
					}

					return GL_NEAREST;
				}

				uint32 GetMagnifyFilter(Texture::MagnfyFilters Filter)
				{
					switch (Filter)
					{
					case Texture::MagnfyFilters::Nearest:
						return GL_NEAREST;
					case Texture::MagnfyFilters::Linear:
						return GL_LINEAR;
					}

					return GL_NEAREST;
				}

				uint32 GetPolygonRenderMode(IDevice::PolygonModes Mode)
				{
					switch (Mode)
					{
					case IDevice::PolygonModes::Point:
						return GL_POINT;
					case IDevice::PolygonModes::Line:
						return GL_LINE;
					case IDevice::PolygonModes::Fill:
						return GL_FILL;
					}

					return GL_FILL;
				}

				uint32 GetAttachmentPoint(RenderTarget::AttachmentPoints Point)
				{
					switch (Point)
					{
					case RenderTarget::AttachmentPoints::Depth: return GL_DEPTH_ATTACHMENT;
					case RenderTarget::AttachmentPoints::Stencil: return GL_STENCIL_ATTACHMENT;
					case RenderTarget::AttachmentPoints::Color0: return GL_COLOR_ATTACHMENT0;
					case RenderTarget::AttachmentPoints::Color1: return GL_COLOR_ATTACHMENT1;
					case RenderTarget::AttachmentPoints::Color2: return GL_COLOR_ATTACHMENT2;
					case RenderTarget::AttachmentPoints::Color3: return GL_COLOR_ATTACHMENT3;
					case RenderTarget::AttachmentPoints::Color4: return GL_COLOR_ATTACHMENT4;
					case RenderTarget::AttachmentPoints::Color5: return GL_COLOR_ATTACHMENT5;
					case RenderTarget::AttachmentPoints::Color6: return GL_COLOR_ATTACHMENT6;
					case RenderTarget::AttachmentPoints::Color7: return GL_COLOR_ATTACHMENT7;
					case RenderTarget::AttachmentPoints::Color8: return GL_COLOR_ATTACHMENT8;
					case RenderTarget::AttachmentPoints::Color9: return GL_COLOR_ATTACHMENT9;
					case RenderTarget::AttachmentPoints::Color10: return GL_COLOR_ATTACHMENT10;
					case RenderTarget::AttachmentPoints::Color11: return GL_COLOR_ATTACHMENT11;
					case RenderTarget::AttachmentPoints::Color12: return GL_COLOR_ATTACHMENT12;
					case RenderTarget::AttachmentPoints::Color13: return GL_COLOR_ATTACHMENT13;
					case RenderTarget::AttachmentPoints::Color14: return GL_COLOR_ATTACHMENT14;
					case RenderTarget::AttachmentPoints::Color15: return GL_COLOR_ATTACHMENT15;
					}

					return GL_COLOR_ATTACHMENT0;
				}

				void DebugOutputProcedure(GLenum Source, GLenum Type, GLuint ID, GLenum Severity, GLsizei Length, const GLchar* Message, const void* Param)
				{
					if (ID == 131169 || ID == 131185 || ID == 131218 || ID == 131204)
						return;

					OpenGLDevice* device = ConstCast(OpenGLDevice*, ReinterpretCast(const OpenGLDevice*, Param));
					IDevice::DebugProcedureType procedure = device->GetDebugCallback();

					if (procedure == nullptr)
						return;

					cstr sourceStr = "";
					switch (Source)
					{
					case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
					case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
					case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
					case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
					}

					cstr typeStr = "";
					switch (Type)
					{
					case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behaviour"; break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behaviour"; break;
					case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
					case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
					case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
					case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
					case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
					case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
					}

					IDevice::Severities severityType;
					switch (Severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         severityType = IDevice::Severities::High; break;
					case GL_DEBUG_SEVERITY_MEDIUM:       severityType = IDevice::Severities::Medium; break;
					case GL_DEBUG_SEVERITY_LOW:          severityType = IDevice::Severities::Low; break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: severityType = IDevice::Severities::Notification; break;
					}

					procedure(ID, sourceStr, Message, typeStr, severityType);
				}

				OpenGLDevice::OpenGLDevice(void) :
					m_WindowHandle(0),
					m_WindowContextHandle(0),
					m_WGLHandle(0),
					m_LastProgram(0),
					m_LastMeshBuffer(0),
					m_LastMeshNumber(0),
					m_LastFrameBuffer(0),
					m_LastActiveTextureUnitIndex(0)
				{
				}

				OpenGLDevice::~OpenGLDevice(void)
				{
				}

				bool OpenGLDevice::Initialize(void)
				{
					Assert(m_WindowHandle != 0, "Window is null");

					if (!RefreshWGLContext())
						return false;

#ifdef DEBUG_MODE
					glEnable(GL_DEBUG_OUTPUT);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback(DebugOutputProcedure, this);
					glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

					m_State.DepthTestFunction = TestFunctions::Never;
					m_State.SetStencilTestFunction(TestFunctions::Never, 0, 0);
					State state;
					SetState(state);

					return true;
				}

				cstr OpenGLDevice::GetVersion(void)
				{
					return ReinterpretCast(cstr, glGetString(GL_VERSION));
				}

				cstr OpenGLDevice::GetVendorName(void)
				{
					return ReinterpretCast(cstr, glGetString(GL_VENDOR));
				}

				cstr OpenGLDevice::GetRendererName(void)
				{
					return ReinterpretCast(cstr, glGetString(GL_RENDERER));
				}

				cstr OpenGLDevice::GetShadingLanguageVersion(void)
				{
					return ReinterpretCast(cstr, glGetString(GL_SHADING_LANGUAGE_VERSION));
				}

				bool OpenGLDevice::SetWindow(PlatformWindow::WindowHandle Handle)
				{
					Assert(m_WindowHandle == 0, "Changing window doesn't supported");

					m_WindowHandle = Handle;

					return true;
				}

				void OpenGLDevice::ResizeViewport(const Vector2I& Size)
				{
					glViewport(0, 0, Size.X, Size.Y);
				}

				void OpenGLDevice::SetClearColor(Color Color)
				{
					if (m_ClearColor == Color)
						return;

					m_ClearColor = Color;

					glClearColor(m_ClearColor.GetFloat32R(), m_ClearColor.GetFloat32G(), m_ClearColor.GetFloat32B(), m_ClearColor.GetFloat32A());
				}

				void OpenGLDevice::SetFaceOrder(FaceOrders Order)
				{
					if (m_State.FaceOrder == Order)
						return;

					m_State.FaceOrder = Order;

					glFrontFace(GetFaceOrdering(m_State.FaceOrder));
				}

				void OpenGLDevice::SetCullMode(CullModes Mode)
				{
					if (m_State.CullMode == Mode)
						return;

					m_State.CullMode = Mode;

					if (m_State.CullMode == CullModes::None)
						glDisable(GL_CULL_FACE);
					else
					{
						glEnable(GL_CULL_FACE);
						glCullFace(GetCullingMode(m_State.CullMode));
					}
				}

				void OpenGLDevice::SetDepthTestFunction(TestFunctions Function)
				{
					if (m_State.DepthTestFunction == Function)
						return;

					m_State.DepthTestFunction = Function;

					if (m_State.DepthTestFunction == TestFunctions::Never)
						glDisable(GL_DEPTH_TEST);
					else
					{
						glEnable(GL_DEPTH_TEST);
						glDepthFunc(GetTestFunction(m_State.DepthTestFunction));
					}
				}

				void OpenGLDevice::SetStencilTestFunction(CullModes CullMode, TestFunctions Function, int32 Reference, uint32 Mask)
				{
					State::FaceState& state = m_State.GetFaceState(CullMode);

					if (state.StencilTestFunction == Function && state.StencilTestFunctionReference == Reference && state.StencilTestFunctionMask == Mask)
						return;

					state.StencilTestFunction = Function;
					state.StencilTestFunctionReference = Reference;
					state.StencilTestFunctionMask = Mask;

					if (state.StencilTestFunction == TestFunctions::Never)
						glDisable(GL_STENCIL_TEST);
					else
					{
						glEnable(GL_STENCIL_TEST);
						glStencilFuncSeparate(GetCullingMode(CullMode), GetTestFunction(state.StencilTestFunction), state.StencilTestFunctionReference, state.StencilTestFunctionMask);
					}
				}

				void OpenGLDevice::SetStencilMask(CullModes CullMode, uint32 Mask)
				{
					State::FaceState& state = m_State.GetFaceState(CullMode);

					if (state.StencilMask == Mask)
						return;

					state.StencilMask = Mask;

					glStencilMaskSeparate(GetCullingMode(CullMode), state.StencilMask);
				}

				void OpenGLDevice::SetStencilOperation(CullModes CullMode, StencilOperations StencilFailed, StencilOperations DepthFailed, StencilOperations DepthPassed)
				{
					State::FaceState& state = m_State.GetFaceState(CullMode);

					if (state.StencilOperationStencilFailed == StencilFailed && state.StencilOperationDepthFailed == DepthFailed && state.StencilOperationDepthPassed == DepthPassed)
						return;

					state.StencilOperationStencilFailed = StencilFailed;
					state.StencilOperationDepthFailed = DepthFailed;
					state.StencilOperationDepthPassed = DepthPassed;

					glStencilOpSeparate(GetCullingMode(CullMode), GetStencilingOperation(state.StencilOperationStencilFailed), GetStencilingOperation(state.StencilOperationDepthFailed), GetStencilingOperation(state.StencilOperationDepthPassed));
				}

				void OpenGLDevice::SetBlendEquation(BlendEquations Equation)
				{
					if (m_State.BlendEquation == Equation)
						return;

					m_State.BlendEquation = Equation;

					glBlendEquation(GetBlendingEquation(m_State.BlendEquation));
				}

				void OpenGLDevice::SetBlendFunction(BlendFunctions SourceFactor, BlendFunctions DestinationFactor)
				{
					if (m_State.BlendFunctionSourceFactor == SourceFactor && m_State.BlendFunctionDestinationFactor == DestinationFactor)
						return;

					m_State.BlendFunctionSourceFactor = SourceFactor;
					m_State.BlendFunctionDestinationFactor = DestinationFactor;

					if (m_State.BlendFunctionSourceFactor == BlendFunctions::One && m_State.BlendFunctionDestinationFactor == BlendFunctions::Zero)
						glDisable(GL_BLEND);
					else
					{
						glEnable(GL_BLEND);
						glBlendFunc(GetBlendingFunction(m_State.BlendFunctionSourceFactor), GetBlendingFunction(m_State.BlendFunctionDestinationFactor));
					}
				}

				void OpenGLDevice::SetPolygonMode(CullModes CullMode, PolygonModes PolygonMode)
				{
					State::FaceState& state = m_State.GetFaceState(CullMode);

					if (state.PolygonMode == PolygonMode)
						return;

					state.PolygonMode = PolygonMode;

					glPolygonMode(GetCullingMode(CullMode), GetPolygonRenderMode(state.PolygonMode));
				}

				bool OpenGLDevice::CreateProgram(cstr VertexShader, cstr FragmentShader, Program::Handle& Handle)
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
					if (result == GL_FALSE)
						return false;

					glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
					if (result == GL_FALSE)
						return false;

					Handle = glCreateProgram();
					glAttachShader(Handle, vertShaderID);
					glAttachShader(Handle, fragShaderID);
					glLinkProgram(Handle);

					glGetProgramiv(Handle, GL_LINK_STATUS, &result);
					if (result == GL_FALSE)
						return false;

					glDetachShader(Handle, vertShaderID);
					glDetachShader(Handle, fragShaderID);

					glDeleteShader(vertShaderID);
					glDeleteShader(fragShaderID);

					return true;
				}

				bool OpenGLDevice::DestroyProgram(Program::Handle Handle)
				{
					glDeleteProgram(Handle);

					return true;
				}

				bool OpenGLDevice::BindProgram(Program::Handle Handle)
				{
					m_LastActiveTextureUnitIndex = 0;

					if (m_LastProgram == Handle)
						return true;

					m_LastProgram = Handle;

					glUseProgram(m_LastProgram);

					return true;
				}

				bool OpenGLDevice::QueryProgramActiveConstants(Program::Handle Handle, Program::ConstantDataList& Constants)
				{
					int32 count = 0;

					glGetProgramiv(Handle, GL_ACTIVE_UNIFORMS, &count);

					if (count == 0)
						return false;

					const uint8 bufferSize = 32;
					char8 name[bufferSize];
					int32 nameLength;
					int32 constantSize;
					uint32 type;

					for (int8 i = 0; i < count; i++)
					{
						glGetActiveUniform(Handle, (GLuint)i, bufferSize, &nameLength, &constantSize, &type, name);

						name[nameLength] = CharacterUtility::Character<char8, '\0'>::Value;

						Program::ConstantHandle handle;
						GetProgramConstantHandle(Handle, name, handle);

						DataType::Types dataType = DataType::Types::Unknown;
						AnyDataType value;

						switch (type)
						{
						case GL_FLOAT:
						{
							dataType = DataType::Types::Float;
							value = 0.0F;
						}
						break;

						case GL_FLOAT_VEC2:
						{
							dataType = DataType::Types::Float2;
							value = Vector2F();
						}
						break;

						case GL_FLOAT_VEC3:
						{
							dataType = DataType::Types::Float3;
							value = Vector3F();
						}
						break;

						case GL_FLOAT_VEC4:
						{
							dataType = DataType::Types::Float4;
							value = Vector4F();
						}
						break;

						case GL_FLOAT_MAT4:
						{
							dataType = DataType::Types::Matrix4;

							Matrix4F mat;
							mat.MakeIdentity();
							value = mat;
						}
						break;

						case GL_SAMPLER_2D:
						{
							dataType = DataType::Types::Texture2D;
							value = nullptr;
						}
						break;
						}

						Program::ConstantData data;
						data.Handle = handle;
						data.Name = name;
						data.Type = dataType;;
						data.Value = value;

						Constants.Add(data);
					}

					return true;
				}

				bool OpenGLDevice::GetProgramConstantHandle(Program::Handle Handle, const String& Name, Program::ConstantHandle& ConstantHandle)
				{
					ConstantHandle = glGetUniformLocation(Handle, Name.GetValue());
					return true;
				}

				bool OpenGLDevice::SetProgramFloat32(Program::ConstantHandle Handle, float32 Value)
				{
					glUniform1f(Handle, Value);

					return true;
				}

				bool OpenGLDevice::SetProgramVector2(Program::ConstantHandle Handle, const Vector2F& Value)
				{
					glUniform2f(Handle, Value.X, Value.Y);

					return true;
				}

				bool OpenGLDevice::SetProgramVector3(Program::ConstantHandle Handle, const Vector3F& Value)
				{
					glUniform3f(Handle, Value.X, Value.Y, Value.Z);

					return true;
				}

				bool OpenGLDevice::SetProgramVector4(Program::ConstantHandle Handle, const Vector4F& Value)
				{
					glUniform4f(Handle, Value.X, Value.Y, Value.Z, Value.W);

					return true;
				}

				bool OpenGLDevice::SetProgramMatrix4(Program::ConstantHandle Handle, const Matrix4F& Value)
				{
					glUniformMatrix4fv(Handle, 1, false, Value.GetValue());

					return true;
				}

				bool OpenGLDevice::SetProgramTexture(Program::ConstantHandle Handle, Texture::Handle Value)
				{
					glActiveTexture(GL_TEXTURE0 + m_LastActiveTextureUnitIndex);

					BindTexture2D(Value);

					glUniform1i(Handle, m_LastActiveTextureUnitIndex);

					++m_LastActiveTextureUnitIndex;

					return true;
				}

				bool OpenGLDevice::CreateTexture2D(const byte* Data, uint32 Width, uint32 Height, Texture::Formats Format, Texture::Handle& Handle)
				{
					glGenTextures(1, &Handle);

					BindTexture2D(Handle);

					glTexImage2D(GL_TEXTURE_2D, 0, GetTextureInternalFormat(Format), Width, Height, 0, GetTextureFormat(Format), GetTexturePixelType(Format), Data);

					GenerateMipMap(Handle);

					return true;
				}

				bool OpenGLDevice::DestroyTexture(Texture::Handle Handle)
				{
					glDeleteTextures(1, &Handle);

					return true;
				}

				bool OpenGLDevice::BindTexture2D(Program::Handle Handle)
				{
					glBindTexture(GL_TEXTURE_2D, Handle);

					return true;
				}

				bool OpenGLDevice::SetTexture2DVerticalWrapping(Texture::Handle Handle, Texture::WrapModes Mode)
				{
					BindTexture2D(Handle);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(Mode));

					return true;
				}

				bool OpenGLDevice::SetTexture2DHorizontalWrapping(Texture::Handle Handle, Texture::WrapModes Mode)
				{
					BindTexture2D(Handle);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(Mode));

					return true;
				}

				bool OpenGLDevice::SetTexture2DMinifyFilter(Texture::Handle Handle, Texture::MinifyFilters Filter)
				{
					BindTexture2D(Handle);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMinifyFilter(Filter));

					return true;
				}

				bool OpenGLDevice::SetTexture2DMagnifyFilter(Texture::Handle Handle, Texture::MagnfyFilters Filter)
				{
					BindTexture2D(Handle);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetMagnifyFilter(Filter));

					return true;
				}

				bool OpenGLDevice::GenerateMipMap(Texture::Handle Handle)
				{
					BindTexture2D(Handle);

					glGenerateMipmap(GL_TEXTURE_2D);

					return true;
				}

				bool OpenGLDevice::CreateRenderTarget(const RenderTargetInfo* Info, RenderTarget::Handle& Handle, TextureList& Textures)
				{
					glGenFramebuffers(1, &Handle);

					glBindFramebuffer(GL_FRAMEBUFFER, Handle);

					m_RenderTargets[Handle] = {};

					auto& texturesList = m_RenderTargets[Handle];

					static uint32 drawBuffers[((int8)RenderTarget::AttachmentPoints::Color15 - (int8)RenderTarget::AttachmentPoints::Color0) + 1];

					int drawBufferIndex = 0;

					for each (const auto & textureInfo in Info->Textures)
					{
						Texture::Handle texHandle;
						CreateTexture2D(nullptr, textureInfo.Width, textureInfo.Height, textureInfo.Format, texHandle);

						uint32 point = GetAttachmentPoint(textureInfo.Point);

						glFramebufferTexture2D(GL_FRAMEBUFFER, point, GL_TEXTURE_2D, texHandle, 0);

						texturesList.Texture.Add(texHandle);

						if (textureInfo.Point >= RenderTarget::AttachmentPoints::Color0)
							drawBuffers[drawBufferIndex++] = point;
					}

					Textures.AddRange(texturesList.Texture);

					glDrawBuffers(drawBufferIndex, drawBuffers);

					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					return true;
				}

				bool OpenGLDevice::DestroyRenderTarget(RenderTarget::Handle Handle)
				{
					if (!m_RenderTargets.Contains(Handle))
						return false;

					auto& info = m_RenderTargets[Handle];

					for each (auto handle in info.Texture)
						DestroyTexture(handle);

					glDeleteFramebuffers(1, &Handle);

					m_RenderTargets.Remove(Handle);

					return true;
				}

				bool OpenGLDevice::BindRenderTarget(RenderTarget::Handle Handle)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, Handle);

					return true;
				}

				bool OpenGLDevice::CreateMesh(const SubMeshInfo* Info, BufferUsages Usage, GPUBuffer::Handle& Handle)
				{
					if (Info->Vertices.GetSize() == 0)
						return false;

					uint32 vertexSize = sizeof(Vertex);

					uint32 vao;
					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);

					uint32 vbo;
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferData(GL_ARRAY_BUFFER, Info->Vertices.GetSize() * vertexSize, Info->Vertices.GetData(), GetBufferUsageFlags(Usage));

					uint32 ebo = 0;
					if (Info->Indices.GetSize() != 0)
					{
						glGenBuffers(1, &ebo);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, Info->Indices.GetSize() * sizeof(float), Info->Indices.GetData(), GetBufferUsageFlags(Usage));
					}

					if (BitwiseUtils::IsEnabled(Info->Layout, Mesh::SubMesh::VertexLayouts::Position))
					{
						uint16 index = SubMeshInfo::GetLayoutIndex(Mesh::SubMesh::VertexLayouts::Position);

						glVertexAttribPointer(index, 3, GL_FLOAT, false, vertexSize, (void*)OffsetOf(&Vertex::Position));
						glEnableVertexAttribArray(index++);
					}
					if (BitwiseUtils::IsEnabled(Info->Layout, Mesh::SubMesh::VertexLayouts::Normal))
					{
						uint16 index = SubMeshInfo::GetLayoutIndex(Mesh::SubMesh::VertexLayouts::Normal);

						glVertexAttribPointer(index, 3, GL_FLOAT, false, vertexSize, (void*)OffsetOf(&Vertex::Normal));
						glEnableVertexAttribArray(index++);
					}
					if (BitwiseUtils::IsEnabled(Info->Layout, Mesh::SubMesh::VertexLayouts::UV))
					{
						uint16 index = SubMeshInfo::GetLayoutIndex(Mesh::SubMesh::VertexLayouts::UV);

						glVertexAttribPointer(index, 2, GL_FLOAT, false, vertexSize, (void*)OffsetOf(&Vertex::UV));
						glEnableVertexAttribArray(index);
					}

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

					m_LastMeshBuffer = 0;

					Handle = ++m_LastMeshNumber;
					m_MeshBuffers[Handle] = { vao, vbo, ebo };

					return true;
				}

				bool OpenGLDevice::DestroyMesh(GPUBuffer::Handle Handle)
				{
					if (!m_MeshBuffers.Contains(Handle))
						return false;

					auto& info = m_MeshBuffers[Handle];

					glDeleteBuffers(1, &info.VertexArrayObject);

					m_MeshBuffers.Remove(Handle);

					return true;
				}

				bool OpenGLDevice::BindMesh(GPUBuffer::Handle Handle)
				{
					if (m_LastMeshBuffer == Handle)
						return true;
					m_LastMeshBuffer = Handle;

					if (!m_MeshBuffers.Contains(m_LastMeshBuffer))
						return false;

					auto& info = m_MeshBuffers[m_LastMeshBuffer];

					glBindVertexArray(info.VertexArrayObject);

					return true;
				}

				void OpenGLDevice::Clear(ClearFlags Flags)
				{
					glClear(GetClearingFlags(Flags));
				}

				void OpenGLDevice::DrawIndexed(Mesh::SubMesh::PolygonTypes PolygonType, uint32 IndexCount)
				{
					glDrawElements(GetPolygonType(PolygonType), IndexCount, GL_UNSIGNED_INT, 0);
				}

				void OpenGLDevice::DrawArray(Mesh::SubMesh::PolygonTypes PolygonType, uint32 VertexCount)
				{
					glDrawArrays(GetPolygonType(PolygonType), 0, VertexCount);
				}

				void OpenGLDevice::SwapBuffers(void)
				{
					if (m_WindowHandle != 0)
						PlatformWindow::SwapBuffers(m_WindowContextHandle);
				}

				bool OpenGLDevice::RefreshWGLContext(void)
				{
					if (m_WindowContextHandle != 0)
						PlatformWindow::DestroyWGLContext(m_WindowContextHandle);

					m_WindowContextHandle = PlatformWindow::GetDeviceContext(m_WindowHandle);

					PlatformWindow::PixelFormatInfo pixelFormat =
					{
						PlatformWindow::PixelFormats::DrawToWindow | PlatformWindow::PixelFormats::SupportOpenGL | PlatformWindow::PixelFormats::DoubleBuffer,
						PlatformWindow::PixelTypes::RGBA,
						32,
						24,
						8,
						PlatformWindow::LayerTypes::MainPlane
					};

					int32 pixelFormatIndex = PlatformWindow::ChoosePixelFormat(m_WindowContextHandle, &pixelFormat);
					PlatformWindow::SetPixelFormat(m_WindowContextHandle, pixelFormatIndex, &pixelFormat);

					m_WGLHandle = PlatformWindow::CreateWGLContext(m_WindowContextHandle);
					PlatformWindow::MakeWGLCurrent(m_WindowContextHandle, m_WGLHandle);

					glewExperimental = true;
					if (glewInit() != GLEW_OK)
						return false;

					PlatformWindow::WGLContextHandle arbwgl = PlatformWindow::CreateWGLARBContext(m_WindowContextHandle, m_WGLHandle,
#ifdef DEBUG_MODE
						true
#else
						false
#endif

					);

					if (arbwgl != 0)
						m_WGLHandle = arbwgl;

					return true;
				}
			}
		}
	}
}
