#include "Translate.h"
#include "String.h"
#include "TranslateNames.h"
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

static std::unordered_map<std::string_view, std::basic_string<char32_t>> const &NameTable()
{
	static const auto table = []() {
		std::unordered_map<std::string_view, std::basic_string<char32_t>> t;
		t.reserve(toolNameTableSize * 2);
		for (unsigned i = 0; i < toolNameTableSize; ++i)
		{
			t.emplace(std::string_view(toolNameTable[i].en), ByteString(toolNameTable[i].ru).FromUtf8());
		}
		return t;
	}();
	return table;
}

String TranslateToolName(String const &name)
{
	auto const &t = NameTable();
	auto ascii = name.ToUtf8();
	auto it = t.find(std::string_view(ascii.data(), ascii.size()));
	if (it != t.end())
	{
		return String(it->second.data(), it->second.size());
	}
	return name;
}

String TranslateToolNameLower(String const &name)
{
	String out = TranslateToolName(name);
	for (auto &ch : out)
	{
		if (ch >= U'A' && ch <= U'Z') ch += 32;
		else if (ch >= U'\u0410' && ch <= U'\u042F') ch += 32;
		else if (ch == U'\u0401') ch = U'\u0451';
	}
	return out;
}
