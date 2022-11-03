#pragma once

#if defined(DISABLE_OUTPUT)
	#define Log(content) 
#else
	#define Log(content) std::wcout << content
#endif


#include "logging.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <map>

namespace utils
{
	std::wstring GetFullTempPath();
	bool ReadFileToMemory(const std::wstring& file_path, std::vector<uint8_t>* out_buffer);
	std::vector<uint8_t> DownloadFileToMemory(const std::wstring& file_path);
	bool CreateFileFromMemory(const std::wstring& desired_file_path, const char* address, size_t size);
	void PressAnyKeyToExit(int exit_code);
	std::vector<std::string> StrSplit(std::string str, std::string delim);
	BOOL IsProcessRunning(const std::wstring& processName);
	DWORD FindProcessId(const std::wstring& processName);
	bool AskYN(std::string question);
	BOOLEAN bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	uintptr_t FindPattern(uintptr_t dwAddress, uintptr_t dwLen, BYTE* bMask, char* szMask);
	PVOID FindSection(char* sectionName, uintptr_t modulePtr, PULONG size);
}