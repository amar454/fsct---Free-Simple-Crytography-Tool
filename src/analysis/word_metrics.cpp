#include "../../include/analysis/word_metrics.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <numeric>

WordMetrics::WordMetrics() {
    initializeDefaultPatterns();
}

WordMetrics::WordMetrics(const std::string& languageConfig) {
    loadLanguageConfig(languageConfig);
}

WordMetricsResult WordMetrics::analyzeText(const std::string& text) const {
    WordMetricsResult result;
    auto words = tokenizeText(text);
    
    result.averageWordLength = calculateAverageWordLength(text);
    result.lexicalDiversity = calculateLexicalDiversity(text);
    result.syllableComplexity = 0.0;
    result.uniqueWordCount = std::set<std::string>(words.begin(), words.end()).size();
    result.typeTokenRatio = calculateTypeTokenRatio(text);
    result.mostComplexWords = getMostComplexWords(text, 10);
    result.wordLengthDistribution = getWordLengthDistribution(text);
    result.readabilityScore = calculateReadabilityScore(text);
    result.vocabularyRichness = calculateVocabularyRichness(text);
    
    return result;
}

WordComplexityMetrics WordMetrics::analyzeWord(const std::string& word) const {
    WordComplexityMetrics metrics;
    metrics.syllableCount = countSyllables(word);
    metrics.morphemes = extractMorphemes(word);
    metrics.morphemeCount = metrics.morphemes.size();
    metrics.isCompound = isCompoundWord(word);
    metrics.complexityScore = calculateWordComplexity(word);
    return metrics;
}

double WordMetrics::calculateAverageWordLength(const std::string& text) const {
    auto words = tokenizeText(text);
    if (words.empty()) return 0.0;
    
    double totalLength = std::accumulate(words.begin(), words.end(), 0.0,
        [](double sum, const std::string& word) { return sum + word.length(); });
    return totalLength / words.size();
}

double WordMetrics::calculateLexicalDiversity(const std::string& text) const {
    auto words = tokenizeText(text);
    std::set<std::string> uniqueWords;
    for (const auto& word : words) {
        uniqueWords.insert(normalizeWord(word));
    }
    return static_cast<double>(uniqueWords.size()) / words.size();
}

double WordMetrics::calculateVocabularyRichness(const std::string& text) const {
    auto freqDist = getWordFrequencyDistribution(text);
    double totalWords = 0;
    for (const auto& pair : freqDist) {
        totalWords += pair.second;
    }
    return std::log(freqDist.size()) / std::log(totalWords);
}

double WordMetrics::calculateReadabilityScore(const std::string& text) const {
    auto words = tokenizeText(text);
    double avgSyllables = 0.0;
    for (const auto& word : words) {
        avgSyllables += countSyllables(word);
    }
    avgSyllables /= words.size();
    
    return 206.835 - 1.015 * (words.size()) - 84.6 * avgSyllables;
}

