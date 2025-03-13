#include "../../include/analysis/pattern_finder.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>
#include <numeric>

PatternFinder::PatternFinder(const std::string& text) 
    : text(normalizeText(text)) {
}

std::vector<Pattern> PatternFinder::findRepeatingPatterns(size_t minLength, size_t maxLength) const {
    std::vector<Pattern> patterns;
    
    for (size_t length = minLength; length <= maxLength; ++length) {
        std::map<std::string, std::vector<size_t>> patternMap;
        
        // Find all patterns of current length
        for (size_t i = 0; i <= text.length() - length; ++i) {
            std::string currentPattern = text.substr(i, length);
            if (isValidPattern(currentPattern)) {
                patternMap[currentPattern].push_back(i);
            }
        }
        
        // Add patterns that repeat
        for (const auto& entry : patternMap) {
            if (entry.second.size() > 1) {
                Pattern p = {
                    entry.first,
                    entry.second,
                    length,
                    entry.second.size()
                };
                patterns.push_back(p);
            }
        }
    }
    
    // Sort patterns by occurrence count
    std::sort(patterns.begin(), patterns.end(),
              [](const Pattern& a, const Pattern& b) {
                  return a.occurrences > b.occurrences;
              });
    
    return patterns;
}

KasiskiResult PatternFinder::performKasiskiExamination(size_t minLength) const {
    KasiskiResult result;
    std::map<int, int> spacingFrequencies;
    
    // Find repeating patterns
    auto patterns = findRepeatingPatterns(minLength, 10);
    
    // Calculate spacings for each pattern
    for (const auto& pattern : patterns) {
        auto spacings = calculateSpacings(pattern.positions);
        for (int spacing : spacings) {
            if (isPotentialKey(spacing)) {
                spacingFrequencies[spacing]++;
            }
        }
    }
    
    // Find possible key lengths based on common factors
    std::set<int> keyLengths;
    for (const auto& entry : spacingFrequencies) {
        auto factors = findFactors(entry.first);
        keyLengths.insert(factors.begin(), factors.end());
    }
    
    result.possibleKeyLengths = std::vector<int>(keyLengths.begin(), keyLengths.end());
    result.spacingFrequencies = spacingFrequencies;
    
    return result;
}

std::vector<std::string> PatternFinder::findAnagrams() const {
    std::vector<std::string> anagrams;
    std::map<std::string, std::vector<std::string>> sortedGroups;
    
    std::istringstream iss(text);
    std::string word;
    
    while (iss >> word) {
        std::string sorted = word;
        std::sort(sorted.begin(), sorted.end());
        sortedGroups[sorted].push_back(word);
    }
    
    for (const auto& group : sortedGroups) {
        if (group.second.size() > 1) {
            anagrams.insert(anagrams.end(), 
                          group.second.begin(), 
                          group.second.end());
        }
    }
    
    return anagrams;
}

std::map<char, std::vector<size_t>> PatternFinder::findLetterSpacing() const {
    std::map<char, std::vector<size_t>> spacings;
    
    for (char c = 'A'; c <= 'Z'; ++c) {
        std::vector<size_t> positions;
        size_t pos = text.find(c);
        
        while (pos != std::string::npos) {
            positions.push_back(pos);
            pos = text.find(c, pos + 1);
        }
        
        if (!positions.empty()) {
            spacings[c] = positions;
        }
    }
    
    return spacings;
}

double PatternFinder::calculatePatternDensity() const {
    size_t totalPatterns = 0;
    auto patterns = findRepeatingPatterns(2, 5);
    
    for (const auto& pattern : patterns) {
        totalPatterns += pattern.occurrences;
    }
    
    return static_cast<double>(totalPatterns) / text.length();
}

size_t PatternFinder::countUniquePatterns(size_t length) const {
    std::set<std::string> uniquePatterns;
    
    for (size_t i = 0; i <= text.length() - length; ++i) {
        std::string pattern = text.substr(i, length);
        if (isValidPattern(pattern)) {
            uniquePatterns.insert(pattern);
        }
    }
    
    return uniquePatterns.size();
}

std::vector<double> PatternFinder::getPatternFrequencies(size_t length) const {
    std::map<std::string, int> patternCounts;
    size_t totalPatterns = 0;
    
    for (size_t i = 0; i <= text.length() - length; ++i) {
        std::string pattern = text.substr(i, length);
        if (isValidPattern(pattern)) {
            patternCounts[pattern]++;
            totalPatterns++;
        }
    }
    
    std::vector<double> frequencies;
    for (const auto& entry : patternCounts) {
        frequencies.push_back(static_cast<double>(entry.second) / totalPatterns);
    }
    
    std::sort(frequencies.begin(), frequencies.end(), std::greater<double>());
    return frequencies;
}

bool PatternFinder::isValidPattern(const std::string& pattern) const {
    return std::all_of(pattern.begin(), pattern.end(), 
                      [](char c) { return std::isalpha(c); });
}

std::vector<size_t> PatternFinder::findAllOccurrences(const std::string& pattern) const {
    std::vector<size_t> positions;
    size_t pos = text.find(pattern);
    
    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = text.find(pattern, pos + 1);
    }
    
    return positions;
}

std::vector<int> PatternFinder::calculateSpacings(const std::vector<size_t>& positions) const {
    std::vector<int> spacings;
    
    for (size_t i = 0; i < positions.size(); ++i) {
        for (size_t j = i + 1; j < positions.size(); ++j) {
            spacings.push_back(static_cast<int>(positions[j] - positions[i]));
        }
    }
    
    return spacings;
}

std::vector<int> PatternFinder::findFactors(int number) const {
    std::vector<int> factors;
    
    for (int i = 2; i <= std::sqrt(number); ++i) {
        if (number % i == 0) {
            factors.push_back(i);
            if (i != number / i) {
                factors.push_back(number / i);
            }
        }
    }
    
    std::sort(factors.begin(), factors.end());
    return factors;
}

bool PatternFinder::isPotentialKey(int spacing) const {
    return spacing >= 2 && spacing <= 20; // Typical key length range
}

std::string PatternFinder::normalizeText(const std::string& input) const {
    std::string normalized;
    for (char c : input) {
        if (std::isalpha(c)) {
            normalized += std::toupper(c);
        }
    }
    return normalized;
}
