#ifndef CAESAR_HPP
#define CAESAR_HPP

#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class CaesarCipher {
public:
    // Constructor to initialize the encrypted text
    CaesarCipher(const string& text);

    // Function to decrypt text using a Caesar shift
    string decrypt(int shift) const;

    // Try all possible shifts and rank based on dictionary matches
    void suggestDecryptions(const unordered_set<string>& dictionary, int topN = 5) const;

private:
    string encryptedText;

    // Struct to store decryption details
    struct DecryptionResult {
        int shift;
        string decryptedText;
        int matchCount;
        double avgWordLength;
        int commonWordScore;
        double score;
    };

    // Remove punctuation from a word
    string cleanWord(const string& word) const;

    // Count recognized dictionary matches in the decrypted text
    int countMatches(const string& text, const unordered_set<string>& dictionary) const;

    // Calculate the average word length in the decrypted text
    double calculateAverageWordLength(const string& text) const;

    // Score based on presence of common English words
    int scoreCommonWords(const string& text) const;
};

#endif // CAESAR_CIPHER_HPP
