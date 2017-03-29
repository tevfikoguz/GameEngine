// Copyright 2016-2017 ?????????????. All Rights Reserved.
namespace Engine.Frontend
{
	class MemoryManagementRules : BuildRules
	{
		public override string TargetName
		{
			get { return "MemoryManagement"; }
		}

		public override LibraryUseTypes LibraryUseType
		{
			get { return LibraryUseTypes.DynamicLibrary; }
		}

		public override string[] DependencyModulesName
		{
			get { return new string[] { "Platform" }; }
		}
	}
}