#include<string>
#include<fstream>
#include<string.h>
using namespace std;
#define N 128
struct TrieNode {
	char data;
	TrieNode* children[N];
	bool isword;
};
inline TrieNode* CreateTrieNode(char data,bool isword) {
	TrieNode* node = new TrieNode();
	for (int i = 0; i < N; i++) {
		node->children[i] = NULL;
	}
	node->isword = isword;
	node->data = data;
	return node;
}
inline void InsertWord(string word, TrieNode* root) {
	//inserts a word into the trie
	//we assume the word has only lowercase letters
	TrieNode* current = root;
	for (int i = 0; i < word.length(); i++) {
		word[i] = tolower(word[i]);
		int letter = (int)word[i];
		if (current->children[letter] == NULL) {
			current->children[letter] = CreateTrieNode(word[i], i == word.length() - 1);
		}
		else {
			if (i == word.length() - 1) {
				current->children[letter]->isword = true;
			}
		}
		current = current->children[letter];
	}
}
inline bool SearchWord(string word, TrieNode* root) {
	TrieNode* current = root;
	for (int i = 0; i < word.length(); i++) {
		word[i] = tolower(word[i]);
		int letter = (int)word[i];
		if (current->children[letter] == NULL) {
			return false;
		}
		else {
			current = current->children[letter];
			if (i == word.length() - 1 && current->isword == true) {
				return true;
			}
		}
	}
	return false;
}
inline void CreateTrie(TrieNode* root, string inputfile) {
	//creates a trie with words
	ifstream words(inputfile);
	string line;
	while (getline(words, line)) {
		InsertWord(line, root);
	}
}
inline bool IsText(string text, TrieNode* root) {
	int i = 0;
	while (i < text.length()) {
		if (i == 0 || text[i - 1] == ' ') {
			//a word begins
			int j = i;
			string s = "";
			while (j < text.length() && isalpha(text[j])) {
				s += text[j];
				j++;
			}
			i = j;
			if (!SearchWord(s, root)) {
				return false;
			}
		}
		else
			i++;
	}
	return true;
}