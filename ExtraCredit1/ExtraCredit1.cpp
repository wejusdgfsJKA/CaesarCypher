#include <iostream>
#include "Trie.cpp"
float ChiSquaredDistanceFormula(float C, float E) {
	return (C - E) * (C - E) / E;
}
void ReadDistribution(float frequencies[], string inputfile) {
	ifstream distribution(inputfile);
	float frequency;
	for (int i = 0; i < 26; i++) {
		distribution >> frequency;
		frequencies[i] = frequency;
	}
}
void ExpectedDistribution(float expectedfreq[], string inputfile) {
	float frequency;
	ifstream expected(inputfile);
	for (int i = 0; i < 26; i++) {
		expected >> frequency;
		expectedfreq[i] = frequency;
	}
}
void NormalizedFrequency(float expectedfreq[], int nrofletters, float normalizedfreq[]) {
	for (int i = 0; i < 26; i++) {
		normalizedfreq[i] = expectedfreq[i] * nrofletters;
	}
}
float ChiSquaredDistance(float freq1[], float freq2[]) {
	float s = 0;
	for (int i = 0; i < 26; i++) {
		s += ChiSquaredDistanceFormula(freq1[i], freq2[i]);
	}
	return s;
}
char ShiftChar(char letter) {
	if (isalpha(letter))
	{
		if (letter == 'a')
		{
			letter = 'z';
		}
		else {
			if (letter == 'A')
			{
				letter = 'Z';
			}
			else
			{
				letter -= 1;
			}
		}
	}
	return letter;
}
void ShiftText(string& text, float freq[]) {
	for (int i = 0; i < text.length(); i++) {
		text[i] = ShiftChar(text[i]);
	}
	float f = freq[0];
	for (int i = 0; i < 25; i++) {
		freq[i] = freq[i + 1];
	}
	freq[25] = f;
}
void ShiftText(string& text) {
	for (int i = 0; i < text.length(); i++) {
		text[i] = ShiftChar(text[i]);
	}
}
void BreakCaesarCypher(string& text, float freq[], float expectedfreq[]) {
	float minchidist = ChiSquaredDistance(freq, expectedfreq);
	string besttext = text;
	for (int i = 0; i < 26; i++) {
		ShiftText(text, freq);
		float chidist = ChiSquaredDistance(freq, expectedfreq);
		if (minchidist > chidist) {
			minchidist = chidist;
			besttext = text;
		}
	}
	text = besttext;
}
bool BreakShortText(string& text, TrieNode* root) {
	if (IsText(text, root)) {
		return true;
	}
	for (int i = 0; i < 26; i++) {
		ShiftText(text);
		if (IsText(text, root)) {
			return true;
		}
	}
	return false;
}
bool ComputeFreq(string text, float freq[]) {
	for (int i = 0; i < 26; i++) {
		freq[i] = 0;
	}
	for (int i = 0; i < text.length(); i++) {
		if (isalpha(text[i])) {
			text[i] = tolower(text[i]);
			freq[int(text[i]) - 'a']++;
		}
	}
	return text.length()>=100;
}
int main()
{
	TrieNode* root = CreateTrieNode(' ', false);
	CreateTrie(root, "Words.txt");
	string defaultstring = "Uf ime ftq nqef ar fuyqe, uf ime ftq iadef ar fuyqe, uf ime ftq msq ar iuepay, uf ime ftq msq ar raaxuetzqee, uf ime ftq qbaot ar nqxuqr,uf ime ftq qbaot ar uzodqpgxufk, uf ime ftq eqmeaz ar xustf, uf ime ftq eqmeaz ar pmdwzqee, uf ime ftq ebduzs ar tabq, uf ime ftq iuzfqd ar pqebmud.";
	string menu = "\n1 - decode the default string.\n2 - decode a given string.\nanything else - exit.\n";
	float defaultfrequencies[26], expectedfrequencies[26], normalizedfrequencies[26], frequencies[26];
	ReadDistribution(defaultfrequencies, "distribution.txt");
	ExpectedDistribution(expectedfrequencies, "expecteddistribution.txt");
	NormalizedFrequency(expectedfrequencies, defaultstring.length(), normalizedfrequencies);
	int opt = 1;
	while (opt != 0) {
		cout << menu;
		cin >> opt;
		if (opt == 1) {
			//decode the given string
			string newstring = defaultstring;
			BreakCaesarCypher(newstring, defaultfrequencies, expectedfrequencies);
			cout << newstring << "\n";
		}
		else
			if (opt == 2) {
				//decode a string given by the user
				string newstring;
				cout << "input string:\n";
				cin.ignore();
				getline(cin, newstring);
				bool longtext = ComputeFreq(newstring, frequencies);
				if (longtext)
				{
					BreakCaesarCypher(newstring, frequencies, expectedfrequencies);
					cout << "\n" << newstring << "\n";
				}
				else {
					bool text = BreakShortText(newstring, root);
					if (text)
						cout << "\n" << newstring << "\n";
					else
						cout << "\nUnable to decode text.\n";
				}
			}
			else
				opt = 0;
	}
}