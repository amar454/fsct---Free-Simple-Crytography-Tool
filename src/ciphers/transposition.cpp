#include "../../include/ciphers/transposition.hpp"
#include "../../include/dictionary/dictionary.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

Transposition::Transposition(const std::string& text, Dictionary* dict) 
    : plaintext(text), dictionary(dict) {}

std::string Transposition::encrypt(int key) const {
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

std::string Transposition::decrypt(int key) const {
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
    std::string encryptedText;
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

bool Transposition::isValidWord(const std::string& word) const {
    // Check if the word exists in the dictionary
    return dictionary->isInDictionary(word);
}

void Transposition::validateText() const {
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

void Transposition::setPlaintext(const std::string& text) {
    plaintext = text;
}

std::string Transposition::getPlaintext() const {
    return plaintext;
}

int Transposition::getKey() const {
    return key;
}

void Transposition::setKey(int newKey) {
    if (newKey <= 0) {
        throw std::invalid_argument("Key must be a positive integer.");
    }
    key = newKey;
}

void Transposition::trimSpaces(std::string& text) const {
    // Remove leading and trailing spaces from the string
    size_t start = text.find_first_not_of(" \t\n\r");
    size_t end = text.find_last_not_of(" \t\n\r");
    
    if (start == std::string::npos || end == std::string::npos) {
        text = "";
    } else {
        text = text.substr(start, end - start + 1);
    }
}

void Transposition::printEncryptedAndDecryptedText(const std::string& encrypted, const std::string& decrypted) const {
    std::cout << "Encrypted text:\n" << encrypted << std::endl;
    std::cout << "Decrypted text:\n" << decrypted << std::endl;
}
void Transposition::suggestDecryptions(int topN, const std::string& analysisMode) const {
    std::vector<DecryptionResult> results;

    for (int shift = 0; shift < 26; ++shift) {
        // Pass the key to the decrypt function
        std::string decrypted = decrypt(key); // Pass the key here
        int matchCount = dictionary->countMatches(decrypted);

        double score = matchCount * 5; // Basic score based on dictionary matches
        double avgWordLength = 0;
        int commonWordScore = 0;

        if (analysisMode == "advanced") {
            avgWordLength = dictionary->calculateAverageWordLength(decrypted);
            commonWordScore = dictionary->scoreCommonWords(decrypted);
            score += commonWordScore * 3 + avgWordLength * 1.5; // Advanced scoring
        }

        results.push_back({shift, decrypted, matchCount, avgWordLength, commonWordScore, score});
    }

    // Sort results by score (descending)
    std::sort(results.begin(), results.end(), [](const DecryptionResult& a, const DecryptionResult& b) {
        return a.score > b.score;
    });

    // Display results
    displayResults(results, topN, analysisMode);
}
// Display the top N results
void Transposition::displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const {
    std::cout << "\n=== Suggested Decryptions (" << analysisMode << " mode) ===\n";

    for (int i = 0; i < std::min(topN, static_cast<int>(results.size())); ++i) {
        std::cout << "Shift: " << results[i].shift 
                  << " | Matches: " << results[i].matchCount;

        if (analysisMode == "advanced") {
            std::cout << " | Avg. Word Length: " << results[i].avgWordLength
                      << " | Common Words: " << results[i].commonWordScore;
        }

        std::cout << "\nDecrypted: " << results[i].decryptedText << "\n\n";
    }
}
