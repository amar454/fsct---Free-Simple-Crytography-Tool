#ifndef PATTERN_FINDER_HPP
#define PATTERN_FINDER_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>

struct Pattern {
    std::string sequence;
    std::vector<size_t> positions;
    size_t length;
    size_t occurrences;
};

struct KasiskiResult {
    std::vector<int> possibleKeyLengths;
    std::map<int, int> spacingFrequencies;
};

class PatternFinder {
public:
    PatternFinder(const std::string& text);
    
    // Main analysis methods
    std::vector<Pattern> findRepeatingPatterns(size_t minLength = 3, size_t maxLength = 10) const;
    KasiskiResult performKasiskiExamination(size_t minLength = 3) const;
    std::vector<std::string> findAnagrams() const;
    std::map<char, std::vector<size_t>> findLetterSpacing() const;
    double calculatePatternDensity() const;

    // Pattern statistics
    size_t countUniquePatterns(size_t length) const;
    std::vector<double> getPatternFrequencies(size_t length) const;
    
    // Utility methods
    bool isValidPattern(const std::string& pattern) const;
    std::vector<size_t> findAllOccurrences(const std::string& pattern) const;
    
private:
    std::string text;
    
    // Helper methods
    std::vector<int> calculateSpacings(const std::vector<size_t>& positions) const;
    std::vector<int> findFactors(int number) const;
    bool isPotentialKey(int spacing) const;
    std::string normalizeText(const std::string& input) const;
};

#endif