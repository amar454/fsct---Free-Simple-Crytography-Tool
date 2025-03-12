#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include <string>
#include <vector>
#include "../../include/dictionary/dictionary.hpp"

struct DecryptionResult {
    int shift;
    std::string decryptedText;
    int matchCount;
    double avgWordLength;
    int commonWordScore;
    double score;
};

class Vigenere {
public:
    Vigenere(const std::string& text, Dictionary* dict, const std::string& key);

    // Encrypt text using Vigenère cipher with a key
    std::string encrypt() const;

    // Decrypt text using Vigenère cipher with a key
    std::string decrypt() const;

    // Suggest encryptions or decryptions based on dictionary matches
    void suggestDecryptions(int topN, const std::string& analysisMode, bool encryptMode = false) const;

private:
    std::string encryptedText;
    Dictionary* dictionary;
    std::string key;


    // Display the top N results
    void displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const;
};

#endif // VIGENERE_HPP
