#include "../../include/ciphers/affine.hpp"
#include "../../include/dictionary/dictionary.hpp"
#include <cctype>
#include <sstream>
#include <algorithm>
#include <iostream>

Affine::Affine(const std::string& text, Dictionary* dict, int a, int b)
    : encryptedText(text), dictionary(dict), a(a), b(b) {
}

// Helper function to find modular inverse of a number
int Affine::modInverse(int a, int m) const {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // If no modular inverse exists
}

// Encrypt text using Affine cipher with a key
std::string Affine::encrypt() const {
    std::string result;
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += (a * (c - base) + b) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

// Decrypt text using Affine cipher with a key
std::string Affine::decrypt() const {
    std::string result;
    int a_inv = modInverse(a, 26); // Modular inverse of 'a' under modulo 26
    if (a_inv == -1) {
        std::cerr << "Error: No modular inverse for 'a' exists. Cannot decrypt.\n";
        return "";
    }
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += (a_inv * (c - base - b + 26)) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}


// Suggest encryptions or decryptions based on dictionary matches
void Affine::suggestDecryptions(int topN, const std::string& analysisMode, bool encryptMode) const {
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
void Affine::displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const {
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
