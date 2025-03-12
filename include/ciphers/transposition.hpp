#ifndef TRANSPOSITION_HPP
#define TRANSPOSITION_HPP

#include <string>
#include "../../include/dictionary/dictionary.hpp"

// Class to handle transposition cipher encryption and decryption
class Transposition {
public:
    // Constructor
    Transposition(const std::string& text, Dictionary* dict);

    // Encrypt text using transposition cipher
    std::string encrypt(int key) const;

    // Decrypt text using transposition cipher
    std::string decrypt(int key) const;

    // Validate if a word exists in the dictionary
    bool isValidWord(const std::string& word) const;

    // Validate the plaintext and check words against the dictionary
    void validateText() const;

    // Setters and getters for plaintext and key
    void setPlaintext(const std::string& text);
    std::string getPlaintext() const;
    int getKey() const;
    void setKey(int newKey);
    void suggestDecryptions(int topN, const std::string& analysisMode) const;
    struct DecryptionResult {
        int shift;
        std::string decryptedText;
        int matchCount;
        double avgWordLength;
        int commonWordScore;
        double score;
    };
private:
    
    std::string plaintext;  // Text to encrypt or decrypt
    Dictionary* dictionary; // Dictionary for word validation
    int key;                // Key for transposition cipher
    // Private DecryptionResult struct similar to the Playfair example
    
    // Trim leading and trailing spaces from the text
    void trimSpaces(std::string& text) const;
    void displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const;
    // Print encrypted and decrypted text
    void printEncryptedAndDecryptedText(const std::string& encrypted, const std::string& decrypted) const;
};

#endif // TRANSPOSITION_HPP
