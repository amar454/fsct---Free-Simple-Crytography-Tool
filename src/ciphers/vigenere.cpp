#include "../../include/ciphers/vigenere.hpp"
#include "../../include/dictionary/dictionary.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>

Vigenere::Vigenere(const std::string& text, Dictionary* dict, const std::string& key)
    : encryptedText(text), dictionary(dict), key(key) {
}

// Encrypt text using Vigenère cipher with a key
std::string Vigenere::encrypt() const {
    std::string result;
    int keyIndex = 0;
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char keyChar = tolower(key[keyIndex % key.length()]);
            result += (c - base + (keyChar - 'a')) % 26 + base;
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}

// Decrypt text using Vigenère cipher with a key
std::string Vigenere::decrypt() const {
    std::string result;
    int keyIndex = 0;
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char keyChar = tolower(key[keyIndex % key.length()]);
            result += (c - base - (keyChar - 'a') + 26) % 26 + base;
            keyIndex++;
        } else {
            result += c;
        }
    }
    return result;
}



// Suggest encryptions or decryptions based on dictionary matches
void Vigenere::suggestDecryptions(int topN, const std::string& analysisMode, bool encryptMode) const {
    std::vector<DecryptionResult> results;

    for (int shift = 0; shift < 26; ++shift) {
        std::string resultText = encryptMode ? encrypt() : decrypt();
        int matchCount = dictionary->countMatches(resultText);

        double score = matchCount * 5; // Basic score based on dictionary matches
        double avgWordLength = 0;
        int commonWordScore = 0;

        if (analysisMode == "advanced") {
            avgWordLength = dictionary->calculateAverageWordLength(resultText);
            commonWordScore = dictionary->scoreCommonWords(resultText);
            score += commonWordScore * 3 + avgWordLength * 1.5; // Advanced scoring
        }

        results.push_back({shift, resultText, matchCount, avgWordLength, commonWordScore, score});
    }

    // Sort results by score (descending)
    std::sort(results.begin(), results.end(), [](const DecryptionResult& a, const DecryptionResult& b) {
        return a.score > b.score;
    });

    // Display results
    displayResults(results, topN, analysisMode);
}

// Display the top N results
void Vigenere::displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const {
    std::cout << "\n=== Suggested Decryptions (" << analysisMode << " mode) ===\n";

    for (int i = 0; i < std::min(topN, static_cast<int>(results.size())); ++i) {
        std::cout << "Shift: " << results[i].shift
                  << " | Matches: " << results[i].matchCount;

        if (analysisMode == "advanced") {
            std::cout << " | Avg. Word Length: " << results[i].avgWordLength
                      << " | Common Words: " << results[i].commonWordScore;
        }

        std::cout << "\nResult: " << results[i].decryptedText << "\n\n";
    }
}
