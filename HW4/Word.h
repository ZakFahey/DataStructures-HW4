#pragma once
#include <string>

class Word
{
public:
	std::string word;
	int frequency;
	bool operator>(const Word &word_in) { return word > word_in.word; }
	bool operator<(const Word &word_in) { return word < word_in.word; }
	bool operator==(const Word &word_in) { return word == word_in.word; }
};

