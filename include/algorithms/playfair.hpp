#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include <string>
#include <vector>
#include "lib/dictionary.hpp"

class PlayfairCipher {
public:
    // Constructor that takes a reference to a Dictionary object
    PlayfairCipher(Dictionary& dict);

    // Public method to process a message: Encryption and Decryption
    void processMessage(const std::string& message);

    // Public method to suggest words based on a prefix
    void suggestWordsForPrefix(const std::string& prefix);

private:
    Dictionary& dictionary;  // Reference to the Dictionary object
    std::vector<std::vector<char>> keySquare;  // The 5x5 Playfair cipher square

    // Helper function to find the position of a character in the Playfair square
    std::pair<int, int> findPosition(char c);

    // Helper function to format the message for Playfair cipher (handling X for repeating letters and odd-length messages)
    std::string formatMessage(const std::string& message);

    // Encrypt the message in pairs
    std::vector<std::string> encrypt(const std::string& message);

    // Decrypt the message in pairs and filter valid dictionary words
    std::vector<std::string> decrypt(const std::vector<std::string>& encryptedMessage);

    // Encrypt a single pair of characters
    std::string encryptPair(char firstChar, char secondChar);

    // Decrypt a single pair of characters
    std::string decryptPair(char firstChar, char secondChar);

    // Helper function to display the results: Encrypted and Decrypted text
    void displayResults(const std::string& originalMessage, const std::vector<std::string>& encryptedText, const std::vector<std::string>& decryptedText);

    // Helper function to display word suggestions for a prefix
    void displaySuggestions(const std::string& prefix, const std::vector<std::string>& suggestions);
};

#endif // PLAYFAIR_CIPHER_HPP
