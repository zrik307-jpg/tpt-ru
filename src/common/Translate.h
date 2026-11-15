#pragma once
#include <string>
#include <cstddef>

struct TranslationEntry
{
	const char *en;
	const char *ru;
};
extern const TranslationEntry translationTable[];
extern const unsigned translationTableSize;

// Look up an English string literal. If the table has a Russian version, it is
// returned; otherwise the literal is converted as before (ASCII or UTF-8).
std::basic_string<char32_t> TranslateAscii(char const *data, std::size_t size);
std::basic_string<char32_t> TranslateUtf8(char const *data, std::size_t size);
