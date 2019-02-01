// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#ifndef OPEN_GL_DEVICE_H
#define OPEN_GL_DEVICE_H

#include <Containers\Map.h>
#include <Rendering\IDevice.h>

namespace Engine
{
	namespace Rendering
	{
		namespace Private
		{
			namespace OpenGL
			{
				class OpenGLDevice : public IDevice
				{
				private:
					struct FrameBufferHandles
					{
					public:
						RenderTarget::Handle RenderTarget;
						GPUBuffer::Handle RenderBuffer;
					};

					struct MeshBufferHandles
					{
					public:
						GPUBuffer::Handle VertexArrayObject;
						GPUBuffer::Handle VertexBufferObject;
						GPUBuffer::Handle ElementBufferObject;
					};

					typedef Map<uint32, MeshBufferHandles> MeshBuffersMap;
					typedef Map<Texture::Handle, FrameBufferHandles> FrameBuffersMap;

				public:
					OpenGLDevice(void);
					~OpenGLDevice(void);

					bool Initialize(void) override;
					bool SecondInitialize(void);

					void SetSampleCount(uint8 Count) override;

					void SetForwardCompatible(bool Value)override;

					void SetClearColor(Color Color) override;

					void SetClearFlags(ClearFlags Flags) override;

					void SetFaceOrder(FaceOrders Order) override;

					void SetCullMode(CullModes Mode) override;

					void SetDepthTestFunction(TestFunctions Function) override;

					void SetStencilTestFunction(CullModes CullMode, TestFunctions Function, int32 Reference, uint32 Mask) override;

					void SetStencilMask(CullModes CullMode, uint32 Mask) override;

					void SetStencilOperation(CullModes CullMode, StencilOperations StencilFailed, StencilOperations DepthFailed, StencilOperations DepthPassed) override;

					void SetBlendFunction(BlendFunctions SourceFactor, BlendFunctions DestinationFactor) override;

					void SetPolygonMode(CullModes CullMode, PolygonModes PolygonMode) override;

					const State &GetState(void) const override
					{
						return m_State;
					}
					void SetState(const State &State) override
					{
						SetClearColor(State.ClearColor);
						SetClearFlags(State.ClearFlags);
						SetFaceOrder(State.FaceOrder);
						SetCullMode(State.CullMode);
						SetDepthTestFunction(State.DepthTestFunction);

						SetStencilTestFunction(CullModes::Front, State.FrontFaceState.StencilTestFunction, State.FrontFaceState.StencilTestFunctionReference, State.FrontFaceState.StencilTestFunctionMask);
						SetStencilTestFunction(CullModes::Back, State.BackFaceStace.StencilTestFunction, State.BackFaceStace.StencilTestFunctionReference, State.BackFaceStace.StencilTestFunctionMask);
						SetStencilTestFunction(CullModes::Both, State.BothFaceState.StencilTestFunction, State.BothFaceState.StencilTestFunctionReference, State.BothFaceState.StencilTestFunctionMask);

						SetStencilMask(CullModes::Front, State.FrontFaceState.StencilMask);
						SetStencilMask(CullModes::Back, State.BackFaceStace.StencilMask);
						SetStencilMask(CullModes::Both, State.BothFaceState.StencilMask);

						SetStencilOperation(CullModes::Front, State.FrontFaceState.StencilOperationStencilFailed, State.FrontFaceState.StencilOperationDepthFailed, State.FrontFaceState.StencilOperationDepthPassed);
						SetStencilOperation(CullModes::Back, State.BackFaceStace.StencilOperationStencilFailed, State.BackFaceStace.StencilOperationDepthFailed, State.BackFaceStace.StencilOperationDepthPassed);
						SetStencilOperation(CullModes::Both, State.BothFaceState.StencilOperationStencilFailed, State.BothFaceState.StencilOperationDepthFailed, State.BothFaceState.StencilOperationDepthPassed);

						SetBlendFunction(State.BlendFunctionSourceFactor, State.BlendFunctionDestinationFactor);

						SetPolygonMode(CullModes::Front, State.FrontFaceState.PolygonMode);
						SetPolygonMode(CullModes::Back, State.BackFaceStace.PolygonMode);
						SetPolygonMode(CullModes::Both, State.BothFaceState.PolygonMode);
					}

