#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class CaesarCipher {
public:
    // Constructor to initialize the encrypted text
    CaesarCipher(const string& text) : encryptedText(text) {}

    // Function to decrypt text using a Caesar shift
    string decrypt(int shift) const {
        string result;
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

    // Try all possible shifts and rank based on dictionary matches
    void suggestDecryptions(const unordered_set<string>& dictionary, int topN = 5) const {
        vector<DecryptionResult> results;

        for (int shift = 0; shift < 26; ++shift) {
            string decrypted = decrypt(shift);
            int matchCount = countMatches(decrypted, dictionary);
            double avgWordLength = calculateAverageWordLength(decrypted);
            int commonWordScore = scoreCommonWords(decrypted);

            // Create a weighted score based on dictionary matches and common word score
            double score = matchCount * 5 + commonWordScore * 3 + avgWordLength * 1.5;

            results.push_back({shift, decrypted, matchCount, avgWordLength, commonWordScore, score});
        }

        // Sort results by weighted score (descending)
        sort(results.begin(), results.end(), [](const DecryptionResult& a, const DecryptionResult& b) {
            return a.score > b.score;
        });

        // Display top N results
        cout << "\n=== Suggested Decryptions ===\n";
        for (int i = 0; i < min(topN, (int)results.size()); ++i) {
            cout << "Shift: " << results[i].shift 
                 << "\nMatches: " << results[i].matchCount
                 << "\nAvg. Word Length: " << results[i].avgWordLength
                 << "\nCommon Word Score: " << results[i].commonWordScore
                 << "\nScore: " << results[i].score
                 << "\nDecrypted: " << results[i].decryptedText << "\n\n";
        }

        // Let user select a specific decryption to display fully
        int selectedShift;
        cout << "Enter the Caesar shift to display full decryption (-1 to skip): ";
        cin >> selectedShift;
        if (selectedShift >= 0 && selectedShift < 26) {
            cout << "\n=== Full Decryption (Shift " << selectedShift << ") ===\n";
            cout << decrypt(selectedShift) << "\n\n";
        }
    }

private:
    string encryptedText;

    // Struct to store decryption details
    struct DecryptionResult {
        int shift;
        string decryptedText;
        int matchCount;
        double avgWordLength;
        int commonWordScore;
        double score;
    };

    // Remove punctuation from a word
    string cleanWord(const string& word) const {
        string result;
        for (char c : word) {
            if (isalpha(c)) {
                result += tolower(c);
            }
        }
        return result;
    }

    // Count recognized dictionary matches in the decrypted text
    int countMatches(const string& text, const unordered_set<string>& dictionary) const {
        istringstream iss(text);
        string word;
        int count = 0;

        while (iss >> word) {
            word = cleanWord(word);
            if (dictionary.find(word) != dictionary.end()) {
                count++;
            }
        }
        return count;
    }

    // Calculate the average word length in the decrypted text
    double calculateAverageWordLength(const string& text) const {
        istringstream iss(text);
        string word;
        int totalLength = 0;
        int wordCount = 0;

        while (iss >> word) {
            word = cleanWord(word);
            if (!word.empty()) {
                totalLength += word.size();
                wordCount++;
            }
        }

        return (wordCount == 0) ? 0 : (double)totalLength / wordCount;
    }

    // Score based on presence of common English words
    int scoreCommonWords(const string& text) const {
        static const unordered_set<string> commonWords = {
            "the", "be", "to", "of", "and", "a", "in", "that", "have", "i", 
            "it", "for", "not", "on", "with", "he", "as", "you", "do", "at"
        };

        istringstream iss(text);
        string word;
        int score = 0;

        while (iss >> word) {
            word = cleanWord(word);
            if (commonWords.find(word) != commonWords.end()) {
                score++;
            }
        }
        return score;
    }
};

int main() {
    string input;
    cout << "Enter the encrypted text: ";
    getline(cin, input);

    // Load dictionary from file
    unordered_set<string> dictionary;
    ifstream file("dictionary.txt");
    if (file.is_open()) {
        string word;
        while (file >> word) {
            dictionary.insert(word);
        }
        file.close();
    } else {
        cerr << "Error: Could not open dictionary file.\n";
        return 1;
    }

    CaesarCipher cipher(input);

    // Suggest the most likely decryptions based on dictionary matches and common word frequency
    cipher.suggestDecryptions(dictionary);

    return 0;
}
