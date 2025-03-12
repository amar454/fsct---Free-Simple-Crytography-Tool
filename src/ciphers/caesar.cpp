#include "../../include/ciphers/caesar.hpp"
#include "../../include/dictionary/dictionary.hpp"
#include <cctype>
#include <sstream>
#include <algorithm>

Caesar::Caesar(const std::string& text, Dictionary* dict)
   : encryptedText(text), dictionary(dict) {
}


// Decrypt text using Caesar cipher shift
std::string Caesar::decrypt(int shift) const {
    std::string result;
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += (c - base - shift + 26) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

// encrypt text using Caesar cipher shift
std::string Caesar::encrypt(int shift) const {
    std::string result;
    for (char c : encryptedText) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += (c - base + shift) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

// Suggest decryptions based on dictionary matches
void Caesar::suggestDecryptions(int topN, const std::string& analysisMode) const {
    std::vector<DecryptionResult> results;

    for (int shift = 0; shift < 26; ++shift) {
        std::string decrypted = decrypt(shift);
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
void Caesar::displayResults(const std::vector<DecryptionResult>& results, int topN, const std::string& analysisMode) const {
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