					bool CreateProgram(cstr VertexShader, cstr FragmentShader, Program::Handle &Handle) override;
					bool DestroyProgram(Program::Handle Handle) override;
					bool BindProgram(Program::Handle Handle) override;
					bool QueryProgramActiveConstants(Program::Handle Handle, Program::ConstantDataList &Constants) override;
					bool GetProgramConstantHandle(Program::Handle Handle, const String &Name, Program::ConstantHandle &ConstantHandle) override;
					bool SetProgramFloat32(Program::ConstantHandle Handle, float32 Value) override;
					bool SetProgramVector2(Program::ConstantHandle Handle, const Vector2F &Value) override;
					bool SetProgramVector3(Program::ConstantHandle Handle, const Vector3F &Value) override;
					bool SetProgramVector4(Program::ConstantHandle Handle, const Vector4F &Value) override;
					bool SetProgramMatrix4(Program::ConstantHandle Handle, const Matrix4F &Value) override;
					bool SetProgramTexture(Program::ConstantHandle Handle, Texture::Handle Value) override;

					bool CreateTexture2D(const byte *Data, uint32 Width, uint32 Height, Texture::Formats Format, Texture::Handle &Handle) override;
					bool DestroyTexture(Texture::Handle Handle) override;
					bool BindTexture2D(Program::Handle Handle) override;
					bool SetTexture2DVerticalWrapping(Texture::Handle Handle, Texture::WrapModes Mode) override;
					bool SetTexture2DHorizontalWrapping(Texture::Handle Handle, Texture::WrapModes Mode) override;
					bool SetTexture2DMinifyFilter(Texture::Handle Handle, Texture::MinifyFilters Filter) override;
					bool SetTexture2DMagnifyFilter(Texture::Handle Handle, Texture::MagnfyFilters Filter) override;
					bool GenerateMipMap(Texture::Handle Handle) override;

					bool CreateRenderTarget(uint32 Width, uint32 Height, RenderTarget::Formats Format, RenderTarget::AttachmentPoints Point, RenderTarget::Handle &Handle) override;
					bool DestroyRenderTarget(RenderTarget::Handle Handle) override;
					bool BindRenderTarget(RenderTarget::Handle Handle) override;

					bool CreateMesh(const SubMeshInfo *Info, BufferUsages Usage, GPUBuffer::Handle &Handle) override;
					bool DestroyMesh(GPUBuffer::Handle Handle) override;
					bool BindMesh(GPUBuffer::Handle Handle) override;

					bool CreateWindow(uint16 Width, uint16 Height, cstr Title, Window::Handle &Handle) override;
					bool DestroyWindow(Window::Handle Handle) override;

					void Clear(void) override;

					void Draw(DrawModes Mode, uint32 Count) override;

					void SwapBuffers(Window::Handle Handle) override;

					void PollEvents(void) override;
					bool WindowShouldClose(Window::Handle Handle) override;

					INLINE cstr GetLastError(void) const override
					{
						return m_LastError;
					}

				private:
					str m_LastError;

					uint8 m_SampleCount;
					bool m_ForwardCompatible;

					State m_State;

					Program::Handle m_LastProgram;
					GPUBuffer::Handle m_LastMeshBuffer;
					GPUBuffer::Handle m_LastFrameBuffer;

					FrameBuffersMap m_FrameBuffers;

					MeshBuffersMap m_MeshBuffers;
					uint32 m_LastMeshNumber;
					uint8 m_LastActiveTextureUnitIndex;
				};
			}
		}
	}
}

#endif