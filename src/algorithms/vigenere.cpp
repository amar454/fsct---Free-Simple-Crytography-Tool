#include "algorithms/vigenere.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <cmath>

/**
 * @brief Constructor to initialize the ciphertext and dictionary.
 * @param cipherText The ciphertext to be decrypted.
 * @param dictionary The dictionary used for word validation.
 */
VigenereCipher::VigenereCipher(const std::string& cipherText, Dictionary& dictionary)
    : cipherText(cipherText), dictionary(dictionary) {}

/**
 * @brief Decrypt the ciphertext using the specified key.
 * @param key The key to use for decryption.
 * @return The decrypted text.
 */
std::string VigenereCipher::decrypt(const std::string& key) {
    std::string decryptedText;
    int keyIndex = 0;

    for (char ch : cipherText) {
        if (std::isalpha(ch)) {
            char decryptedChar;
            if (std::isupper(ch)) {
                decryptedChar = (ch - 'A' - (key[keyIndex % key.size()] - 'A') + 26) % 26 + 'A';
            } else {
                decryptedChar = (ch - 'a' - (key[keyIndex % key.size()] - 'a') + 26) % 26 + 'a';
            }
            decryptedText += decryptedChar;
            keyIndex++;
        } else {
            decryptedText += ch;
        }
    }

    return decryptedText;
}

/**
 * @brief Attempt to decrypt using all possible keys up to the specified length.
 * @param maxKeyLength The maximum length of the key to try.
 */
void VigenereCipher::tryAllKeys(int maxKeyLength) {
    std::vector<std::pair<int, std::string>> results;

    for (int length = 1; length <= maxKeyLength; ++length) {
        std::string key(length, 'A');

        while (true) {
            std::string decrypted = decrypt(key);
            int matchCount = countMatches(decrypted);
            double score = evaluateDecryption(decrypted, matchCount);
            results.push_back({matchCount, decrypted, score});

            if (!incrementKey(key)) {
                break;
            }
        }
    }

    // Sort by score (highest first), considering match count and contextual analysis
    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.score > b.score;
    });

    // Display the best matches
    for (const auto& result : results) {
        std::cout << "Matches: " << result.first << "\nDecrypted Text: " << sanitizeResult(result.second) << "\n\n";
    }
}

/**
 * @brief Count the number of valid dictionary words in the decrypted text.
 * @param decryptedText The decrypted text.
 * @return The number of valid dictionary words found.
 */
int VigenereCipher::countMatches(const std::string& decryptedText) {
    std::istringstream iss(decryptedText);
    std::string word;
    int matchCount = 0;

    while (iss >> word) {
        word = removePunctuation(word);
        if (dictionary.isInDictionary(word)) {
            matchCount++;
        }
    }

    return matchCount;
}

/**
 * @brief Evaluate the decryption based on frequency of common English words, structure, etc.
 * @param decryptedText The decrypted text.
 * @param matchCount The number of matching words in the decrypted text.
 * @return A score to rank the decryption.
 */
double VigenereCipher::evaluateDecryption(const std::string& decryptedText, int matchCount) {
    // Score based on word frequency in decrypted text
    double wordFrequencyScore = calculateWordFrequencyScore(decryptedText);

    // Combine the match count and word frequency score
    return matchCount * 0.6 + wordFrequencyScore * 0.4; // Adjust weights based on testing
}

/**
 * @brief Calculate a score based on the frequency of common words in the decrypted text.
 * @param decryptedText The decrypted text.
 * @return A score based on word frequency.
 */
double VigenereCipher::calculateWordFrequencyScore(const std::string& decryptedText) {
    static const std::unordered_map<std::string, double> wordFrequencies = {
        {"the", 0.075}, {"of", 0.038}, {"and", 0.028}, {"to", 0.022}, {"a", 0.022},
        {"in", 0.017}, {"that", 0.015}, {"is", 0.014}, {"it", 0.013}, {"for", 0.012}
    };

    std::istringstream iss(decryptedText);
    std::string word;
    double score = 0.0;

    while (iss >> word) {
        word = removePunctuation(word);
        auto it = wordFrequencies.find(word);
        if (it != wordFrequencies.end()) {
            score += it->second;
        }
    }

    return score;
}

/**
 * @brief Remove punctuation from a word.
 * @param word The word to clean.
 * @return The cleaned word without punctuation.
 */
std::string VigenereCipher::removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (std::isalpha(ch)) {
            result += ch;
        }
    }
    return result;
}

/**
 * @brief Increment the key in lexicographical order.
 * @param key The key to increment.
 * @return True if incrementing is successful, false if the key wraps around.
 */
bool VigenereCipher::incrementKey(std::string& key) {
    for (int i = key.size() - 1; i >= 0; --i) {
        if (key[i] < 'Z') {
            key[i]++;
            return true;
        } else {
            key[i] = 'A';
        }
    }
    return false;
}

/**
 * @brief Sanitize the decrypted text by removing any potential cipher analysis clues.
 * @param decryptedText The decrypted text to sanitize.
 * @return A sanitized version of the decrypted text.
 */
std::string VigenereCipher::sanitizeResult(const std::string& decryptedText) {
    std::string result = decryptedText;

    // Basic sanitization: remove any suspicious non-alphabetic characters
    result = std::regex_replace(result, std::regex("[^a-zA-Z ]"), "");

    // Optionally, add obfuscation or normalization if needed
    // For example: randomize the order of words, adjust character frequencies, etc.

    return result;
}
