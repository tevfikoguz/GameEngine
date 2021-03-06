// Copyright 2016-2020 ?????????????. All Rights Reserved.
using Engine.Frontend.System;
using System.Collections.Generic;
using System.IO;

namespace Engine.Frontend.Utilities
{
	static class FileSystemUtilites
	{
		public static string[] GetAllFiles(string DirectoryAddress, params string[] SearchPatterns)
        {
            List<string> files = new List<string>();

            if (!Directory.Exists(DirectoryAddress))
				return files.ToArray();

			if (SearchPatterns.Length == 0)
				return Directory.GetFiles(DirectoryAddress, "*.*", SearchOption.AllDirectories);
			
			foreach (string pattern in SearchPatterns)
				files.AddRange(Directory.GetFiles(DirectoryAddress, pattern, SearchOption.AllDirectories));

			return files.ToArray();
		}

		public static void CopyAllFiles(string SourceDirectoryAddress, string DestinationDirectoryAddress, bool Overwrite = true, params string[] SearchPattern)
		{
			string[] files = GetAllFiles(SourceDirectoryAddress, SearchPattern);

			DestinationDirectoryAddress = DestinationDirectoryAddress.Replace('/', '\\');

			if (!DestinationDirectoryAddress.EndsWith("\\"))
				DestinationDirectoryAddress += "\\";

			if (!Directory.Exists(DestinationDirectoryAddress))
				Directory.CreateDirectory(DestinationDirectoryAddress);

			foreach (string file in files)
				File.Copy(file, DestinationDirectoryAddress + Path.GetFileName(file), Overwrite);
		}

		public static string PathSeperatorCorrection(string Path)
		{
			return Path.Replace((EnvironmentHelper.Platform == EnvironmentHelper.Platforms.Windows ? '/' : '\\'), EnvironmentHelper.PathSeparator);
		}

		public static string GetParentDirectory(string Directory)
		{
			Directory = PathSeperatorCorrection(Directory);

			if (Directory.EndsWith("" + EnvironmentHelper.PathSeparator))
				Directory = Directory.Substring(0, Directory.Length - 1);

			return Path.GetDirectoryName(Directory);
		}
	}
}
