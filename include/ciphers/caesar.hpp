#ifndef CAESAR_HPP
#define CAESAR_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>
#include "../dictionary/dictionary.hpp"


class Caesar {
public:
    // Constructor
    Caesar(const std::string& text,Dictionary* dict);

    // Method to decrypt text using a Caesar shift
    std::string decrypt(int shift) const;

    // Method to encrypt text using a Caesar shift
    std::string encrypt(int shift) const;


    // Suggest decryptions based on dictionary matches
    void suggestDecryptions(int topN, const std::string& analysisMode) const;

private:
    std::string encryptedText;
    Dictionary* dictionary;
    // Helper struct for storing decryption results
    struct DecryptionResult {
        int shift;
        std::string decryptedText;
        int matchCount;
        double avgWordLength;
        int commonWordScore;
        double score;
    };

    void displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const;
};

#endif // CAESAR_HPP
