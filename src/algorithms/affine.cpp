#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <cmath>
#include "../lib/dictionary.hpp"

using namespace std;

class AffineCipher {
public:
    AffineCipher(const string& cipherText) : cipherText(cipherText) {}

    // Function to decrypt using the Affine cipher formula
    string decrypt(int a, int b) {
        string decryptedText;
        int a_inv = modInverse(a, 26);
        if (a_inv == -1) return ""; // If no inverse exists, invalid key

        for (char ch : cipherText) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                int x = ch - base;
                int decryptedChar = (a_inv * (x - b + 26)) % 26;
                decryptedText += decryptedChar + base;
            } else {
                decryptedText += ch;
            }
        }
        return decryptedText;
    }

    // Encrypt the text using the Affine cipher formula
    string encrypt(int a, int b) {
        string encryptedText;
        for (char ch : cipherText) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                int x = ch - base;
                int encryptedChar = (a * x + b) % 26;
                encryptedText += encryptedChar + base;
            } else {
                encryptedText += ch;
            }
        }
        return encryptedText;
    }

    // Try all key combinations and find the most probable plaintexts
    void tryAllKeys(const Dictionary& dict) {
        vector<pair<int, string>> results;

        for (int a = 1; a < 26; ++a) {
            if (gcd(a, 26) != 1) continue; // `a` must be coprime with 26

            for (int b = 0; b < 26; ++b) {
                string decrypted = decrypt(a, b);
                if (decrypted.empty()) continue;

                int matchCount = countMatches(decrypted, dict);
                results.push_back({matchCount, decrypted});
            }
        }

        // Sort by number of dictionary matches (higher matches first)
        sort(results.begin(), results.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
            return a.first > b.first;
        });

        // Display top results
        cout << "\n=== Best Possible Decryptions ===\n";
        int displayed = 0;
        for (const auto& result : results) {
            if (displayed >= 5) break; // Limit to top 5 results
            cout << "[Matches: " << result.first << "]\n" << result.second << "\n\n";
            displayed++;
        }
    }

    // Perform a basic frequency analysis of the ciphertext to suggest possible 'a' and 'b' values
    void frequencyAnalysis() {
        unordered_map<char, int> frequencyMap;

        // Count the frequency of each letter
        for (char ch : cipherText) {
            if (isalpha(ch)) {
                frequencyMap[tolower(ch)]++;
            }
        }

        // Sort by frequency in descending order
        vector<pair<char, int>> sortedFreq(frequencyMap.begin(), frequencyMap.end());
        sort(sortedFreq.begin(), sortedFreq.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
            return a.second > b.second;
        });

        // Display the frequency of letters
        cout << "\n=== Frequency Analysis ===\n";
        for (const auto& entry : sortedFreq) {
            cout << entry.first << ": " << entry.second << "\n";
        }

        // Attempt a common approach for breaking ciphers by letter frequency
        // assuming the most frequent letter corresponds to 'E' in the English language
        if (sortedFreq.size() > 1) {
            char mostFrequentChar = sortedFreq[0].first;
            char expectedChar = 'e'; // Most common letter in English is 'e'
            int a = modInverse(3, 26); // Placeholder logic for breaking cipher
            cout << "Attempting to break with most frequent letter '" << mostFrequentChar
                 << "' as 'e'... [not yet implemented completely].\n";
        }
    }

private:
    string cipherText;

    // Compute the greatest common divisor (GCD)
    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Compute the modular multiplicative inverse using the Extended Euclidean Algorithm
    int modInverse(int a, int m) {
        int m0 = m, y = 0, x = 1;
        if (m == 1) return 0;

        while (a > 1) {
            int q = a / m;
            int t = m;

            m = a % m;
            a = t;
            t = y;

            y = x - q * y;
            x = t;
        }

        if (x < 0) x += m0;

        return x;
    }

    // Count how many words match the dictionary
    int countMatches(const string& decryptedText, const Dictionary& dict) {
        istringstream iss(decryptedText);
        string word;
        int matchCount = 0;

        while (iss >> word) {
            word = removePunctuation(word);
            if (dict.isInDictionary(word)) {
                matchCount++;
            }
        }
        return matchCount;
    }

    // Remove punctuation from words
    string removePunctuation(const string& word) {
        string result;
        for (char ch : word) {
            if (isalpha(ch)) {
                result += ch;
            }
        }
        return result;
    }
};

// Main function
int main() {
    string cipherText;
    string dictionaryFileName = "dictionary.txt";

    cout << "Enter the ciphertext: ";
    getline(cin, cipherText);

    // Load the dictionary
    Dictionary dict(dictionaryFileName);
    if (dict.size() == 0) {
        cerr << "Failed to load dictionary from " << dictionaryFileName << endl;
        return 1;
    }

    // Create an AffineCipher object
    AffineCipher cipher(cipherText);

    // Try all key combinations and suggest the best matches
    cipher.tryAllKeys(dict);

    // Perform frequency analysis
    cipher.frequencyAnalysis();

    return 0;
}
