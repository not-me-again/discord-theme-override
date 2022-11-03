#include "printa/printa/printa.hpp"
#include <sstream>
#include <ostream>
#include <iomanip>

#define Log_Project printa->project<3>
#define Log_Loading printa->print<loading>
#define Log_Question printa->print<input>
#define Log_Ok printa->print<ok>
#define Log_Error printa->print<fail>
#define Log_Warn printa->print<warn>
#define Log_Info printa->print<info>
#define Log_WriteListItem printa->list_item

#ifndef wstring_to_string
__inline std::string wstring_to_string(std::wstring wstr) {
	return std::string(wstr.begin(), wstr.end());
}
#endif
#ifndef tohex
template<typename T>
__inline std::string tohex(T const& input) {
	std::stringstream stream;
	stream << std::hex << input;
	std::string hex_str = std::string(stream.str());
	std::transform(hex_str.begin(), hex_str.end(), hex_str.begin(), ::toupper);
	return hex_str;
}
#endif
#ifndef tostring
#define tostring std::to_string
#endif

/*template<typename T>
std::string tostring(T const& input) {
	return tohex;
}*/