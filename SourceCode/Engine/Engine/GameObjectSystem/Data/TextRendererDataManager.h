// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef TEXT_RENDERER_DATA_MANAGER_H
#define TEXT_RENDERER_DATA_MANAGER_H

#include <GameObjectSystem\Data\ComponentDataManager.h>
#include <FontSystem\Font.h>

namespace Engine
{
	using namespace FontSystem;

	namespace GameObjectSystem
	{
		namespace Data
		{
			class GAMEOBJECTSYSTEM_API TextRendererDataManager : public ComponentDataManager
			{
			private:
				struct ColdData
				{
				public:
					FontHandle *Font;
					Material *Material;
					WString Text;
					int8 Alignment;
					float32 Size;
					float32 OutlineThickness;
				};

			public:
				TextRendererDataManager(SceneData *SceneData);

				IDType Create(void) override;

				void SetFont(IDType ID, FontHandle *Mesh);
				void SetMaterial(IDType ID, Material *Material);
				void SetText(IDType ID, const WString &Text);
				void SetRightToLeft(IDType ID, bool RightToLeft);
				void SetSize(IDType ID, float32 Size);
				void SetOutlineThicknes(IDType ID, float32 OutlineThickness);

				void Update(void) override
				{
				}

				void Render(void) override;

			private:
				DataContainer<ColdData> m_Data;

				DynamicSizeAllocator m_DataAllocator;
			};
		}
	}
}

#endif