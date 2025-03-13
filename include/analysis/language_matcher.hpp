#ifndef LANGUAGE_MATCHER_HPP
#define LANGUAGE_MATCHER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <future>
#include <numeric>
#include "entropy_calculator.hpp" 
struct LanguageProfile {
    std::string name;
    std::map<std::string, double> wordFrequencies;
    std::vector<std::string> commonWords;
    std::map<std::string, std::string> grammarRules;
    double averageWordLength;
};

struct MatchResult {
    double confidence;
    std::vector<std::string> matchedWords;
    std::vector<std::string> grammarMatches;
    double wordFrequencyScore;
    double structuralScore;
};

class LanguageMatcher {
public:
    // Constructors for different dictionary sources
    LanguageMatcher(const std::string& dictionaryFilePath);
    LanguageMatcher(const std::vector<std::string>& wordList);
    LanguageMatcher(bool downloadDict);
    // Core analysis methods
    std::map<std::string, double> analyzeNGramDistribution(const std::string& text, size_t n) const;
    MatchResult analyzeText(const std::string& text) const;
    double calculateLanguageConfidence(const std::string& text) const;
    std::vector<std::string> identifyValidWords(const std::string& text) const;
    
    // Language profile methods
    void addLanguageProfile(const LanguageProfile& profile);
    void loadLanguageRules(const std::string& rulesFile);
    std::vector<std::pair<std::string, double>> detectPossibleLanguages(const std::string& text) const;
    
    // Dictionary management
    void updateDictionary(const std::string& word);
    void removeDictionaryWord(const std::string& word);
    bool isValidWord(const std::string& word) const;
    
    // Statistical analysis
    double calculateWordFrequencyScore(const std::string& text) const;
    double calculateGrammarScore(const std::string& text) const;
    double calculateTextComplexity(const std::string&) const;
    std::vector<std::pair<std::string, std::string>> findSimilarWords(const std::string& word, double threshold) const;
    std::vector<std::string> findCommonPhrases(const std::string& text) const;
    
private:
    std::set<std::string> dictionary;
    std::vector<LanguageProfile> languageProfiles;
    std::map<std::string, std::vector<std::string>> grammarPatterns;
    bool downloadDictionary();
    void mergeDictionary(const std::set<std::string>& newWords);
    // Helper methods
    std::vector<std::string> tokenizeText(const std::string& text) const;
    std::string normalizeWord(const std::string& word) const;
    bool loadDictionaryFromFile(const std::string& filePath);
    void initializeDefaultPatterns();
    double calculateWordSimilarity(const std::string& word1, const std::string& word2) const;
};

#endif