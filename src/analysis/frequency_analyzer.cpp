#include "../../include/analysis/frequency_analyzer.hpp"
#include "../../include/analysis/entropy_calculator.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <sstream>

// Initialize English letter frequencies, found them somewhere on the internet
const std::array<double, 26> FrequencyAnalyzer::ENGLISH_FREQUENCIES = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, // A-E
    0.02228, 0.02015, 0.06094, 0.06966, 0.00153, // F-J
    0.00772, 0.04025, 0.02406, 0.06749, 0.07507, // K-O
    0.01929, 0.00095, 0.05987, 0.06327, 0.09056, // P-T
    0.02758, 0.00978, 0.02360, 0.00150, 0.01974, // U-Y
    0.00074  // Z
};

// Constructor with text normalization
FrequencyAnalyzer::FrequencyAnalyzer(const std::string& text) 
    : text(normalizeText(text)) {
    if (text.empty()) {
        throw std::invalid_argument("Input text cannot be empty");
    }
}

// analyze character frequencies 
std::vector<FrequencyData> FrequencyAnalyzer::analyzeCharacterFrequencies() const {
    std::vector<FrequencyData> frequencies;
    std::map<char, int> charCount;
    
    // Count occurrences
    for (char c : text) {
        if (isValidChar(c)) {
            charCount[c]++;
        }
    }
    
    // Calculate frequencies
    size_t totalChars = text.length();
    for (const auto& pair : charCount) {
        FrequencyData data;
        data.character = pair.first;
        data.count = pair.second;
        data.frequency = calculateFrequency(pair.second, totalChars);
        data.expectedFrequency = ENGLISH_FREQUENCIES[pair.first - 'A'];
        data.deviation = std::abs(data.frequency - data.expectedFrequency);
        
        frequencies.push_back(data);
    }
    
    // Sort by frequency
    std::sort(frequencies.begin(), frequencies.end(),
              [](const FrequencyData& a, const FrequencyData& b) {
                  return a.frequency > b.frequency;
              });
    
    return frequencies;
}

std::vector<NGramData> FrequencyAnalyzer::analyzeNGrams(size_t n) const {
    std::vector<NGramData> ngrams;
    std::map<std::string, NGramData> ngramMap;
    
    // Get n-gram probabilities from entropy calculator
    auto ngramProbabilities = entropyCalc.calculateNGramProbabilities(text, n);
    auto extractedNGrams = entropyCalc.extractNGrams(text, n);
    
    // Track positions and counts
    for (size_t i = 0; i <= text.length() - n; ++i) {
        std::string sequence = text.substr(i, n);
        
        if (ngramMap.find(sequence) == ngramMap.end()) {
            ngramMap[sequence] = {
                sequence,
                1,
                ngramProbabilities[sequence],
                {i}
            };
        } else {
            ngramMap[sequence].count++;
            ngramMap[sequence].positions.push_back(i);
        }
    }
    
    // Convert map to vector
    for (const auto& pair : ngramMap) {
        ngrams.push_back(pair.second);
    }
    
    // Sort by frequency
    std::sort(ngrams.begin(), ngrams.end(),
        [](const NGramData& a, const NGramData& b) {
            return a.frequency > b.frequency;
        });
    
    return ngrams;
}

std::vector<NGramData> FrequencyAnalyzer::analyzeBigrams() const {
    return analyzeNGrams(2);
}

std::vector<NGramData> FrequencyAnalyzer::analyzeTrigrams() const {
    return analyzeNGrams(3);
}

double FrequencyAnalyzer::calculateIndexOfCoincidence() const {
    std::vector<int> frequencies(26, 0);
    size_t totalChars = 0;
    
    for (char c : text) {
        if (isValidChar(c)) {
            frequencies[c - 'A']++;
            totalChars++;
        }
    }
    
    double ic = 0.0;
    for (int freq : frequencies) {
        ic += (freq * (freq - 1));
    }
    
    return ic / (totalChars * (totalChars - 1));
}

double FrequencyAnalyzer::calculateChiSquared() const {
    auto observed = analyzeCharacterFrequencies();
    double chiSquared = 0.0;
    size_t totalChars = text.length();
    
    for (const auto& data : observed) {
        double expected = totalChars * ENGLISH_FREQUENCIES[data.character - 'A'];
        double difference = data.count - expected;
        chiSquared += (difference * difference) / expected;
    }
    
    return chiSquared;
}


std::vector<std::pair<char, char>> FrequencyAnalyzer::suggestSimpleSubstitutions() const {
    std::vector<std::pair<char, char>> suggestions;
    auto observed = analyzeCharacterFrequencies();
    
    // Create sorted list of expected frequencies
    std::vector<std::pair<char, double>> expected;
    for (char c = 'A'; c <= 'Z'; ++c) {
        expected.push_back({c, ENGLISH_FREQUENCIES[c - 'A']});
    }
    
    std::sort(expected.begin(), expected.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    // Match observed with expected frequencies
    for (size_t i = 0; i < std::min(observed.size(), expected.size()); ++i) {
        suggestions.push_back({observed[i].character, expected[i].first});
    }
    
    return suggestions;
}

double FrequencyAnalyzer::compareToLanguageProfile(
    const std::map<char, double>& languageProfile) const {
    auto observed = getRelativeFrequencies();
    double difference = 0.0;
    
    for (const auto& pair : languageProfile) {
        double observedFreq = observed[pair.first];
        difference += std::abs(observedFreq - pair.second);
    }
    
    return difference;
}

std::map<char, double> FrequencyAnalyzer::getRelativeFrequencies() const {
    std::map<char, double> frequencies;
    auto data = analyzeCharacterFrequencies();
    
    for (const auto& freq : data) {
        frequencies[freq.character] = freq.frequency;
    }
    
    return frequencies;
}

std::map<char, double> FrequencyAnalyzer::getEnglishFrequencies() {
    std::map<char, double> frequencies;
    for (char c = 'A'; c <= 'Z'; ++c) {
        frequencies[c] = ENGLISH_FREQUENCIES[c - 'A'];
    }
    return frequencies;
}

std::string FrequencyAnalyzer::getNormalizedText() const {
    return text;
}

size_t FrequencyAnalyzer::getTextLength() const {
    return text.length();
}

std::string FrequencyAnalyzer::normalizeText(const std::string& input) const {
    std::string normalized;
    for (char c : input) {
        if (std::isalpha(c)) {
            normalized += std::toupper(c);
        }
    }
    return normalized;
}

bool FrequencyAnalyzer::isValidChar(char c) const {
    return c >= 'A' && c <= 'Z';
}

double FrequencyAnalyzer::calculateFrequency(int count, size_t total) const {
    return static_cast<double>(count) / total;
}


