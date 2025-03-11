#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include "lib/dictionary.hpp"
#include <string>

class TranspositionCipher {
public:
    // Constructor
    TranspositionCipher(const std::string& text, Dictionary* dict);

    // Encryption method
    std::string encrypt(int key) const;

    // Decryption method
    std::string decrypt(int key) const;

    // Check if a word is valid (exists in the dictionary)
    bool isValidWord(const std::string& word) const;

    // Validate the plaintext by checking dictionary validity of each word
    void validateText() const;

    // Set the plaintext for the cipher
    void setPlaintext(const std::string& text);

    // Get the current plaintext
    std::string getPlaintext() const;

    // Set the cipher key
    void setKey(int newKey);

    // Get the current cipher key
    int getKey() const;

private:
    // Helper function to remove leading and trailing spaces from text
    void trimSpaces(std::string& text) const;

    // Helper function to print encrypted and decrypted text
    void printEncryptedAndDecryptedText(const std::string& encrypted, const std::string& decrypted) const;

    std::string plaintext;   // Plaintext to encrypt or decrypt
    Dictionary* dictionary;  // Pointer to a Dictionary instance
    int key;                 // Cipher key
};

#endif // TRANSPOSITION_H
