#ifndef FREQUENCY_ANALYZER_HPP
#define FREQUENCY_ANALYZER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <array>
#include "entropy_calculator.hpp"

struct FrequencyData {
    char character;
    int count;
    double frequency;
    double expectedFrequency;
    double deviation;
};

struct NGramData {
    std::string sequence;
    int count;
    double frequency;
    std::vector<size_t> positions;  // Unique to FrequencyAnalyzer
};

class FrequencyAnalyzer {
public:
    FrequencyAnalyzer(const std::string& text);
    
    // Core frequency analysis
    std::vector<FrequencyData> analyzeCharacterFrequencies() const;
    std::map<char, double> getRelativeFrequencies() const;
    std::vector<std::pair<char, char>> suggestSimpleSubstitutions() const;
    
    // Enhanced N-gram analysis with position tracking
    std::vector<NGramData> analyzeBigrams() const;
    std::vector<NGramData> analyzeTrigrams() const;
    std::vector<NGramData> analyzeNGrams(size_t n) const;
    
    // Statistical measures specific to frequency analysis
    double calculateIndexOfCoincidence() const;
    double calculateChiSquared() const;
    
    // Language analysis
    double compareToLanguageProfile(const std::map<char, double>& languageProfile) const;
    std::string detectProbableLanguage() const;
    
    // Utility functions
    size_t getTextLength() const;
    std::string getNormalizedText() const;
    static std::map<char, double> getEnglishFrequencies();

private:
    std::string text;
    static const std::array<double, 26> ENGLISH_FREQUENCIES;
    EntropyCalculator entropyCalc;  // For any entropy-related calculations
    
    // Helper methods
    std::string normalizeText(const std::string& input) const;
    bool isValidChar(char c) const;
    std::vector<FrequencyData> calculateDeviations(const std::vector<FrequencyData>& data) const;
    double calculateFrequency(int count, size_t total) const;
};

#endif
