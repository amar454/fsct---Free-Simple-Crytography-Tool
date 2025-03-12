#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include <string>
#include <vector>
#include <set>
#include "../../include/dictionary/dictionary.hpp"



class Playfair {
public:
    // Constructor
    Playfair(const std::string& text, Dictionary* dict, const std::string& key);

    // Encrypt text using Playfair cipher
    std::string encrypt() const;

    // Decrypt text using Playfair cipher
    std::string decrypt() const;

    // Suggest decryptions based on dictionary matches
    void suggestDecryptions(int topN, const std::string& analysisMode) const;
    // Struct to hold results for decryption suggestions
struct DecryptionResult {
    int shift;
    std::string decryptedText;
    int matchCount;
    double avgWordLength;
    int commonWordScore;
    double score;
};
    // Display the top N decryption suggestions
    void displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const;

private:
    std::string encryptedText;  // Text to encrypt or decrypt
    Dictionary* dictionary;     // Pointer to dictionary for matching
    std::string key;            // Playfair cipher key
    char keyMatrix[5][5];       // Key matrix for Playfair cipher
    
    // Generate the key matrix for the Playfair cipher
    void generateKeyMatrix();

    // Clean the key by removing duplicates and non-alphabet characters
    std::string cleanKey(const std::string& inputKey) const;

    // Prepare text by removing spaces and ensuring it has even length
    std::string prepareTextForCipher(const std::string& inputText) const;

    // Find the position of a character in the key matrix
    void findPosition(char c, int& row, int& col) const;
};

#endif // PLAYFAIR_HPP
