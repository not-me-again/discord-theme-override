#include <Windows.h>
#include <wininet.h>
#include <urlmon.h>
#pragma comment(lib , "urlmon.lib")
#include <atlbase.h>
#include "utils.hpp"

std::wstring utils::GetFullTempPath() {
	wchar_t temp_directory[MAX_PATH + 1] = { 0 };
	const uint32_t get_temp_path_ret = GetTempPathW(sizeof(temp_directory) / 2, temp_directory);
	if (!get_temp_path_ret || get_temp_path_ret > MAX_PATH + 1) {
		Log_Warn("Failed to get temp path\n");
		return L"";
	}
	if (temp_directory[wcslen(temp_directory) - 1] == L'\\')
		temp_directory[wcslen(temp_directory) - 1] = 0x0;

	return std::wstring(temp_directory);
}

std::vector<std::string> utils::StrSplit(std::string str, std::string delim) {
	size_t pos = 0;
	std::vector<std::string> found{};
	while ((pos = str.find(delim)) != std::string::npos) {
		found.push_back(str.substr(0, pos));
		str.erase(0, pos + delim.length());
	}
	return found;
}

void utils::PressAnyKeyToExit(int exit_code) {
	std::cout << " Press any key to exit...";
	int _ = _getch();
	exit(exit_code);
}

DWORD utils::FindProcessId(const std::wstring& processName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

BOOL utils::IsProcessRunning(const std::wstring& processName) {
	DWORD pid = utils::FindProcessId(processName);
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

bool utils::AskYN(std::string question) {
	char userAnswer;
	Log_Question(question + " [y/n] ");
	std::cin >> userAnswer;
	return userAnswer == 'y' || userAnswer == 'Y' || userAnswer == 'yes';
}

bool utils::ReadFileToMemory(const std::wstring& file_path, std::vector<uint8_t>* out_buffer) {
	std::ifstream file_ifstream(file_path, std::ios::binary);

	if (!file_ifstream)
		return false;

	out_buffer->assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
	file_ifstream.close();

	return true;
}

std::vector<uint8_t> utils::DownloadFileToMemory(const std::wstring& file_url) {
	std::vector<uint8_t> out_buffer{};

	HINTERNET hInternetSession = InternetOpen(L"Microsoft Internet Explorer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	HINTERNET hURL = InternetOpenUrl(hInternetSession, file_url.c_str(), NULL, 0, 0, 0); 
	if (hURL == 0)
		return out_buffer;

	DWORD dataSize = 7200;
	uint8_t dataReceived[7200];
	DWORD dwBytesRead = 0;
	do
	{
		if (InternetReadFile(hURL, dataReceived, dataSize, &dwBytesRead))
		{
			if (dwBytesRead == 0)
				break;

			return std::vector<uint8_t>(std::begin(dataReceived), std::end(dataReceived));
		}
		else
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				return out_buffer;
			}

			delete[] dataReceived;
		}
	} while (true);

	InternetCloseHandle(hInternetSession);
	InternetCloseHandle(hURL);

	return out_buffer;
}

bool utils::CreateFileFromMemory(const std::wstring& desired_file_path, const char* address, size_t size) {
	std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);

	if (!file_ofstream.write(address, size)) {
		file_ofstream.close();
		return false;
	}

	file_ofstream.close();
	return true;
}

BOOLEAN utils::bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	return (*szMask) == 0;
}

uintptr_t utils::FindPattern(uintptr_t dwAddress, uintptr_t dwLen, BYTE* bMask, char* szMask) {
	size_t max_len = dwLen - strlen(szMask);
	for (uintptr_t i = 0; i < max_len; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (uintptr_t)(dwAddress + i);
	return 0;
}

PVOID utils::FindSection(char* sectionName, uintptr_t modulePtr, PULONG size) {
	size_t namelength = strlen(sectionName);
	PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)(modulePtr + ((PIMAGE_DOS_HEADER)modulePtr)->e_lfanew);
	PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(headers);
	for (DWORD i = 0; i < headers->FileHeader.NumberOfSections; ++i) {
		PIMAGE_SECTION_HEADER section = &sections[i];
		if (memcmp(section->Name, sectionName, namelength) == 0 &&
			namelength == strlen((char*)section->Name)) {
			if (!section->VirtualAddress) {
				return 0;
			}
			if (size) {
				*size = section->Misc.VirtualSize;
			}
			return (PVOID)(modulePtr + section->VirtualAddress);
		}
	}
	return 0;
}