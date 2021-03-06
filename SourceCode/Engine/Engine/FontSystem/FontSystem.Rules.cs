// Copyright 2016-2020 ?????????????. All Rights Reserved.
namespace Engine.Frontend
{
	class FontSystemRules : BuildRules
	{
		public override string ModuleName
		{
			get { return "FontSystem"; }
		}

		public class Rule : RuleBase
		{
			public override string TargetName
			{
				get { return "FontSystem"; }
			}

			public override LibraryUseTypes LibraryUseType
			{
				get { return LibraryUseTypes.DynamicLibrary; }
			}

			public override string[] IncludeModulesName
			{
				get { return new string[] { "ResourceSystem" }; }
			}

			public override string[] DependencyStaticLibraries
			{
				get { return new string[] { "opengl32.lib", "Glu32.lib" }; }
			}

			public override string[] DependencyModulesName
			{
				get { return new string[] { "Containers", "MemoryManagement", "Rendering", "FreeType", "Utility", "Platform" }; }
			}
		}
	}
}