// Copyright 2016-2017 ?????????????. All Rights Reserved.
namespace Engine.Frontend
{
    class TestRenderingRules : BuildRules
    {
        public override string ModuleName
        {
            get { return "TestRendering"; }
        }

        public class Rule : RuleBase
        {
            public override string TargetName
            {
                get { return "TestRendering"; }
            }

            public override LibraryUseTypes LibraryUseType
            {
                get { return LibraryUseTypes.Executable; }
            }

            public override string[] DependencyModulesName
            {
                get { return new string[] { "Rendering", "Vulkan", "Parallelizing", "MemoryManagement", "Containers" }; }
			}

			public override string[] PreprocessorDefinitions
			{
				get { return new string[] { "VK_USE_PLATFORM_WIN32_KHR" }; }
			}
		}
    }
}