#include "../../include/dictionary/dictionary.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>



Dictionary::Dictionary() {
    // Initially load predefined dictionary
    for (const auto& word : predefinedDictionary) {
        dictionary.insert(word);
    }
}

Dictionary::~Dictionary() {
    clearDictionary();
}

bool Dictionary::loadFromFile(const std::string& filename, char delimiter) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (std::getline(ss, word, delimiter)) {
            dictionary.insert(cleanWord(word));
        }
    }

    file.close();
    return true;
}


void Dictionary::addWord(const std::string& word) {
    dictionary.insert(cleanWord(word));
}

void Dictionary::removeWord(const std::string& word) {
    dictionary.erase(cleanWord(word));
}

void Dictionary::displayDictionary() const {
    for (const auto& word : dictionary) {
        std::cout << word << std::endl;
    }
}

void Dictionary::clearDictionary() {
    dictionary.clear();
}

bool Dictionary::isInDictionary(const std::string& word) const {
    return dictionary.find(cleanWord(word)) != dictionary.end();
}

// Helper function to clean words (e.g., lowercase, strip punctuation)
std::string Dictionary::cleanWord(const std::string& word) const {
    std::string cleaned;
    for (char c : word) {
        if (isalpha(c)) {
            cleaned += std::tolower(c);
        }
    }
    return cleaned;
}
std::vector<std::string> Dictionary::suggestByPrefix(const std::string& prefix) const {
    std::vector<std::string> suggestions;
    std::string cleanedPrefix = cleanWord(prefix);

    // Create a sorted vector from the unordered set
    std::vector<std::string> sorted(dictionary.begin(), dictionary.end());
    std::sort(sorted.begin(), sorted.end());

    auto it = std::lower_bound(sorted.begin(), sorted.end(), cleanedPrefix);

    while (it != sorted.end() && it->find(cleanedPrefix) == 0) {
        suggestions.push_back(*it);
        ++it;
    }

    return suggestions;
}


// 
std::unordered_set<std::string> Dictionary::extractWords(const std::string& text) const {
    std::unordered_set<std::string> words;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        words.insert(cleanWord(word));
    }
    return words;
}

std::string Dictionary::getMostFrequentWord(const std::string& text) const {
    auto frequency = getWordFrequency(text);
    std::string mostFrequent;
    int maxCount = 0;
    for (const auto& pair : frequency) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mostFrequent = pair.first;
        }
    }
    return mostFrequent;
}

std::unordered_map<std::string, int> Dictionary::getWordFrequency(const std::string& text) const {
    std::unordered_map<std::string, int> wordFrequency;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        word = cleanWord(word);
        if (isInDictionary(word)) {
            wordFrequency[word]++;
        }
    }
    return wordFrequency;
}

std::vector<std::string> Dictionary::getTopNFrequentWords(const std::string& text, int N) const {
    auto frequency = getWordFrequency(text);
    std::vector<std::pair<std::string, int>> sorted(frequency.begin(), frequency.end());
    
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<std::string> result;
    for (int i = 0; i < N && i < sorted.size(); ++i) {
        result.push_back(sorted[i].first);
    }
    return result;
}
int Dictionary::countMatches(const std::string& text) const {
    std::istringstream iss(text);
    std::string word;
    int matchCount = 0;
    while (iss >> word) {
        if (isInDictionary(cleanWord(word))) {
            matchCount++;
        }
    }
    return matchCount;
}
// Returns the number of words in the dictionary
int Dictionary::size() const {
    return dictionary.size();
}

// Returns the longest word in the dictionary
std::string Dictionary::getLongestWord() const {
    std::string longest;
    for (const auto& word : dictionary) {
        if (word.size() > longest.size()) {
            longest = word;
        }
    }
    return longest;
}

// Returns the shortest word in the dictionary
std::string Dictionary::getShortestWord() const {
    std::string shortest;
    if (!dictionary.empty()) {
        shortest = *dictionary.begin();
        for (const auto& word : dictionary) {
            if (word.size() < shortest.size()) {
                shortest = word;
            }
        }
    }
    return shortest;
}

// Returns true if a word is a palindrome
bool Dictionary::isPalindrome(const std::string& word) const {
    std::string clean = cleanWord(word);
    return std::equal(clean.begin(), clean.end(), clean.rbegin());
}

// Returns a list of palindromic words in the dictionary
std::vector<std::string> Dictionary::getPalindromicWords() const {
    std::vector<std::string> palindromes;
    for (const auto& word : dictionary) {
        if (isPalindrome(word)) {
            palindromes.push_back(word);
        }
    }
    return palindromes;
}

int Dictionary::levenshteinDistance(const std::string& a, const std::string& b) const {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({dp[i - 1][j - 1], dp[i][j - 1], dp[i - 1][j]}) + 1;
            }
        }
    }
    return dp[m][n];
}

std::vector<std::string> Dictionary::suggestCorrections(const std::string& word) const {
    std::vector<std::pair<int, std::string>> suggestions;
    for (const auto& dictWord : dictionary) {
        int dist = levenshteinDistance(word, dictWord);
        suggestions.emplace_back(dist, dictWord);
    }
    std::sort(suggestions.begin(), suggestions.end());
    
    std::vector<std::string> result;
    for (int i = 0; i < 5 && i < suggestions.size(); ++i) {
        result.push_back(suggestions[i].second);
    }
    return result;
}

// Finds anagrams of a word from the dictionary
std::vector<std::string> Dictionary::findAnagrams(const std::string& word) const {
    std::vector<std::string> anagrams;
    std::string sortedWord = word;
    std::sort(sortedWord.begin(), sortedWord.end());

    for (const auto& dictWord : dictionary) {
        std::string sortedDictWord = dictWord;
        std::sort(sortedDictWord.begin(), sortedDictWord.end());
        if (sortedDictWord == sortedWord) {
            anagrams.push_back(dictWord);
        }
    }
    return anagrams;
}


