#ifndef VIGENERE_HPP
#define VIGENERE_HPP

#include <string>
#include "lib/dictionary.hpp"

class VigenereCipher {
public:
    // Constructor to initialize the ciphertext and dictionary
    VigenereCipher(const std::string& cipherText, Dictionary& dictionary);

    // Decrypt the ciphertext using the specified key
    std::string decrypt(const std::string& key);

    // Attempt to decrypt using all possible keys up to the specified length
    void tryAllKeys(int maxKeyLength);

private:
    std::string cipherText;    // The ciphertext to be decrypted
    Dictionary& dictionary;    // Reference to the Dictionary object

    // Count the number of valid dictionary words in the decrypted text
    int countMatches(const std::string& decryptedText);

    // Evaluate the decryption based on word frequency and match count
    double evaluateDecryption(const std::string& decryptedText, int matchCount);

    // Calculate a score based on the frequency of common words in the decrypted text
    double calculateWordFrequencyScore(const std::string& decryptedText);

    // Remove punctuation from a word
    std::string removePunctuation(const std::string& word);

    // Increment the key in lexicographical order
    bool incrementKey(std::string& key);

    // Sanitize the decrypted text to make further analysis more difficult
    std::string sanitizeResult(const std::string& decryptedText);
};

#endif // VIGENERE_HPP
