#include "../../include/analysis/entropy_calculator.hpp"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <curl/curl.h>
EntropyCalculator::EntropyCalculator() {
    // Initialize uniform distribution
    for (char c = 'a'; c <= 'z'; ++c) {
        referenceDistribution[c] = 1.0 / 26;
    }
}

EntropyCalculator::EntropyCalculator(const std::string& referenceText) {
    initializeReferenceDistribution(referenceText);
}

EntropyMetrics EntropyCalculator::calculateFullMetrics(const std::string& text) const {
    EntropyMetrics metrics;
    metrics.shannonEntropy = calculateShannon(text);
    metrics.normalizedEntropy = calculateNormalizedEntropy(text);
    metrics.conditionalEntropy = calculateConditionalEntropy(text, text);
    metrics.jointEntropy = calculateJointEntropy(text.substr(0, text.length()/2), 
                                               text.substr(text.length()/2));
    metrics.mutualInformation = calculateMutualInformation(text.substr(0, text.length()/2), 
                                                         text.substr(text.length()/2));
    metrics.relativeEntropy = calculateRelativeEntropy(text, text);
    metrics.characterProbabilities = calculateProbabilities(text);
    metrics.ngramEntropies = calculateNGramEntropies(text, 3);
    
    return metrics;
}

double EntropyCalculator::calculateShannon(const std::string& text) const {
    auto probabilities = calculateProbabilities(text);
    double entropy = 0.0;
    
    for (const auto& [symbol, prob] : probabilities) {
        if (prob > 0) {
            entropy -= prob * calculateLog2(prob);
        }
    }
    
    return entropy;
}

double EntropyCalculator::calculateNormalizedEntropy(const std::string& text) const {
    double maxEntropy = calculateLog2(static_cast<double>(text.length()));
    return calculateShannon(text) / maxEntropy;
}

double EntropyCalculator::calculateConditionalEntropy(
    const std::string& text, const std::string& condition) const {
    auto jointProbs = calculateNGramProbabilities(text + condition, 2);
    auto conditionProbs = calculateProbabilities(condition);
    double conditionalEntropy = 0.0;
    
    for (const auto& [pair, jointProb] : jointProbs) {
        if (pair.length() >= 2) {
            char conditionChar = pair[1];
            double condProb = conditionProbs[conditionChar];
            if (condProb > 0 && jointProb > 0) {
                conditionalEntropy -= jointProb * calculateLog2(jointProb / condProb);
            }
        }
    }
    
    return conditionalEntropy;
}

double EntropyCalculator::calculateJointEntropy(
    const std::string& text1, const std::string& text2) const {
    std::map<std::pair<char, char>, double> jointDist;
    size_t minLength = std::min(text1.length(), text2.length());
    
    for (size_t i = 0; i < minLength; ++i) {
        jointDist[{text1[i], text2[i]}] += 1.0 / minLength;
    }
    
    double jointEntropy = 0.0;
    for (const auto& [pair, prob] : jointDist) {
        if (prob > 0) {
            jointEntropy -= prob * calculateLog2(prob);
        }
    }
    
    return jointEntropy;
}

double EntropyCalculator::calculateMutualInformation(
    const std::string& text1, const std::string& text2) const {
    double entropy1 = calculateShannon(text1);
    double entropy2 = calculateShannon(text2);
    double jointEntropy = calculateJointEntropy(text1, text2);
    return entropy1 + entropy2 - jointEntropy;
}

double EntropyCalculator::calculateRelativeEntropy(
    const std::string& text, const std::string& referenceText) const {
    auto probsP = calculateProbabilities(text);
    auto probsQ = calculateProbabilities(referenceText);
    double relativeEntropy = 0.0;
    
    for (const auto& [symbol, probP] : probsP) {
        double probQ = probsQ[symbol];
        if (probP > 0 && probQ > 0) {
            relativeEntropy += probP * calculateLog2(probP / probQ);
        }
    }
    
    return relativeEntropy;
}

std::map<std::string, double> EntropyCalculator::calculateNGramEntropies(
    const std::string& text, size_t n) const {
    std::map<std::string, double> entropies;
    
    for (size_t i = 1; i <= n; ++i) {
        auto ngramProbs = calculateNGramProbabilities(text, i);
        double entropy = 0.0;
        
        for (const auto& [ngram, prob] : ngramProbs) {
            if (prob > 0) {
                entropy -= prob * calculateLog2(prob);
            }
        }
        
        entropies[std::to_string(i) + "-gram"] = entropy;
    }
    
    return entropies;
}

double EntropyCalculator::calculateMarkovEntropy(
    const std::string& text, size_t order) const {
    auto transitionProbs = calculateNGramProbabilities(text, order + 1);
    double entropy = 0.0;
    
    for (const auto& [sequence, prob] : transitionProbs) {
        if (prob > 0) {
            entropy -= prob * calculateLog2(prob);
        }
    }
    
    return entropy;
}

double EntropyCalculator::calculateEntropyRate(const std::string& text) const {
    return calculateShannon(text) / text.length();
}

double EntropyCalculator::calculateCrossEntropy(
    const std::string& text1, const std::string& text2) const {
    auto probs1 = calculateProbabilities(text1);
    auto probs2 = calculateProbabilities(text2);
    double crossEntropy = 0.0;
    
    for (const auto& [symbol, prob1] : probs1) {
        if (prob1 > 0 && probs2[symbol] > 0) {
            crossEntropy -= prob1 * calculateLog2(probs2[symbol]);
        }
    }
    
    return crossEntropy;
}

std::vector<double> EntropyCalculator::calculateEntropySpectrum(
    const std::string& text, size_t maxOrder) const {
    std::vector<double> spectrum;
    for (size_t i = 1; i <= maxOrder; ++i) {
        spectrum.push_back(calculateMarkovEntropy(text, i));
    }
    return spectrum;
}

std::map<char, double> EntropyCalculator::calculateProbabilities(
    const std::string& text) const {
    std::map<char, double> probabilities;
    double total = text.length();
    
    for (char c : text) {
        probabilities[c] += 1.0 / total;
    }
    
    return probabilities;
}

std::map<std::string, double> EntropyCalculator::calculateNGramProbabilities(
    const std::string& text, size_t n) const {
    auto ngrams = extractNGrams(text, n);
    std::map<std::string, double> probabilities;
    double total = ngrams.size();
    
    for (const auto& ngram : ngrams) {
        probabilities[ngram] += 1.0 / total;
    }
    
    return probabilities;
}

double EntropyCalculator::calculateLog2(double value) const {
    return std::log2(value);
}

std::vector<std::string> EntropyCalculator::extractNGrams(
    const std::string& text, size_t n) const {
    std::vector<std::string> ngrams;
    
    for (size_t i = 0; i <= text.length() - n; ++i) {
        ngrams.push_back(text.substr(i, n));
    }
    
    return ngrams;
}

void EntropyCalculator::initializeReferenceDistribution(const std::string& text) {
    referenceDistribution = calculateProbabilities(text);
}