// Reverse suggestions
std::vector<std::string> Dictionary::suggestByReversal(const std::string& word) const {
    std::string reversed = word;
    std::reverse(reversed.begin(), reversed.end());
    return suggestCorrections(reversed);
}

// QUADGRAM FREQUENCY
std::unordered_map<std::string, int> Dictionary::quadgramFrequency(const std::string& text) const {
    std::unordered_map<std::string, int> frequency;
    int length = text.size();

    for (int i = 0; i <= length - 4; ++i) {
        std::string quadgram = text.substr(i, 4);
        frequency[quadgram]++;
    }

    return frequency;
}

// TRIGRAM FREQUENCY
std::unordered_map<std::string, int> Dictionary::trigramFrequency(const std::string& text) const {
    std::unordered_map<std::string, int> frequency;
    int length = text.size();
    for (int i = 0; i <= length - 3; ++i) {
        std::string trigram = text.substr(i, 3);
        frequency[trigram]++;
    }
    return frequency;
}

// BIGRAM FREQUENCY
std::unordered_map<std::string, int> Dictionary::bigramFrequency(const std::string& text) const {
    std::unordered_map<std::string, int> frequency;
    int length = text.size();
    for (int i = 0; i <= length - 2; ++i) {
        std::string bigram = text.substr(i, 2);
        frequency[bigram]++;
    }

    return frequency;
}
// INDEX OF COINCIDENCE OVER SUBSTRINGS
double Dictionary::indexOfCoincidenceOverSubstrings(const std::string& text, int substringLength) const {
    if (substringLength <= 0 || substringLength > text.size()) return 0.0;

    double totalIC = 0;
    int substringCount = 0;

    for (size_t i = 0; i + substringLength <= text.size(); i += substringLength) {
        std::string substring = text.substr(i, substringLength);

        std::unordered_map<char, int> freq;
        for (char c : substring) {
            if (std::isalpha(c)) {
                freq[std::tolower(c)]++;
            }
        }

        int n = substring.size();
        double ic = 0;
        for (const auto& [ch, count] : freq) {
            ic += count * (count - 1);
        }

        if (n > 1) {
            ic /= (n * (n - 1));
        }

        totalIC += ic;
        substringCount++;
    }

    return (substringCount > 0) ? (totalIC / substringCount) : 0.0;
}

// INDEX OF COINCIDENCE 

// N-GRAM ENTROPY
double Dictionary::ngramEntropy(const std::string& text, int n) const {
    if (n <= 0 || text.size() < n) return 0.0;

    std::unordered_map<std::string, int> freq;
    int total = 0;

    for (size_t i = 0; i + n <= text.size(); ++i) {
        std::string ngram = text.substr(i, n);
        freq[ngram]++;
        total++;
    }

    double entropy = 0;
    for (const auto& [ngram, count] : freq) {
        double p = static_cast<double>(count) / total;
        entropy -= p * log2(p);
    }

    return entropy;
}
// Score based on presence of common English words
int Dictionary::scoreCommonWords(const std::string& text) const  {
    static const std::unordered_set<std::string> commonWords = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "i", 
        "it", "for", "not", "on", "with", "he", "as", "you", "do", "at"
    };

    std::istringstream iss(text);
    std::string word;
    int score = 0;

    while (iss >> word) {
        word = cleanWord(word);
        if (commonWords.find(word) != commonWords.end()) {
            score++;
        }
    }
    return score;
}


// Calculate the average word length in the decrypted text
double Dictionary::calculateAverageWordLength(const std::string& decryptedText) const {
    std::istringstream iss(decryptedText);
    std::string word;
    int totalLength = 0;
    int wordCount = 0;

    while (iss >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        totalLength += word.length();
        wordCount++;
    }

    return (wordCount == 0) ? 0 : static_cast<double>(totalLength) / wordCount;
}
// Evaluate the quality of a decryption by scoring it based on dictionary matches, common words, and average word length
double Dictionary::evaluateDecryption(const std::string& decryptedText) const {
    // 1. Score based on dictionary matches (number of valid words from dictionary)
    int dictionaryMatchCount = countMatches(decryptedText);

    // 2. Score based on common words (how many common words appear in the decrypted text)
    int commonWordScore = scoreCommonWords(decryptedText);

    // 3. Average word length in the decrypted text
    double averageWordLength = calculateAverageWordLength(decryptedText);

    // 4. N-gram entropy to assess randomness/structure of the text (using bigrams as an example)
    double bigramEntropy = ngramEntropy(decryptedText, 2);

    // We could combine these scores into a final score, giving different weights to each factor
    double finalScore = 0.5 * dictionaryMatchCount + 0.3 * commonWordScore + 0.1 * averageWordLength - 0.2 * bigramEntropy;

    // Normalize the score to be between 0 and 1, if necessary (for example, divide by max possible score)
    double maxPossibleScore = 100.0;  // Adjust this according to your desired range
    double normalizedScore = std::max(0.0, std::min(1.0, finalScore / maxPossibleScore));

    std::cout << "Evaluation result: \n";
    std::cout << "Dictionary Matches: " << dictionaryMatchCount << std::endl;
    std::cout << "Common Word Score: " << commonWordScore << std::endl;
    std::cout << "Average Word Length: " << averageWordLength << std::endl;
    std::cout << "Bigram Entropy: " << bigramEntropy << std::endl;
    std::cout << "Final Score: " << finalScore << std::endl;
    std::cout << "Normalized Score (0-1): " << normalizedScore << std::endl;

    return normalizedScore;
}