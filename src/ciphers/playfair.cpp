#include "../../include/ciphers/playfair.hpp"
#include "../../include/dictionary/dictionary.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

Playfair::Playfair(const std::string& text, Dictionary* dict, const std::string& key)
   : encryptedText(text), dictionary(dict), key(key) {
    generateKeyMatrix();
}

// Generates the key matrix for the Playfair cipher
void Playfair::generateKeyMatrix() {
    std::string adjustedKey = cleanKey(key);
    std::set<char> usedChars;

    // Build the key matrix with a 5x5 grid
    int k = 0;
    for (char c : adjustedKey) {
        if (usedChars.find(c) == usedChars.end()) {
            keyMatrix[k / 5][k % 5] = c;
            usedChars.insert(c);
            k++;
        }
    }

    // Fill the matrix with the remaining letters of the alphabet
    for (char c = 'a'; c <= 'z'; ++c) {
        if (c != 'j' && usedChars.find(c) == usedChars.end()) {
            keyMatrix[k / 5][k % 5] = c;
            k++;
        }
    }
}

// Clean the key by removing duplicates and non-alphabet characters
std::string Playfair::cleanKey(const std::string& inputKey) const {
    std::string cleaned;
    for (char c : inputKey) {
        if (isalpha(c)) {
            cleaned += std::tolower(c);
        }
    }
    return cleaned;
}

// Encrypt text using Playfair cipher
std::string Playfair::encrypt() const {
    std::string result;
    std::string text = prepareTextForCipher(encryptedText);

    for (size_t i = 0; i < text.size(); i += 2) {
        char first = text[i];
        char second = text[i + 1];

        int row1, col1, row2, col2;
        findPosition(first, row1, col1);
        findPosition(second, row2, col2);

        if (col1 == col2) {
            result += keyMatrix[(row1 + 1) % 5][col1];
            result += keyMatrix[(row2 + 1) % 5][col2];
        } else if (row1 == row2) {
            result += keyMatrix[row1][(col1 + 1) % 5];
            result += keyMatrix[row2][(col2 + 1) % 5];
        } else {
            result += keyMatrix[row1][col2];
            result += keyMatrix[row2][col1];
        }
    }

    return result;
}

// Decrypt text using Playfair cipher
std::string Playfair::decrypt() const {
    std::string result;
    std::string text = prepareTextForCipher(encryptedText);

    for (size_t i = 0; i < text.size(); i += 2) {
        char first = text[i];
        char second = text[i + 1];

        int row1, col1, row2, col2;
        findPosition(first, row1, col1);
        findPosition(second, row2, col2);

        if (col1 == col2) {
            result += keyMatrix[(row1 + 4) % 5][col1];
            result += keyMatrix[(row2 + 4) % 5][col2];
        } else if (row1 == row2) {
            result += keyMatrix[row1][(col1 + 4) % 5];
            result += keyMatrix[row2][(col2 + 4) % 5];
        } else {
            result += keyMatrix[row1][col2];
            result += keyMatrix[row2][col1];
        }
    }

    return result;
}

// Prepares text by removing spaces and ensuring it has an even length (for Playfair cipher)
std::string Playfair::prepareTextForCipher(const std::string& inputText) const {
    std::string cleanedText;
    for (char c : inputText) {
        if (isalpha(c)) {
            cleanedText += std::tolower(c);
        }
    }

    // Ensure the length is even by adding an 'x' if necessary
    if (cleanedText.size() % 2 != 0) {
        cleanedText += 'x';
    }

    return cleanedText;
}

// Finds the position of a character in the key matrix
void Playfair::findPosition(char c, int& row, int& col) const {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (keyMatrix[i][j] == c) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Suggest decryptions based on dictionary matches
void Playfair::suggestDecryptions(int topN, const std::string& analysisMode) const {
    std::vector<DecryptionResult> results;

    for (int shift = 0; shift < 26; ++shift) {
        std::string decrypted = decrypt();
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
void Playfair::displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const {
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
