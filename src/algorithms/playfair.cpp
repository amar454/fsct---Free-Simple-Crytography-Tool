#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "lib/dictionary.hpp"

class PlayfairCipher {
public:
    PlayfairCipher(Dictionary& dict) : dictionary(dict) {
        // Initialize Playfair square (5x5 grid)
        keySquare = {
            {'A', 'B', 'C', 'D', 'E'},
            {'F', 'G', 'H', 'I', 'K'},  // Note: 'J' is omitted as per Playfair rules
            {'L', 'M', 'N', 'O', 'P'},
            {'Q', 'R', 'S', 'T', 'U'},
            {'V', 'W', 'X', 'Y', 'Z'}
        };
    }

    // Main interface for the cipher
    void processMessage(const std::string& message) {
        std::string formattedMessage = formatMessage(message);
        std::vector<std::string> encryptedText = encrypt(formattedMessage);
        std::vector<std::string> decryptedText = decrypt(encryptedText);

        displayResults(formattedMessage, encryptedText, decryptedText);
    }

    void suggestWordsForPrefix(const std::string& prefix) {
        std::vector<std::string> suggestions = dictionary.suggestByPrefix(prefix);
        displaySuggestions(prefix, suggestions);
    }

private:
    Dictionary& dictionary;  // Reference to the Dictionary object
    std::vector<std::vector<char>> keySquare;

    // Helper function to find the position of a character in the Playfair square
    std::pair<int, int> findPosition(char c) {
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (keySquare[row][col] == c) {
                    return {row, col};
                }
            }
        }
        return {-1, -1}; // Should never happen if the input is valid
    }

    std::string formatMessage(const std::string& message) {
        std::string formattedMessage;
        std::string preprocessedMessage = dictionary.cleanWord(message);

        // Insert an extra 'X' if there are consecutive repeating characters
        for (size_t i = 0; i < preprocessedMessage.length(); i++) {
            if (i > 0 && preprocessedMessage[i] == preprocessedMessage[i - 1]) {
                formattedMessage += 'X';
            }
            formattedMessage += preprocessedMessage[i];
        }

        // If message has an odd length, append an 'X' at the end
        if (formattedMessage.length() % 2 != 0) {
            formattedMessage += 'X';
        }

        return formattedMessage;
    }

    std::vector<std::string> encrypt(const std::string& message) {
        std::vector<std::string> encryptedTexts;
        for (size_t i = 0; i < message.length(); i += 2) {
            char firstChar = message[i];
            char secondChar = message[i + 1];
            encryptedTexts.push_back(encryptPair(firstChar, secondChar));
        }
        return encryptedTexts;
    }

    std::vector<std::string> decrypt(const std::vector<std::string>& encryptedMessage) {
        std::vector<std::string> decryptedTexts;
        for (const auto& encryptedPair : encryptedMessage) {
            decryptedTexts.push_back(decryptPair(encryptedPair[0], encryptedPair[1]));
        }

        // Filter results based on dictionary validity
        std::vector<std::string> validDecryptedTexts;
        for (const auto& decrypted : decryptedTexts) {
            if (dictionary.isInDictionary(decrypted)) {
                validDecryptedTexts.push_back(decrypted);
            }
        }
        return validDecryptedTexts;
    }

    std::string encryptPair(char firstChar, char secondChar) {
        auto [row1, col1] = findPosition(firstChar);
        auto [row2, col2] = findPosition(secondChar);

        if (row1 == row2) {
            col1 = (col1 + 1) % 5;
            col2 = (col2 + 1) % 5;
        } else if (col1 == col2) {
            row1 = (row1 + 1) % 5;
            row2 = (row2 + 1) % 5;
        } else {
            std::swap(col1, col2);
        }

        return {keySquare[row1][col1], keySquare[row2][col2]};
    }

    std::string decryptPair(char firstChar, char secondChar) {
        auto [row1, col1] = findPosition(firstChar);
        auto [row2, col2] = findPosition(secondChar);

        if (row1 == row2) {
            col1 = (col1 - 1 + 5) % 5;
            col2 = (col2 - 1 + 5) % 5;
        } else if (col1 == col2) {
            row1 = (row1 - 1 + 5) % 5;
            row2 = (row2 - 1 + 5) % 5;
        } else {
            std::swap(col1, col2);
        }

        return {keySquare[row1][col1], keySquare[row2][col2]};
    }

    // Helper function to display encrypted, decrypted results
    void displayResults(const std::string& originalMessage, const std::vector<std::string>& encryptedText, const std::vector<std::string>& decryptedText) {
        std::cout << "Original Message: " << originalMessage << "\n";
        std::cout << "Encrypted: ";
        for (const auto& encrypted : encryptedText) {
            std::cout << encrypted << " ";
        }
        std::cout << "\n";

        std::cout << "Decrypted possibilities: \n";
        for (const auto& decrypted : decryptedText) {
            std::cout << decrypted << " ";
        }
        std::cout << "\n";
    }

    // Helper function to display word suggestions
    void displaySuggestions(const std::string& prefix, const std::vector<std::string>& suggestions) {
        std::cout << "Suggested words for prefix '" << prefix << "': \n";
        for (const auto& suggestion : suggestions) {
            std::cout << suggestion << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    // Initialize Dictionary object and load a dictionary (can be from file or set manually)
    Dictionary dictionary;
    dictionary.addWord("HELLO");
    dictionary.addWord("WORLD");
    dictionary.addWord("TEST");

    // Initialize PlayfairCipher with the Dictionary object
    PlayfairCipher cipher(dictionary);

    // Process a message: Encryption and Decryption
    std::string message = "HELLO WORLD";
    cipher.processMessage(message);

    // Suggest words based on a prefix
    std::string prefix = "TE";
    cipher.suggestWordsForPrefix(prefix);

    return 0;
}
