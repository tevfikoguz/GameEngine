// Copyright 2016-2020 ?????????????. All Rights Reserved.
#include <Rendering\Private\Commands\ClearCommand.h>

namespace Engine
{
	namespace Rendering
	{
		namespace Private
		{
			namespace Commands
			{
				ClearCommand::ClearCommand(IDevice::ClearFlags Flags, Color Color) :
					m_Flags(Flags),
					m_Color(Color)
				{
				}

				void ClearCommand::Execute(IDevice * Device)
				{
					Device->SetClearColor(m_Color);
					Device->Clear(m_Flags);
				}
			}
		}
	}
}
