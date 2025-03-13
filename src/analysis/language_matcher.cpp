#include "../../include/analysis/language_matcher.hpp"
#include "../../include/analysis/entropy_calculator.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <curl/curl.h>
#include <regex>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Enhanced constructors with better error handling
LanguageMatcher::LanguageMatcher(const std::string& dictionaryFilePath) {
    if (!loadDictionaryFromFile(dictionaryFilePath)) {
        throw std::runtime_error("Dictionary load failed: " + dictionaryFilePath);
    }
    initializeDefaultPatterns();
}

LanguageMatcher::LanguageMatcher(const std::vector<std::string>& wordList) {
    for (const auto& word : wordList) {
        dictionary.insert(normalizeWord(word));
    }
    initializeDefaultPatterns();
}

LanguageMatcher::LanguageMatcher(bool downloadDict) {
    if (downloadDict && !downloadDictionary()) {
        throw std::runtime_error("Online dictionary download failed");
    }
    initializeDefaultPatterns();
}

MatchResult LanguageMatcher::analyzeText(const std::string& text) const {
    MatchResult result;
    auto words = tokenizeText(text);
    
    // Calculate word matches
    for (const auto& word : words) {
        if (isValidWord(word)) {
            result.matchedWords.push_back(word);
        }
    }
    
    // Calculate scores
    result.wordFrequencyScore = calculateWordFrequencyScore(text);
    result.structuralScore = calculateGrammarScore(text);
    
    // Calculate overall confidence
    result.confidence = (result.wordFrequencyScore * 0.6 + 
                        result.structuralScore * 0.4);
    
    // Find grammar matches
    for (const auto& pattern : grammarPatterns) {
        std::regex re(pattern.first);
        if (std::regex_search(text, re)) {
            result.grammarMatches.insert(
                result.grammarMatches.end(),
                pattern.second.begin(),
                pattern.second.end()
            );
        }
    }
    
    return result;
}

double LanguageMatcher::calculateLanguageConfidence(const std::string& text) const {
    auto words = tokenizeText(text);
    int validWords = 0;
    
    for (const auto& word : words) {
        if (isValidWord(word)) {
            validWords++;
        }
    }
    
    double wordRatio = static_cast<double>(validWords) / words.size();
    double grammarScore = calculateGrammarScore(text);
    
    return (wordRatio * 0.7 + grammarScore * 0.3) * 100.0;
}

std::vector<std::string> LanguageMatcher::identifyValidWords(const std::string& text) const {
    std::vector<std::string> validWords;
    auto words = tokenizeText(text);
    
    for (const auto& word : words) {
        std::string normalized = normalizeWord(word);
        if (isValidWord(normalized)) {
            validWords.push_back(word);
        }
    }
    
    return validWords;
}

void LanguageMatcher::addLanguageProfile(const LanguageProfile& profile) {
    languageProfiles.push_back(profile);
}

void LanguageMatcher::loadLanguageRules(const std::string& rulesFile) {
    std::ifstream file(rulesFile);
    std::string line;
    
    while (std::getline(file, line)) {
        size_t delimPos = line.find('=');
        if (delimPos != std::string::npos) {
            std::string pattern = line.substr(0, delimPos);
            std::string rules = line.substr(delimPos + 1);
            
            std::vector<std::string> ruleList;
            std::istringstream ruleStream(rules);
            std::string rule;
            
            while (std::getline(ruleStream, rule, ',')) {
                ruleList.push_back(rule);
            }
            
            grammarPatterns[pattern] = ruleList;
        }
    }
}

std::vector<std::pair<std::string, double>> 
LanguageMatcher::detectPossibleLanguages(const std::string& text) const {
    std::vector<std::pair<std::string, double>> results;
    
    for (const auto& profile : languageProfiles) {
        double confidence = 0.0;
        auto words = tokenizeText(text);
        
        for (const auto& word : words) {
            auto it = profile.wordFrequencies.find(normalizeWord(word));
            if (it != profile.wordFrequencies.end()) {
                confidence += it->second;
            }
        }
        
        confidence /= words.size();
        results.push_back({profile.name, confidence});
    }
    
    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    return results;
}

