#undef EOF

#pragma comment(lib, "wininet.lib")

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <limits>
#include <map>
#include <codecvt>
#include <regex>
#include <filesystem>
#include <thread>
#include <shlobj.h>
#include "utils.hpp"

constexpr auto BUFFER_SZ = 8192;
constexpr auto APP_SUB_PATH = "\\modules\\discord_krisp-1\\discord_krisp\\index.js";
constexpr auto THEME_OVERRIDE_FOLDER_NAME = "custom_themes";

constexpr auto SUPER_DARK_THEME = ".theme-dark .container-2cd8Mz {background-color: hsl(0deg 0% 5%);}.theme-dark {--saturation-factor: 1;--header-primary: #ffffff;--header-secondary: hsl(216,3.7%,73.5%);--text-normal: hsl(210,2.9%,86.7%);--text-muted: hsl(214,4%,65.3%);--text-link: hsl(197,100%,47.8%);--text-link-low-saturation: hsl(197,100%,52.9%);--text-positive: hsl(139,51.6%,52.2%);--text-warning: hsl(38,95.7%,54.1%);--text-danger: hsl(359,82%,73.9%);--text-brand: hsl(235,86.1%,77.5%);--interactive-normal: hsl(0deg 0% 100%);--interactive-hover: hsl(210,2.9%,86.7%);--interactive-active: hsl(0deg 0% 100%);--interactive-muted: hsl(217,7.6%,33.5%);--background-primary: hsl(0deg 0% 5%) !important;--background-secondary: hsl(0deg 0% 5%);--background-secondary-alt: hsl(0deg 0% 5%);--background-tertiary: hsl(0deg 0% 5%);--background-accent: hsl(0deg 0% 10%);--background-floating: hsl(0deg 0% 3%);--background-nested-floating: hsl(0deg 0% 10%);--background-mobile-primary: hsl(220,7.7%,22.9%);--background-mobile-secondary: hsl(223,6.9%,19.8%);--chat-background: hsl(0deg 0% 17%);--chat-border: hsl(216,7.2%,13.5%);--chat-input-container-background: hsl(0deg 0% 0%);--background-modifier-hover: hsla(217,7.6%,33.5%,0.4);--background-modifier-active: hsla(217,7.6%,33.5%,0.48);--background-modifier-selected: hsl(0deg 0% 19% / 60%);--background-modifier-accent: hsla(217,7.6%,33.5%,0.48);--info-positive-background: hsla(139,47.3%,43.9%,0.1);--info-positive-foreground: hsl(139,47.3%,43.9%);--info-positive-text: hsl(0,0%,100%);--info-warning-background: hsla(38,95.7%,54.1%,0.1);--info-warning-foreground: hsl(38,95.7%,54.1%);--info-warning-text: hsl(0,0%,100%);--info-danger-background: hsla(359,82.6%,59.4%,0.1);--info-danger-foreground: hsl(359,82.6%,59.4%);--info-danger-text: hsl(0,0%,100%);--info-help-background: hsla(197,100%,47.8%,0.1);--info-help-foreground: hsl(197,100%,47.8%);--info-help-text: hsl(0,0%,100%);--status-positive-background: hsl(139,47.1%,33.3%);--status-positive-text: hsl(0,0%,100%);--status-warning-background: hsl(38,95.7%,54.1%);--status-warning-text: hsl(0,0%,0%);--status-danger-background: hsl(359,82.6%,59.4%);--status-danger-text: hsl(0,0%,100%);--status-danger: hsl(359,82.6%,59.4%);--status-positive: hsl(139,47.3%,43.9%);--status-warning: hsl(38,95.7%,54.1%);--button-danger-background: hsl(359,66.7%,54.1%);--button-danger-background-hover: hsl(359,56.3%,40.4%);--button-danger-background-active: hsl(359,56.4%,35.1%);--button-danger-background-disabled: hsl(359,66.7%,54.1%);--button-positive-background: hsl(139,47.1%,33.3%);--button-positive-background-hover: hsl(138,46.8%,24.3%);--button-positive-background-active: hsl(138,46.9%,22.2%);--button-positive-background-disabled: hsl(139,47.1%,33.3%);--button-secondary-background: hsl(0deg 0% 23%);--button-secondary-background-hover: hsl(213,5%,42.9%);--button-secondary-background-active: hsl(218,4.6%,46.9%);--button-secondary-background-disabled: hsl(217,7.6%,33.5%);--button-outline-danger-text: hsl(0,0%,100%);--button-outline-danger-border: hsl(359,82.6%,59.4%);--button-outline-danger-background: hsla(0,0%,100%,0);--button-outline-danger-background-hover: hsl(359,66.7%,54.1%);--button-outline-danger-text-hover: hsl(0,0%,100%);--button-outline-danger-border-hover: hsl(359,66.7%,54.1%);--button-outline-danger-background-active: hsl(359,56.7%,48%);--button-outline-danger-text-active: hsl(0,0%,100%);--button-outline-danger-border-active: hsl(359,66.7%,54.1%);--button-outline-positive-text: hsl(0,0%,100%);--button-outline-positive-border: hsl(139,47.3%,43.9%);--button-outline-positive-background: hsla(0,0%,100%,0);--button-outline-positive-background-hover: hsl(139,47.1%,33.3%);--button-outline-positive-text-hover: hsl(0,0%,100%);--button-outline-positive-border-hover: hsl(139,47.1%,33.3%);--button-outline-positive-background-active: hsl(138,46.8%,24.3%);--button-outline-positive-text-active: hsl(0,0%,100%);--button-outline-positive-border-active: hsl(138,46.8%,24.3%);--button-outline-brand-text: hsl(0,0%,100%);--button-outline-brand-border: hsl(235,85.6%,64.7%);--button-outline-brand-background: hsla(0,0%,100%,0);--button-outline-brand-background-hover: hsl(235,85.6%,64.7%);--button-outline-brand-text-hover: hsl(0,0%,100%);--button-outline-brand-border-hover: hsl(235,85.6%,64.7%);--button-outline-brand-background-active: hsl(235,51.4%,52.4%);--button-outline-brand-text-active: hsl(0,0%,100%);--button-outline-brand-border-active: hsl(235,51.4%,52.4%);--button-outline-primary-text: hsl(0,0%,100%);--button-outline-primary-border: hsl(217,7.6%,33.5%);--button-outline-primary-background: hsla(0,0%,100%,0);--button-outline-primary-background-hover: hsl(217,7.6%,33.5%);--button-outline-primary-text-hover: hsl(0,0%,100%);--button-outline-primary-border-hover: hsl(217,7.6%,33.5%);--button-outline-primary-background-active: hsl(213,5%,42.9%);--button-outline-primary-text-active: hsl(0,0%,100%);--button-outline-primary-border-active: hsl(213,5%,42.9%);--modal-background: hsl(0deg 0% 0%);--modal-footer-background: hsl(223,6.9%,19.8%);--scrollbar-thin-thumb: hsl(216,7.2%,13.5%);--scrollbar-thin-track: hsla(0,0%,0%,0);--scrollbar-auto-thumb: hsl(216,7.2%,13.5%);--scrollbar-auto-track: hsl(210,9.8%,20%);--scrollbar-auto-scrollbar-color-thumb: hsl(216,7.2%,13.5%);--scrollbar-auto-scrollbar-color-track: hsl(223,6.9%,19.8%);--input-background: hsl(216,7.2%,13.5%);--input-placeholder-text: hsl(216,2.1%,53.9%);--logo-primary: hsl(0,0%,100%);--control-brand-foreground: hsl(235,86.1%,77.5%);--control-brand-foreground-new: hsl(235,86.1%,77.5%);--background-mentioned: hsla(38,95.7%,54.1%,0.1);--background-mentioned-hover: hsla(38,95.7%,54.1%,0.08);--background-message-hover: hsla(240,11.1%,1.8%,0.07);--background-message-automod: hsla(359,82.6%,59.4%,0.05);--background-message-automod-hover: hsla(359,82.6%,59.4%,0.1);--background-message-highlight: hsla(235,86.1%,77.5%,0.08);--background-message-highlight-hover: hsla(235,86.1%,77.5%,0.06);--channels-default: hsl(223,3.4%,60.2%);--channel-icon: hsl(213,4.1%,57.5%);--channel-text-area-placeholder: hsl(0deg 0% 44%);--channeltextarea-background: hsl(0deg 0% 10%);--activity-card-background: hsl(0deg 0% 10%);--textbox-markdown-syntax: hsl(213,4.1%,57.5%);--spoiler-revealed-background: hsl(220,6.8%,17.3%);--spoiler-hidden-background: hsl(216,7.2%,13.5%);--android-navigation-bar-background: hsl(220,7.7%,7.6%);--android-ripple: hsla(0,0%,100%,0.07);--deprecated-card-bg: hsla(216,7.2%,13.5%,0.6);--deprecated-card-editable-bg: hsla(216,7.2%,13.5%,0.3);--deprecated-store-bg: hsl(220,7.7%,22.9%);--deprecated-quickswitcher-input-background: hsl(218,4.6%,46.9%);--deprecated-quickswitcher-input-placeholder: hsla(0,0%,100%,0.3);--deprecated-text-input-bg: hsl(216,7.2%,13.5%);--deprecated-text-input-border: hsla(0,0%,0%,0.3);--deprecated-text-input-border-hover: hsl(240,11.1%,1.8%);--deprecated-text-input-border-disabled: hsl(216,7.2%,13.5%);--deprecated-text-input-prefix: hsl(210,2.9%,86.7%);--font-primary:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-display:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-headline:'ABC Ginto Nord','Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;}";
constexpr auto BLUE_THEME = "*{--saturation-factor:5!important;--font-primary:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-display:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-headline:'ABC Ginto Nord','Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;}";
constexpr auto GRAYSCALE_THEME = "*{--saturation-factor:0!important;--font-primary:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-display:'Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;--font-headline:'ABC Ginto Nord','Noto Sans','Helvetica Neue',Helvetica,Arial,sans-serif;}";

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string ws2s(const std::wstring& wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

int paramExists(const int argc, wchar_t** argv, const wchar_t* param) {
	size_t plen = wcslen(param);
	for (int i = 1; i < argc; i++) {
		if (wcslen(argv[i]) == plen + 1ull && _wcsicmp(&argv[i][1], param) == 0 && argv[i][0] == '/') { // with slash
			return i;
		}
		else if (wcslen(argv[i]) == plen + 2ull && _wcsicmp(&argv[i][2], param) == 0 && argv[i][0] == '-' && argv[i][1] == '-') { // with double dash
			return i;
		}
	}
	return -1;
}

inline bool fn_exists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}
std::string fs_read(std::string fn) {
	std::ifstream auth_file(fn);
	std::stringstream auth_buffer;
	auth_buffer << auth_file.rdbuf();
	return auth_buffer.str();
}
void fs_write(std::string fn, std::string c) {
	std::ofstream ofs(fn);
	ofs << c;
	ofs.close();
}

