#include "transposition.hpp"
#include "lib/dictionary.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

TranspositionCipher::TranspositionCipher(const std::string& text, Dictionary* dict) 
    : plaintext(text), dictionary(dict) {}

std::string TranspositionCipher::encrypt(int key) const {
    if (key <= 0) {
        throw std::invalid_argument("Key must be a positive integer.");
    }

    int len = plaintext.length();
    int rows = (len / key) + (len % key == 0 ? 0 : 1); // Determine the number of rows
    std::vector<std::string> grid(rows);

    int currentChar = 0;
    // Fill the grid with characters
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < key; ++c) {
            if (currentChar < len) {
                grid[r] += plaintext[currentChar++];
            } else {
                grid[r] += ' '; // Fill with spaces if the message is shorter than grid
            }
        }
    }

    // Read the grid column by column to get the encrypted text
    std::string encryptedText;
    for (int c = 0; c < key; ++c) {
        for (int r = 0; r < rows; ++r) {
            encryptedText += grid[r][c];
        }
    }

    // Optionally, trim any trailing spaces
    return encryptedText;
}

std::string TranspositionCipher::decrypt(int key) const {
    if (key <= 0) {
        throw std::invalid_argument("Key must be a positive integer.");
    }

    int len = plaintext.length();
    int rows = (len / key) + (len % key == 0 ? 0 : 1);
    int fullColumns = len % key;
    int numColumns = key;

    // Create grid for decryption
    std::vector<std::string> grid(rows);

    int currentChar = 0;
    // Fill the grid column by column with encrypted text
    for (int c = 0; c < numColumns; ++c) {
        for (int r = 0; r < rows; ++r) {
            if (currentChar < len) {
                grid[r] += encryptedText[currentChar++];
            }
        }
    }

    std::string decryptedText;
    // Combine the rows to form the decrypted message
    for (const auto& row : grid) {
        decryptedText += row;
    }

    // Optionally, remove trailing spaces and format the output
    return decryptedText;
}

bool TranspositionCipher::isValidWord(const std::string& word) const {
    // Check if the word exists in the dictionary
    return dictionary->isInDictionary(word);
}

void TranspositionCipher::validateText() const {
    std::istringstream stream(plaintext);
    std::string word;
    int validCount = 0;
    int totalCount = 0;
    
    while (stream >> word) {
        ++totalCount;
        if (isValidWord(word)) {
            ++validCount;
        }
    }

    // Display a summary of how many words are valid
    std::cout << "Out of " << totalCount << " words, " 
              << validCount << " are valid words from the dictionary." << std::endl;
}

void TranspositionCipher::setPlaintext(const std::string& text) {
    plaintext = text;
}

std::string TranspositionCipher::getPlaintext() const {
    return plaintext;
}

int TranspositionCipher::getKey() const {
    return key;
}

void TranspositionCipher::setKey(int newKey) {
    if (newKey <= 0) {
        throw std::invalid_argument("Key must be a positive integer.");
    }
    key = newKey;
}

void TranspositionCipher::trimSpaces(std::string& text) const {
    // Remove leading and trailing spaces from the string
    size_t start = text.find_first_not_of(" \t\n\r");
    size_t end = text.find_last_not_of(" \t\n\r");
    
    if (start == std::string::npos || end == std::string::npos) {
        text = "";
    } else {
        text = text.substr(start, end - start + 1);
    }
}

void TranspositionCipher::printEncryptedAndDecryptedText(const std::string& encrypted, const std::string& decrypted) const {
    std::cout << "Encrypted text:\n" << encrypted << std::endl;
    std::cout << "Decrypted text:\n" << decrypted << std::endl;
}

int main() {
    // Load dictionary
    Dictionary dictionary;
    if (!dictionary.loadFromFile("path_to_your_dictionary.txt")) {
        std::cerr << "Failed to load dictionary!" << std::endl;
        return 1;
    }

    std::string plaintext = "This is a test message for transposition cipher.";
    int key = 5;

    TranspositionCipher cipher(plaintext, &dictionary);
    
    // Validate plaintext words with dictionary
    cipher.validateText();

    // Encrypt and decrypt
    std::string encrypted = cipher.encrypt(key);
    cipher.printEncryptedAndDecryptedText(encrypted, "");

    std::string decrypted = cipher.decrypt(key);
    cipher.printEncryptedAndDecryptedText("", decrypted);

    return 0;
}
