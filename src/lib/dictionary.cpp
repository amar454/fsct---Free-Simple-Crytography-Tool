#include "lib/dictionary.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Dictionary::Dictionary() {
    // Constructor to initialize the dictionary.
}

Dictionary::~Dictionary() {
    // Destructor if needed for cleanup.
}

/**
 * @brief Loads words into the dictionary from a file.
 *
 * @param filename The file containing the dictionary words.
 * @return bool Returns true if loading was successful, false otherwise.
 */
bool Dictionary::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            dictionary.insert(cleanWord(word));
        }
        file.close();
        return true;
    } else {
        std::cerr << "Error: Could not load dictionary file '" << filename << "'.\n";
        return false;
    }
}

/**
 * @brief Checks if a given word exists in the dictionary.
 *
 * @param word The word to check.
 * @return bool Returns true if the word exists, false otherwise.
 */
bool Dictionary::isInDictionary(const std::string& word) const {
    return dictionary.find(word) != dictionary.end();
}

/**
 * @brief Adds a word to the dictionary.
 *
 * @param word The word to add.
 */
void Dictionary::addWord(const std::string& word) {
    dictionary.insert(cleanWord(word));
}

/**
 * @brief Removes a word from the dictionary.
 *
 * @param word The word to remove.
 */
void Dictionary::removeWord(const std::string& word) {
    dictionary.erase(cleanWord(word));
}

/**
 * @brief Returns a clean version of the word (lowercase and alphabetic only).
 *
 * @param word The word to clean.
 * @return std::string The cleaned word.
 */
std::string Dictionary::cleanWord(const std::string& word) const {
    std::string cleanedWord;
    for (char ch : word) {
        if (std::isalpha(ch)) {
            cleanedWord += std::tolower(ch);
        }
    }
    return cleanedWord;
}

/**
 * @brief Displays all words currently in the dictionary.
 */
void Dictionary::displayDictionary() const {
    for (const auto& word : dictionary) {
        std::cout << word << '\n';
    }
}

/**
 * @brief Clears all words in the dictionary.
 */
void Dictionary::clearDictionary() {
    dictionary.clear();
}

/**
 * @brief Counts how many words in the text are in the dictionary.
 *
 * @param text The text to analyze.
 * @return int The number of matching words.
 */
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

/**
 * @brief Returns a list of words found in the text.
 *
 * @param text The text to extract words from.
 * @return std::unordered_set<std::string> A set of words found in the text.
 */
std::unordered_set<std::string> Dictionary::extractWords(const std::string& text) const {
    std::unordered_set<std::string> words;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        words.insert(cleanWord(word));
    }
    return words;
}

/**
 * @brief Finds the most frequent word in a given text that is in the dictionary.
 *
 * @param text The text to analyze.
 * @return std::string The most frequent word found in the dictionary.
 */
std::string Dictionary::getMostFrequentWord(const std::string& text) const {
    std::unordered_map<std::string, int> wordFrequency;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        word = cleanWord(word);
        if (isInDictionary(word)) {
            wordFrequency[word]++;
        }
    }

    std::string mostFrequentWord;
    int maxFrequency = 0;
    for (const auto& entry : wordFrequency) {
        if (entry.second > maxFrequency) {
            maxFrequency = entry.second;
            mostFrequentWord = entry.first;
        }
    }

    return mostFrequentWord;
}

/**
 * @brief Returns a map of word frequencies for words found in the text.
 *
 * @param text The text to analyze.
 * @return std::unordered_map<std::string, int> A map of word frequencies.
 */
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

/**
 * @brief Returns the top N frequent words from the text.
 *
 * @param text The text to analyze.
 * @param N The number of top frequent words to return.
 * @return std::vector<std::string> A list of the top N frequent words.
 */
std::vector<std::string> Dictionary::getTopNFrequentWords(const std::string& text, int N) const {
    std::unordered_map<std::string, int> wordFrequency = getWordFrequency(text);
    std::vector<std::pair<std::string, int>> rankedWords(wordFrequency.begin(), wordFrequency.end());
    
    std::sort(rankedWords.begin(), rankedWords.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<std::string> topNWords;
    for (int i = 0; i < N && i < rankedWords.size(); ++i) {
        topNWords.push_back(rankedWords[i].first);
    }

    return topNWords;
}

/**
 * @brief Suggests words from the dictionary based on a given word.
 *
 * @param word The word to base the suggestions on.
 * @return std::vector<std::string> A list of suggested words.
 */
std::vector<std::string> Dictionary::suggestCorrections(const std::string& word) const {
    std::vector<std::string> suggestions;
    // Placeholder for real implementation, e.g., Levenshtein distance or other heuristics.
    return suggestions;
}

/**
 * @brief Finds the longest word from the dictionary that starts with the given prefix.
 *
 * @param word The prefix to match against.
 * @return std::string The longest matching word.
 */
std::string Dictionary::longestPrefix(const std::string& word) const {
    std::string longest;
    for (const auto& dictWord : dictionary) {
        if (dictWord.rfind(word, 0) == 0 && dictWord.size() > longest.size()) {
            longest = dictWord;
        }
    }
    return longest;
}

/**
 * @brief Suggests words from the dictionary that start with a given prefix.
 *
 * @param prefix The prefix to match against.
 * @return std::vector<std::string> A list of suggested words.
 */
std::vector<std::string> Dictionary::suggestByPrefix(const std::string& prefix) const {
    std::vector<std::string> suggestions;
    for (const auto& dictWord : dictionary) {
        if (dictWord.rfind(prefix, 0) == 0) {
            suggestions.push_back(dictWord);
        }
    }
    return suggestions;
}

/**
 * @brief Finds common words between two texts.
 *
 * @param text1 The first text to analyze.
 * @param text2 The second text to analyze.
 * @return std::unordered_set<std::string> A set of common words between both texts.
 */
std::unordered_set<std::string> Dictionary::getCommonWords(const std::string& text1, const std::string& text2) const {
    auto words1 = extractWords(text1);
    auto words2 = extractWords(text2);
    std::unordered_set<std::string> commonWords;
    
    for (const auto& word : words1) {
        if (words2.find(word) != words2.end()) {
            commonWords.insert(word);
        }
    }

    return commonWords;
}
