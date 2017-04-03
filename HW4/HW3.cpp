/*
~ Homework 4 ~

Zak Fahey - Task 1
Evan Vujcec - Task 2, Remove and balancing methods in Task 1
*/
#include "Exceptions.h"
#include "SearchTree.h"
#include "Word.h"
#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>

using namespace std;

// finds and prints a word and its frequency
void printWord(SearchTree<Word> *book)
{
	string word;
	cout << "Enter word: ";
	cin >> word;
	Word word_in;
	word_in.word = word;
	cout << "Frequency: " << book->Find(&word_in)->frequency << endl;
}

// prints word distribution
void printWordDistribution(vector<Word*> *wordList)
{
	for each (Word *var in *wordList)
	{
		cout << var->word << ": " << var->frequency << endl;
	}
}

// puts book data into a search tree and returns whether it was successful
void readBookData(string filename, SearchTree<Word> *book)
{
	ifstream stream;
	stream.open(filename, ios::in);
	while (stream.fail())
	{
		cout << "The file '" << filename << "' does not exist." << endl;
		cout << "Enter a valid filename:";
		cin >> filename;
		stream.open(filename, ios::in);
	}

	string line;
	// skip "Contents:" header
	while (true)
	{
		getline(stream, line);
		if (line == "Contents:" || line == "Contents: ")
		{
			break;
		}
	}

	while (true)
	{
		string wd = "";
		char next;
		while (true)
		{
			next = stream.get();
			if (stream.fail())
				break;
			if (next > 47 && next < 58)
				wd += next;
			else if (next > 64 && next < 91)
				wd += (next + 32);
			else if (next > 96 && next < 123)
				wd += next;
			else
				break;
		}

		if (stream.fail())
			break;

		if (wd != "")
		{
			Word *word = new Word;
			word->word = wd;
			word->frequency = 1;
			try
			{
				book->Insert(word);
			}
			catch (ItemAlreadyExistsException)
			{
				book->Find(word)->frequency++;
			}
		}
	}

	stream.close();
}

// method containing all the main code for the assignment
void analyzeBook()
{
	string input;
	cout << "Enter filename: ";
	cin >> input;
	bool success = false;
	SearchTree<Word> *book = new SearchTree<Word>();
	readBookData(input, book);
	bool cont = true;
	while (cont)
	{
		int choice;
		do
		{
			cout << endl;
			cout << "1. Search word" << endl;
			cout << "2. Display words ascending" << endl;
			cout << "3. Display words descending" << endl;
			cout << "4. Exit" << endl;
			cout << "Enter choice: ";
			cin >> choice;
			cout << endl;
		} while (choice < 1 || choice > 4);
		switch (choice)
		{
		case 1:
			printWord(book);
			break;
		case 2:
			printWordDistribution(&book->GetAllAscending());
			break;
		case 3:
			printWordDistribution(&book->GetAllDescending());
			break;
		case 4:
			cont = false;
			break;
		}
	}
	book->EmptyTree();
	delete book;
}

int main()
{
	while (true)
	{
		analyzeBook();
		char answer;
		do
		{
			cout << "Would you like to process another book?(y/n): ";
			cin >> answer;
		} while (answer != 'y' && answer != 'n');
		if (answer == 'n')
			break;
	}
}