bool EndsWith(const std::string& a, const std::string& b) {
	if (b.size() > a.size()) return false;
	return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

int wmain(const int argc, wchar_t** argv) {
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	SetConsoleTitleA("Discord Theme Override");
	Log_Project("Discord Theme Override");

	std::filesystem::path stable_path;
	std::filesystem::path ptb_path;
	std::filesystem::path canary_path;

	/* get or create theme_override file */

	if (!std::filesystem::exists(THEME_OVERRIDE_FOLDER_NAME))
		std::filesystem::create_directory(THEME_OVERRIDE_FOLDER_NAME);

	std::string file_names[] = { "super_dark", "blue", "grayscale" };
	const char* file_contents[] = { SUPER_DARK_THEME, BLUE_THEME, GRAYSCALE_THEME };
	for (int i = 0; i < 3; i++) {
		std::string file_name = std::string(THEME_OVERRIDE_FOLDER_NAME) + "\\" + file_names[i] + ".css";
		if (!std::filesystem::exists(file_name)) {
			std::fstream override_data;
			override_data.open(file_name, std::fstream::in | std::fstream::out | std::fstream::app);
			override_data << file_contents[i];
			override_data.close();
		}
	}

	/* get %LOCALAPPDATA% path */

	PWSTR localappdata_path;

	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &localappdata_path);

	stable_path = localappdata_path;
	stable_path += "\\Discord";
	ptb_path = localappdata_path;
	ptb_path += "\\DiscordPTB";
	canary_path = localappdata_path;
	canary_path += "\\DiscordCanary";

	CoTaskMemFree(localappdata_path);
	
	/* check installs */

	const bool stable_installed = std::filesystem::exists(stable_path);
	const bool ptb_installed = std::filesystem::exists(ptb_path);
	const bool canary_installed = std::filesystem::exists(canary_path);

	if (!stable_installed && !ptb_installed && !canary_installed) {
		Log_Error("No valid Discord install found.\n");
		utils::PressAnyKeyToExit(1);
	}

	std::filesystem::path install_path;
	std::filesystem::path full_path;

	if (stable_installed) Log_WriteListItem("[S]", "Stable\n");
	if (ptb_installed) Log_WriteListItem("[P]", "PTB\n");
	if (canary_installed) Log_WriteListItem("[C]", "Canary\n");

	while (1) {
		char choice;

		Log_Question("Which Discord version do you want to modify? ");

		std::cin >> choice;

		if (choice == 's' && stable_installed) {
			Log_Info("Atempting to install into Discord stable build\n");
			install_path = stable_path;
			break;
		}
		if (choice == 'p' && ptb_installed) {
			Log_Info("Atempting to install into Discord public test build\n");
			install_path = ptb_path;
			break;
		}
		if (choice == 'c' && canary_installed) {
			Log_Info("Atempting to install into Discord canary build\n");
			install_path = canary_path;
			break;
		}

		Log_Warn("Invalid\n");
	}

	/* find target file */

	Log_Info("Getting client data\n");

	int maxVer = 0;
	std::string ver_name_str;
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& folder : recursive_directory_iterator(install_path)) {
		const auto folder_name = folder.path().filename().string();
		if (folder_name.rfind("app-", 0) == 0) {
			int ver = stoi(folder_name.substr(folder_name.length() - 4, folder_name.length()));
			if (ver >= maxVer) {
				ver_name_str = folder_name.substr(4);
				full_path = folder.path();
				maxVer = ver;
			}
		}
	}

	if (!std::filesystem::exists(full_path)) {
		Log_Error("Invalid Discord install detected. Reinstall your Discord client and try again.\n");
		utils::PressAnyKeyToExit(1);
	}
	else {
		Log_Ok("Got client version " + ver_name_str + "\n");
	}

	full_path += APP_SUB_PATH;

	if (!std::filesystem::exists(full_path)) {
		Log_Error("Your Discord install is missing files. Reinstall your Discord client and try again.\n");
		utils::PressAnyKeyToExit(1);
	}

	/* ask which theme we want to install */

	std::vector<std::string> theme_names;
	std::vector<std::string> theme_contents;
	Log_WriteListItem("[0]", "Restore default theme\n");
	int ticker = 0;
	for (const auto& theme : recursive_directory_iterator(THEME_OVERRIDE_FOLDER_NAME)) {
		const auto theme_path = theme.path();
		const auto theme_name = theme_path.filename().string();
		if (EndsWith(theme_name, ".css")) {
			std::fstream file;
			file.open(theme_path, std::fstream::in | std::fstream::out | std::fstream::app);
			if (file) {
				std::stringstream file_contents;
				std::string tmp;
				while (getline(file, tmp)) {
					file_contents << tmp << "\n";
				}
				theme_contents.push_back(file_contents.str());
				theme_names.push_back(theme_name);
				ticker++;
				Log_WriteListItem("[" + std::to_string(ticker) + "]", std::string(theme_name) + "\n");
			}
			file.close();
		}
	}

	if (ticker < 1) {
		Log_Error("No available custom themes exist. Cannot proceed.\n");
		utils::PressAnyKeyToExit(1);
	}

	bool do_remove = false;

	int ichoice;
	while (1) {
		char cchoice;
		Log_Question("Which theme would you like to install? ");
		std::cin >> cchoice;
		ichoice = cchoice - '0';
		if (ichoice == 0) {
			do_remove = true;
			break;
		} else if (ichoice > 0 && ichoice < (ticker + 1)) {
			ichoice--;
			break;
		}
		Log_Warn("Invalid\n");
	}

	!do_remove
		? Log_Info("Attempting to install theme \"" + std::string(theme_names[ichoice]) + "\"\n")
		: Log_Info("Attempting to uninstall themes & shims\n");

	/* inject our custom loader */

	!do_remove
		? Log_Info("Injecting shims into Discord client\n")
		: Log_Info("Flushing shims\n");

	std::fstream index_js;
	index_js.open(full_path, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!index_js) {
		Log_Error("Failed to open file. Try running the program as administrator.");
		utils::PressAnyKeyToExit(1);
	}

	std::stringstream old_data;
	std::stringstream new_data;

	std::string tmp;
	while (getline(index_js, tmp)) {
		old_data << tmp << "\n";
	}

	index_js.close();

	if (old_data.str().rfind("require('./dto_shim.js');", 0) > 0) {
		if (!std::filesystem::remove(full_path)) {
			Log_Error("Failed to overwrite existing file. Try running the program as administrator.\n");
			utils::PressAnyKeyToExit(1);
		}

		new_data
			<< "require('./dto_shim.js');\n"
			<< old_data.str();

		std::fstream new_index_js;
		new_index_js.open(full_path, std::fstream::in | std::fstream::out | std::fstream::app);
		new_index_js << new_data.str();
		new_index_js.close();

		if (!std::filesystem::exists(full_path)) {
			Log_Error("Failed to overwrite existing file. Try running the program as administrator.\n");
			utils::PressAnyKeyToExit(1);
		}
	}

	full_path = full_path.replace_filename(std::string("dto_shim.js"));

	if (std::filesystem::exists(full_path)) {
		if (!std::filesystem::remove(full_path)) {
			Log_Error("Failed to overwrite existing file. Try running the program as administrator.\n");
			utils::PressAnyKeyToExit(1);
		}
	}

	std::fstream shim_file;
	shim_file.open(full_path, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!do_remove) {
		shim_file
			<< "const theme = `" << theme_contents[ichoice] << "`;\n"
			<< "const fake_style = document.createElement('style');\n"
			<< "fake_style.innerHTML = theme;\n"
			<< "document.head.appendChild(fake_style);";
		Log_Ok("Theme successfully installed. Reload Discord for changes to take effect.\n");
	} else {
		shim_file << "\n";
		Log_Ok("All themes & shims successfully removed. Reload your Discord client for changes to take effect.\n");
	}
	shim_file.close();

	/* exit */
	utils::PressAnyKeyToExit(0);
}