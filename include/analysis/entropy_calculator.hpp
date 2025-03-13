#ifndef ENTROPY_CALCULATOR_HPP
#define ENTROPY_CALCULATOR_HPP

#include <string>
#include <vector>
#include <map>

struct EntropyMetrics {
    double shannonEntropy;
    double normalizedEntropy;
    double conditionalEntropy;
    double jointEntropy;
    double mutualInformation;
    double relativeEntropy;
    std::map<char, double> characterProbabilities;
    std::map<std::string, double> ngramEntropies;
};

class EntropyCalculator {
public:
    EntropyCalculator();
    explicit EntropyCalculator(const std::string& referenceText);
    
    // Core entropy calculations
    EntropyMetrics calculateFullMetrics(const std::string& text) const;
    double calculateShannon(const std::string& text) const;
    double calculateNormalizedEntropy(const std::string& text) const;
    double calculateConditionalEntropy(const std::string& text, const std::string& condition) const;
    
    // Advanced entropy metrics
    double calculateJointEntropy(const std::string& text1, const std::string& text2) const;
    double calculateMutualInformation(const std::string& text1, const std::string& text2) const;
    double calculateRelativeEntropy(const std::string& text, const std::string& referenceText) const;
    
    // N-gram entropy analysis
    std::map<std::string, double> calculateNGramEntropies(const std::string& text, size_t n) const;
    double calculateMarkovEntropy(const std::string& text, size_t order) const;
    
    // Entropy-based features
    double calculateEntropyRate(const std::string& text) const;
    double calculateCrossEntropy(const std::string& text1, const std::string& text2) const;
    std::vector<double> calculateEntropySpectrum(const std::string& text, size_t maxOrder) const;
    // Helper methods
    std::map<char, double> calculateProbabilities(const std::string& text) const;
    std::map<std::string, double> calculateNGramProbabilities(const std::string& text, size_t n) const;
    double calculateLog2(double value) const;
    std::vector<std::string> extractNGrams(const std::string& text, size_t n) const;
private:
    std::map<char, double> referenceDistribution;
    
    
    void initializeReferenceDistribution(const std::string& text);
};

#endif