void LanguageMatcher::updateDictionary(const std::string& word) {
    dictionary.insert(normalizeWord(word));
}

void LanguageMatcher::removeDictionaryWord(const std::string& word) {
    dictionary.erase(normalizeWord(word));
}

bool LanguageMatcher::isValidWord(const std::string& word) const {
    return dictionary.find(normalizeWord(word)) != dictionary.end();
}

double LanguageMatcher::calculateWordFrequencyScore(const std::string& text) const {
    auto words = tokenizeText(text);
    std::map<std::string, int> wordCounts;
    
    for (const auto& word : words) {
        wordCounts[normalizeWord(word)]++;
    }
    
    double score = 0.0;
    for (const auto& profile : languageProfiles) {
        for (const auto& count : wordCounts) {
            auto it = profile.wordFrequencies.find(count.first);
            if (it != profile.wordFrequencies.end()) {
                score += std::abs(it->second - 
                    (static_cast<double>(count.second) / words.size()));
            }
        }
    }
    
    return 1.0 - (score / languageProfiles.size());
}

double LanguageMatcher::calculateGrammarScore(const std::string& text) const {
    double score = 0.0;
    int patternMatches = 0;
    
    for (const auto& pattern : grammarPatterns) {
        std::regex re(pattern.first);
        auto words_begin = 
            std::sregex_iterator(text.begin(), text.end(), re);
        auto words_end = std::sregex_iterator();
        
        patternMatches += std::distance(words_begin, words_end);
    }
    
    if (!grammarPatterns.empty()) {
        score = static_cast<double>(patternMatches) / grammarPatterns.size();
    }
    
    return score;
}

std::vector<std::string> LanguageMatcher::findCommonPhrases(const std::string& text) const {
    std::vector<std::string> phrases;
    std::string normalized = text;
    
    for (const auto& profile : languageProfiles) {
        for (const auto& commonWord : profile.commonWords) {
            size_t pos = normalized.find(commonWord);
            if (pos != std::string::npos) {
                phrases.push_back(commonWord);
            }
        }
    }
    
    return phrases;
}

std::vector<std::string> LanguageMatcher::tokenizeText(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string LanguageMatcher::normalizeWord(const std::string& word) const {
    std::string normalized;
    for (char c : word) {
        if (std::isalpha(c)) {
            normalized += std::tolower(c);
        }
    }
    return normalized;
}

bool LanguageMatcher::loadDictionaryFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    std::string word;
    while (std::getline(file, word)) {
        dictionary.insert(normalizeWord(word));
    }
    
    return true;
}

void LanguageMatcher::initializeDefaultPatterns() {
    // Common English grammar patterns
    grammarPatterns["\\b(the|a|an)\\s+\\w+"] = {"article_noun"};
    grammarPatterns["\\b(is|are|was|were)\\s+\\w+"] = {"be_verb"};
    grammarPatterns["\\b\\w+ing\\b"] = {"present_participle"};
    grammarPatterns["\\b\\w+ed\\b"] = {"past_tense"};
    grammarPatterns["\\b(very|quite|rather)\\s+\\w+"] = {"intensifier"};
}
// Add this method to download the dictionary
bool LanguageMatcher::downloadDictionary() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/dwyl/english-words/master/words.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            return false;
        }

        // Process the downloaded content
        std::istringstream iss(readBuffer);
        std::string word;
        while (std::getline(iss, word)) {
            dictionary.insert(normalizeWord(word));
        }
        
        return true;
    }
    return false;
}

