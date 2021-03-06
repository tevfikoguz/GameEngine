// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Utility\Window.h>

namespace Engine
{
	namespace Utility
	{
		Window::Window(const String & Name) :
			m_Handle(0),
			m_Name(Name),
			m_ShouldClose(false)
		{
		}

		Window::~Window(void)
		{
			if (m_Handle != 0)
				PlatformWindow::Destroy(m_Handle);
		}

		bool Window::Initialize(void)
		{
			m_Handle = PlatformWindow::Create(PlatformOS::GetExecutingModuleInstance(), m_Name.GetValue(), PlatformWindow::Styles::OverlappedWindow, [&](PlatformWindow::WindowMessages Message) { return MessageProcedure(Message); });

			return false;
		}

		void Window::SetTitle(const String & Value)
		{
			m_Title = Value;

			PlatformWindow::SetTitle(m_Handle, Value.GetValue());
		}

		void Window::SetPosition(const Vector2I & Value)
		{
			m_Position = Value;

			PlatformWindow::SetPosition(m_Handle, m_Position.X, m_Position.Y);
		}

		void Window::SetSize(const Vector2I &Value)
		{
			m_Size = Value;

			PlatformWindow::SetSize(m_Handle, m_Size.X, m_Size.Y);
		}

		void Window::SetFullscreen(bool Value)
		{
			m_IsFullscreen = true;
		}

		bool Window::MessageProcedure(PlatformWindow::WindowMessages Message)
		{
			switch (Message)
			{
			case PlatformWindow::WindowMessages::Create:
				break;
			case PlatformWindow::WindowMessages::Size:
			{
				uint16 x;
				uint16 y;

				PlatformWindow::GetClientSize(m_Handle, x, y);
				if (x != m_ClientSize.X || y != m_ClientSize.Y)
				{
					m_ClientSize.X = x;
					m_ClientSize.Y = y;

					PlatformWindow::GetSize(m_Handle, x, y);
					m_Size.X = x;
					m_Size.Y = y;

					for each (auto listener in m_Listeners)
						listener->OnWindowResized(this);
				}
			} break;
			case PlatformWindow::WindowMessages::Move:
			{
				int16 x;
				int16 y;

				PlatformWindow::GetClientPosition(m_Handle, x, y);
				if (x != m_ClientPosition.X || y != m_ClientPosition.Y)
				{
					m_ClientPosition.X = x;
					m_ClientPosition.Y = y;

					PlatformWindow::GetPosition(m_Handle, x, y);
					m_Position.X = x;
					m_Position.Y = y;

					for each (auto listener in m_Listeners)
						listener->OnWindowPositioned(this);
				}
			} break;
			case PlatformWindow::WindowMessages::Close:
				m_ShouldClose = true;
				break;
			}

			return false;
		}
	}
}