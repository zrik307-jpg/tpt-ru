#include "Translate.h"
#include "String.h"
#include <string_view>
#include <unordered_map>

static std::unordered_map<std::string_view, std::basic_string<char32_t>> const &Table()
{
	static const auto table = []() {
		std::unordered_map<std::string_view, std::basic_string<char32_t>> t;
		t.reserve(translationTableSize * 2);
		for (unsigned i = 0; i < translationTableSize; ++i)
		{
			t.emplace(std::string_view(translationTable[i].en), ByteString(translationTable[i].ru).FromUtf8());
		}
		return t;
	}();
	return table;
}

std::basic_string<char32_t> TranslateAscii(char const *data, std::size_t size)
{
	auto const &t = Table();
	auto it = t.find(std::string_view(data, size));
	if (it != t.end())
	{
		return it->second;
	}
	return ByteString(data, size).FromAscii();
}

std::basic_string<char32_t> TranslateUtf8(char const *data, std::size_t size)
{
	auto const &t = Table();
	auto it = t.find(std::string_view(data, size));
	if (it != t.end())
	{
		return it->second;
	}
	return ByteString(data, size).FromUtf8();
}
