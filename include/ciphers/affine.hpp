#ifndef AFFINE_HPP
#define AFFINE_HPP

#include <string>
#include <vector>
#include "../../include/dictionary/dictionary.hpp"

class Affine {
public:
    Affine(const std::string& text, Dictionary* dict, int a, int b);

    // Encrypt text using Affine cipher with a key
    std::string encrypt() const;

    // Decrypt text using Affine cipher with a key
    std::string decrypt() const;

    // Suggest encryptions or decryptions based on dictionary matches
    void suggestDecryptions(int topN, const std::string& analysisMode, bool encryptMode = false) const;

private:
    std::string encryptedText;
    Dictionary* dictionary;
    int a; // Multiplier for Affine cipher
    int b; // Shift for Affine cipher

    // Private DecryptionResult struct similar to the Playfair example
    struct DecryptionResult {
        int shift;
        std::string decryptedText;
        int matchCount;
        double avgWordLength;
        int commonWordScore;
        double score;
    };

    // Helper function to find modular inverse of a number
    int modInverse(int a, int m) const;

    // Helper function to evaluate decryption result
    DecryptionResult evaluateDecryption(const std::string& decryptedText) const;

    // Display the top N results
    void displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const;
};

#endif // AFFINE_HPP
