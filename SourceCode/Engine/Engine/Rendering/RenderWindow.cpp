// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Rendering\RenderWindow.h>
#include <Common\BitwiseUtils.h>

namespace Engine
{
	namespace Rendering
	{
#define SET_STYLE_STATE(Style, Enabled) \
		{ \
			PlatformWindow::Styles style = PlatformWindow::GetStyle(GetHandle()); \
			if (Enabled) \
				style |= Style; \
			else \
				style &= ~Style; \
			PlatformWindow::SetStyle(GetHandle(), style); \
		}

		RenderWindow::RenderWindow(const String& Name) :
			Window(Name)
		{
			Initialize();
			SetIsVisible(true);
		}

		void RenderWindow::SetIsFullscreen(bool Value)
		{
			if (Value)
			{
				SetIsTopMost(true);
				SetShowCaption(false);
				SetShowSystemMenu(false);
				SetShowMinimizeBox(false);
				SetShowMaximizeBox(false);
				SetShowFrame(false);

				SetState(Window::States::Maximized);
			}
			else
			{
				SetIsTopMost(false);
				SetShowCaption(true);
				SetShowSystemMenu(true);
				SetShowMinimizeBox(true);
				SetShowMaximizeBox(true);
				SetShowFrame(true);

				SetState(Window::States::Noraml);
			}
		}
	}
}