#ifndef WORD_METRICS_HPP
#define WORD_METRICS_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

struct WordMetricsResult {
    double averageWordLength;
    double lexicalDiversity;
    double syllableComplexity;
    size_t uniqueWordCount;
    double typeTokenRatio;
    std::vector<std::string> mostComplexWords;
    std::map<size_t, size_t> wordLengthDistribution;
    double readabilityScore;
    double vocabularyRichness;
};

struct WordComplexityMetrics {
    size_t syllableCount;
    size_t morphemeCount;
    double complexityScore;
    bool isCompound;
    std::vector<std::string> morphemes;
};

class WordMetrics {
public:
    // Constructors
    WordMetrics();
    explicit WordMetrics(const std::string& languageConfig);

    // Core analysis methods
    WordMetricsResult analyzeText(const std::string& text) const;
    WordComplexityMetrics analyzeWord(const std::string& word) const;
    
    // Statistical metrics
    double calculateAverageWordLength(const std::string& text) const;
    double calculateLexicalDiversity(const std::string& text) const;
    double calculateVocabularyRichness(const std::string& text) const;
    double calculateReadabilityScore(const std::string& text) const;
    
    // Word-specific analysis
    std::vector<std::string> getMostComplexWords(const std::string& text, size_t n) const;
    std::map<std::string, size_t> getSyllableCounts(const std::vector<std::string>& words) const;
    std::vector<std::string> extractMorphemes(const std::string& word) const;
    
    // Distribution analysis
    std::map<size_t, size_t> getWordLengthDistribution(const std::string& text) const;
    std::map<std::string, size_t> getWordFrequencyDistribution(const std::string& text) const;
    
    // Advanced metrics
    double calculateTypeTokenRatio(const std::string& text) const;
    double calculateHapaxLegomenaRatio(const std::string& text) const;
    double calculateYuleK(const std::string& text) const;
    
    // Morphological analysis
    bool isCompoundWord(const std::string& word) const;
    std::vector<std::string> decomposeCompoundWord(const std::string& word) const;
    size_t countSyllables(const std::string& word) const;

private:
    // Configuration
    std::map<std::string, std::vector<std::string>> morphemePatterns;
    std::set<std::string> commonAffixes;
    std::map<std::string, size_t> syllableRules;
    
    // Helper methods
    std::vector<std::string> tokenizeText(const std::string& text) const;
    std::string normalizeWord(const std::string& word) const;
    bool isVowel(char c) const;
    bool isConsonant(char c) const;
    
    // Advanced analysis helpers
    double calculateWordComplexity(const std::string& word) const;
    std::vector<std::string> findMorphemePatterns(const std::string& word) const;
    size_t countMorphemes(const std::string& word) const;
    
    // Statistical helpers
    double calculateStandardDeviation(const std::vector<double>& values) const;
    double calculateMean(const std::vector<double>& values) const;
    
    // Configuration methods
    void loadLanguageConfig(const std::string& configPath);
    void initializeDefaultPatterns();
};

#endif
