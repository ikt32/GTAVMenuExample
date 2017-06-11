#pragma once
#include <Windows.h>
#include <string>

namespace Paths {
	const std::string	GetRunningExecutableFolder();
	const std::string	GetRunningExecutableName();
	const std::string	GetRunningExecutableNameWithoutExtension();

	const std::string	GetModuleFolder(const HMODULE module);
	const std::string	GetModuleName(const HMODULE module);
	const std::string	GetModuleNameWithoutExtension(const HMODULE module);

	const std::string	GetModuleFolder();
	const std::string	GetModuleName();
	const std::string	GetModuleNameWithoutExtension();

	void				SetOurModuleHandle(const HMODULE module);
	const HMODULE		GetOurModuleHandle();
	void				SetModuleInfo(std::string folder, std::string name);
}