double LanguageMatcher::calculateWordSimilarity(
    const std::string& word1, const std::string& word2) const {
    int m = word1.length();
    int n = word2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (word1[i-1] == word2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + std::min({dp[i-1][j],
                                       dp[i][j-1],
                                       dp[i-1][j-1]});
            }
        }
    }
    
    return 1.0 - (static_cast<double>(dp[m][n]) / 
                  std::max(word1.length(), word2.length()));
}
std::map<std::string, double> LanguageMatcher::analyzeNGramDistribution(const std::string& text, size_t n) const {
    EntropyCalculator entropyCalc;
    
    // Get entropy-based n-gram probabilities
    auto ngramProbabilities = entropyCalc.calculateNGramProbabilities(text, n);
    
    // Calculate entropy metrics for each n-gram
    std::map<std::string, double> distribution;
    for (const auto& [ngram, prob] : ngramProbabilities) {
        double entropyScore = -prob * entropyCalc.calculateLog2(prob);
        distribution[ngram] = entropyScore;
    }
    
    // Add entropy spectrum analysis
    auto entropySpectrum = entropyCalc.calculateEntropySpectrum(text, n);
    double spectrumWeight = entropySpectrum[n-1];
    
    // Weight the distributions by entropy spectrum
    for (auto& [ngram, score] : distribution) {
        score *= spectrumWeight;
    }
    
    return distribution;
}

double LanguageMatcher::calculateTextComplexity(const std::string& text) const {
    EntropyCalculator entropyCalc;
    auto words = tokenizeText(text);
    
    if (words.empty()) return 0.0;
    
    // Calculate average word length
    double avgLength = std::accumulate(words.begin(), words.end(), 0.0,
        [](double sum, const std::string& word) {
            return sum + word.length();
        }) / words.size();
    
    // Calculate unique words ratio
    std::set<std::string> uniqueWords(words.begin(), words.end());
    double uniqueRatio = static_cast<double>(uniqueWords.size()) / words.size();
    
    // Get comprehensive entropy metrics
    EntropyMetrics metrics = entropyCalc.calculateFullMetrics(text);
    
    // Enhanced complexity calculation using multiple entropy features
    double entropyScore = (metrics.shannonEntropy * 0.4 + 
                          metrics.normalizedEntropy * 0.3 + 
                          metrics.conditionalEntropy * 0.3);
    
    return (avgLength * 0.25 + uniqueRatio * 0.25 + entropyScore * 0.5);
}


std::vector<std::pair<std::string, std::string>> 
LanguageMatcher::findSimilarWords(const std::string& word, double threshold) const {
    std::vector<std::pair<std::string, std::string>> similarWords;
    std::string normalizedInput = normalizeWord(word);
    
    // Parallel processing for large dictionaries
    const size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::future<std::vector<std::pair<std::string, std::string>>>> futures;
    
    auto processWords = [this, &normalizedInput, threshold]
        (const std::vector<std::string>& wordBatch) {
        std::vector<std::pair<std::string, std::string>> results;
        for (const auto& dictWord : wordBatch) {
            double similarity = calculateWordSimilarity(normalizedInput, dictWord);
            if (similarity >= threshold) {
                results.emplace_back(dictWord, std::to_string(similarity));
            }
        }
        return results;
    };
    
    // Split dictionary into batches
    std::vector<std::string> dictVec(dictionary.begin(), dictionary.end());
    size_t batchSize = dictVec.size() / threadCount;
    
    for (size_t i = 0; i < threadCount; ++i) {
        auto start = dictVec.begin() + i * batchSize;
        auto end = (i == threadCount - 1) ? dictVec.end() : start + batchSize;
        std::vector<std::string> batch(start, end);
        
        futures.push_back(std::async(std::launch::async, processWords, batch));
    }
    
    // Collect results
    for (auto& future : futures) {
        auto results = future.get();
        similarWords.insert(similarWords.end(), results.begin(), results.end());
    }
    
    return similarWords;
}

void LanguageMatcher::mergeDictionary(const std::set<std::string>& newWords) {
    dictionary.insert(newWords.begin(), newWords.end());
}

