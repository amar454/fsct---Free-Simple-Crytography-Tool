/*

    Copyright (c) 2025 Amar Djulovic <aaamargml@gmail.com>

    This file is a part of the 
*/



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
    Dictionary();
    ~Dictionary();
    // Load a dictionary from a file.
    bool loadFromFile(const std::string& filename, char delimiter);

    // add a word to the dictionary
    void addWord(const std::string& word);

    // remove a word from the dictionary
    void removeWord(const std::string& word);

    // print the dictionary to cout 
    void displayDictionary() const;

    // clear the dictionary
    void clearDictionary();

    // check if a word is in the dictionary
    bool isInDictionary(const std::string& word) const;

    //  clean a word by removing non-alphabetic characters
    std::string cleanWord(const std::string& word) const;

    // use the Levenshtein distance algorithm to find the closest match for a word
    int levenshteinDistance(const std::string& a, const std::string& b) const;

    // extract words from a text
    std::unordered_set<std::string> extractWords(const std::string& text) const;

    // get the frequency of each word in a text
    std::unordered_map<std::string, int> getWordFrequency(const std::string& text) const;

    // get the most frequent word in a text
    std::string getMostFrequentWord(const std::string& text) const;

    // get the top N most frequent words in a text
    std::vector<std::string> getTopNFrequentWords(const std::string& text, int N) const;
    
    // count the number of matches in a text
    int countMatches(const std::string& text) const;
    
    // return the number of words in the dictionary
    int size() const;

    // return the longest word in the dictionary
    std::string getLongestWord() const;

    // return the shortest word in the dictionary
    std::string getShortestWord() const;

    // return true if the word is a palindrome
    bool isPalindrome(const std::string& word) const;

    // return a list of palindromic words in the dictionary
    std::vector<std::string> getPalindromicWords() const;

    // suggesst corrections for a word
    std::vector<std::string> suggestCorrections(const std::string& word) const;

    // return a list of anagrams for a word
    std::vector<std::string> findAnagrams(const std::string& word) const;

    // Suggest words by reversing the word and checking if it exists in the dictionary
    std::vector<std::string> suggestByReversal(const std::string& word) const;

    // Identify quadrigrams in a text and return their frequency
    std::unordered_map<std::string, int> quadgramFrequency(const std::string& text) const;
    
    // Identify trigrams in a text and return their frequency
    std::unordered_map<std::string, int> trigramFrequency(const std::string& text) const;

    // Identify bigrams in a text and return their frequency
    std::unordered_map<std::string, int> bigramFrequency(const std::string& text) const;

    // identify n grams in a text and return their frequency
    double ngramEntropy(const std::string& text, int n) const;
    
    // Calculate the index of coincidence for a given text, over substrings of length substringLength
    double indexOfCoincidenceOverSubstrings(const std::string& text, int substringLength) const;
    
    // Calculate the index of coincidence for a given text, over substrings of length 3
    std::vector<std::string> suggestByPrefix(const std::string& prefix) const;

    // score the text based on the number of common words
    int scoreCommonWords(const std::string& text) const;

    double calculateAverageWordLength(const std::string& decryptedText) const;

    double evaluateDecryption(const std::string& decryptedText) const;
private:
    std::unordered_set<std::string> dictionary;
    
    /// Example set of words to initialize the dictionary with.
    const std::vector<std::string> predefinedDictionary = {
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