std::vector<std::string> WordMetrics::getMostComplexWords(const std::string& text, size_t n) const {
    auto words = tokenizeText(text);
    std::vector<std::pair<std::string, double>> wordComplexities;
    
    for (const auto& word : words) {
        wordComplexities.emplace_back(word, calculateWordComplexity(word));
    }
    
    std::sort(wordComplexities.begin(), wordComplexities.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> result;
    for (size_t i = 0; i < std::min(n, wordComplexities.size()); ++i) {
        result.push_back(wordComplexities[i].first);
    }
    return result;
}

std::map<std::string, size_t> WordMetrics::getSyllableCounts(
    const std::vector<std::string>& words) const {
    std::map<std::string, size_t> counts;
    for (const auto& word : words) {
        counts[word] = countSyllables(word);
    }
    return counts;
}

std::vector<std::string> WordMetrics::extractMorphemes(const std::string& word) const {
    return findMorphemePatterns(word);
}

std::map<size_t, size_t> WordMetrics::getWordLengthDistribution(const std::string& text) const {
    auto words = tokenizeText(text);
    std::map<size_t, size_t> distribution;
    for (const auto& word : words) {
        distribution[word.length()]++;
    }
    return distribution;
}

std::map<std::string, size_t> WordMetrics::getWordFrequencyDistribution(
    const std::string& text) const {
    auto words = tokenizeText(text);
    std::map<std::string, size_t> distribution;
    for (const auto& word : words) {
        distribution[normalizeWord(word)]++;
    }
    return distribution;
}

double WordMetrics::calculateTypeTokenRatio(const std::string& text) const {
    auto words = tokenizeText(text);
    std::set<std::string> uniqueWords;
    for (const auto& word : words) {
        uniqueWords.insert(normalizeWord(word));
    }
    return static_cast<double>(uniqueWords.size()) / words.size();
}

double WordMetrics::calculateHapaxLegomenaRatio(const std::string& text) const {
    auto freqDist = getWordFrequencyDistribution(text);
    size_t hapaxCount = std::count_if(freqDist.begin(), freqDist.end(),
        [](const auto& pair) { return pair.second == 1; });
    return static_cast<double>(hapaxCount) / freqDist.size();
}

double WordMetrics::calculateYuleK(const std::string& text) const {
    auto freqDist = getWordFrequencyDistribution(text);
    double M1 = 0.0, M2 = 0.0;
    
    for (const auto& pair : freqDist) {
        M1 += pair.second;
        M2 += pair.second * pair.second;
    }
    
    return 10000 * (M2 - M1) / (M1 * M1);
}

bool WordMetrics::isCompoundWord(const std::string& word) const {
    return decomposeCompoundWord(word).size() > 1;
}

std::vector<std::string> WordMetrics::decomposeCompoundWord(const std::string& word) const {
    std::vector<std::string> components;
    std::string normalized = normalizeWord(word);
    
    for (const auto& pattern : morphemePatterns) {
        size_t pos = 0;
        while ((pos = normalized.find(pattern.first, pos)) != std::string::npos) {
            components.push_back(pattern.first);
            pos += pattern.first.length();
        }
    }
    
    return components;
}

size_t WordMetrics::countSyllables(const std::string& word) const {
    size_t count = 0;
    bool prevIsVowel = false;
    
    for (char c : word) {
        bool currIsVowel = isVowel(c);
        if (currIsVowel && !prevIsVowel) {
            count++;
        }
        prevIsVowel = currIsVowel;
    }
    
    return std::max(count, size_t(1));
}

// Private helper methods implementation
bool WordMetrics::isVowel(char c) const {
    c = std::tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
}

bool WordMetrics::isConsonant(char c) const {
    return std::isalpha(c) && !isVowel(c);
}

std::vector<std::string> WordMetrics::tokenizeText(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string WordMetrics::normalizeWord(const std::string& word) const {
    std::string normalized;
    for (char c : word) {
        if (std::isalpha(c)) {
            normalized += std::tolower(c);
        }
    }
    return normalized;
}

void WordMetrics::loadLanguageConfig(const std::string& configPath) {
    std::ifstream file(configPath);
    // Implementation of config loading logic
    initializeDefaultPatterns();
}

void WordMetrics::initializeDefaultPatterns() {
    // Initialize default morpheme patterns
    morphemePatterns = {
        {"pre", {"pre", "pro", "anti"}},
        {"suffix", {"ing", "ed", "ly", "tion"}},
        {"root", {"act", "form", "struct", "port"}}
    };
    
    // Initialize common affixes
    commonAffixes = {"un", "re", "dis", "able", "ible", "ment"};
    
    // Initialize syllable rules
    syllableRules = {
        {"consonant_cluster", 1},
        {"vowel_sequence", 1},
        {"silent_e", 0}
    };
}

double WordMetrics::calculateWordComplexity(const std::string& word) const {
    double complexity = 0.0;
    complexity += countSyllables(word) * 0.4;
    complexity += countMorphemes(word) * 0.3;
    complexity += (isCompoundWord(word) ? 0.3 : 0.0);
    return complexity;
}

size_t WordMetrics::countMorphemes(const std::string& word) const {
    return extractMorphemes(word).size();
}

std::vector<std::string> WordMetrics::findMorphemePatterns(const std::string& word) const {
    std::vector<std::string> patterns;
    std::string normalized = normalizeWord(word);
    
    for (const auto& pattern : morphemePatterns) {
        for (const auto& morpheme : pattern.second) {
            if (normalized.find(morpheme) != std::string::npos) {
                patterns.push_back(morpheme);
            }
        }
    }
    
    return patterns;
}

double WordMetrics::calculateStandardDeviation(const std::vector<double>& values) const {
    double mean = calculateMean(values);
    double sumSquares = 0.0;
    
    for (double value : values) {
        sumSquares += (value - mean) * (value - mean);
    }
    
    return std::sqrt(sumSquares / values.size());
}

double WordMetrics::calculateMean(const std::vector<double>& values) const {
    if (values.empty()) return 0.0;
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}
