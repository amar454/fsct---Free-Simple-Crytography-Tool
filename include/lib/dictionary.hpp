#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

/**
 * @class Dictionary
 * @brief A class to manage a collection of words and perform various dictionary operations.
 * 
 * This class provides functions to load a dictionary from a file, check if a word exists, 
 * add/remove words, clean and extract words from text, and suggest corrections.
 * It also includes functions to analyze word frequency and suggest words by prefix.
 * 
 * @author Amar Djulovic
 * @date 2025
 */
class Dictionary {
public:
    /**
     * @brief Default constructor for Dictionary class.
     * Initializes the dictionary with a default set of example words.
     */
    Dictionary();

    /**
     * @brief Destructor for Dictionary class.
     * Cleans up any resources used by the Dictionary object.
     */
    ~Dictionary();

    /**
     * @brief Loads dictionary data from a file.
     * 
     * @param filename The path to the dictionary file.
     * @return true if the file was successfully loaded, false otherwise.
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Checks if a word exists in the dictionary.
     * 
     * @param word The word to check for existence.
     * @return true if the word is found, false otherwise.
     */
    bool isInDictionary(const std::string& word) const;

    /**
     * @brief Adds a word to the dictionary.
     * 
     * @param word The word to add.
     */
    void addWord(const std::string& word);

    /**
     * @brief Removes a word from the dictionary.
     * 
     * @param word The word to remove.
     */
    void removeWord(const std::string& word);

    /**
     * @brief Cleans up a word by removing unwanted characters (e.g., punctuation).
     * 
     * @param word The word to clean.
     * @return The cleaned word.
     */
    std::string cleanWord(const std::string& word) const;

    /**
     * @brief Displays all words in the dictionary.
     */
    void displayDictionary() const;

    /**
     * @brief Clears all words from the dictionary.
     */
    void clearDictionary();

    /**
     * @brief Counts the number of matches of dictionary words in a given text.
     * 
     * @param text The text to analyze.
     * @return The number of dictionary words found in the text.
     */
    int countMatches(const std::string& text) const;

    /**
     * @brief Extracts all words from a given text.
     * 
     * @param text The input text.
     * @return A set of words found in the text.
     */
    std::unordered_set<std::string> extractWords(const std::string& text) const;

    /**
     * @brief Finds the most frequent word in a given text.
     * 
     * @param text The text to analyze.
     * @return The most frequent word in the text.
     */
    std::string getMostFrequentWord(const std::string& text) const;

    /**
     * @brief Returns a map of word frequencies in the given text.
     * 
     * @param text The input text.
     * @return A map of words and their frequencies.
     */
    std::unordered_map<std::string, int> getWordFrequency(const std::string& text) const;

    /**
     * @brief Returns the top N most frequent words in the given text.
     * 
     * @param text The input text.
     * @param N The number of top frequent words to return.
     * @return A list of the top N most frequent words.
     */
    std::vector<std::string> getTopNFrequentWords(const std::string& text, int N) const;

    /**
     * @brief Suggests corrections for a given word based on the dictionary.
     * 
     * @param word The word for which corrections are suggested.
     * @return A list of suggested corrections.
     */
    std::vector<std::string> suggestCorrections(const std::string& word) const;

    /**
     * @brief Finds the longest common prefix for a word in the dictionary.
     * 
     * @param word The word to check.
     * @return The longest common prefix.
     */
    std::string longestPrefix(const std::string& word) const;

    /**
     * @brief Suggests words from the dictionary that start with a given prefix.
     * 
     * @param prefix The prefix to search for.
     * @return A list of words that start with the given prefix.
     */
    std::vector<std::string> suggestByPrefix(const std::string& prefix) const;

    /**
     * @brief Finds common words between two texts.
     * 
     * @param text1 The first text.
     * @param text2 The second text.
     * @return A set of words that are common in both texts.
     */
    std::unordered_set<std::string> getCommonWords(const std::string& text1, const std::string& text2) const;

private:
    std::unordered_set<std::string> dictionary; ///< The set containing all dictionary words.
    
    /// Example set of words to initialize the dictionary with.
    const std::vector<std::string> exampleWords = {
        "a", "ability", "able", "about", "above", "accept", "access", "accident", "according", "account", 
        "achieve", "across", "act", "action", "activity", "actor", "actually", "add", "address", "advance", 
        "advice", "affect", "afford", "after", "again", "against", "age", "agency", "air", "all", "allow", "almost", 
        "alone", "already", "also", "although", "always", "am", "among", "amount", "an", "analysis", "animal", "another", 
        "answer", "anxiety", "any", "anyone", "anything", "appear", "apply", "area", "argue", "army", "arrange", "art", 
        "article", "aspect", "assault", "assess", "assign", "assist", "assume", "athlete", "attempt", "attract", "average", 
        "aware", "back", "balance", "ball", "band", "bar", "base", "basic", "battery", "be", "beautiful", "become", "before", 
        "begin", "behavior", "behind", "believe", "benefit", "best", "better", "between", "beyond", "billion", "bitter", 
        "black", "blood", "board", "body", "bottle", "bottom", "boundary", "bravery", "breathe", "brother", "budget", "build", 
        "but", "button", "cancer", "capital", "capture", "car", "care", "cause", "center", "chance", "change", "charge", "cheap", 
        "choice", "citizen", "classic", "climate", "close", "coffee", "color", "common", "community", "company", "compare", "complete", 
        "complex", "connect", "control", "courage", "create", "cultural", "current", "damage", "dancer", "danger", "data", "decide", 
        "defend", "define", "degree", "demand", "detect", "develop", "disease", "district", "divide", "doctor", "domestic", "during", 
        "dynamic", "economy", "education", "effect", "eliminate", "energy", "engage", "environment", "error", "evaluate", "example", 
        "expand", "experience", "factor", "feature", "final", "finance", "flavor", "flood", "follow", "forget", "form", "former", 
        "friend", "front", "future", "gallery", "generate", "govern", "grade", "grand", "handle", "happen", "harbor", "harmony", 
        "health", "hearing", "height", "honor", "hotel", "human", "ideal", "impact", "implement", "import", "improve", "increase", 
        "initial", "inspire", "insert", "interior", "invest", "journal", "judge", "journey", "judge", "keen", "labor", "land", 
        "leader", "legacy", "level", "library", "limit", "literature", "local", "manage", "market", "measure", "memory", "mention", 
        "message", "method", "modern", "moment", "monitor", "network", "notice", "obtain", "occur", "office", "online", "option", 
        "outcome", "overcome", "partner", "pattern", "performance", "planet", "positive", "power", "practical", "preach", "prepare", 
        "process", "project", "public", "purpose", "quality", "quote", "react", "reality", "relate", "research", "result", "revenue", 
        "reveal", "safety", "science", "secure", "segment", "sensitive", "service", "settle", "signal", "situation", "society", "source", 
        "special", "speech", "spirit", "standard", "strength", "submit", "success", "support", "supply", "surface", "sustain", 
        "system", "talent", "teacher", "theory", "thrive", "throne", "together", "traffic", "transform", "unite", "unique", 
        "universe", "update", "urban", "utilize", "vision", "visible", "vital", "wealth", "weigh", "wellness", "within", 
        "witness", "wonder", "youth", "zeal"
    };
};

#endif // DICTIONARY_HPP
