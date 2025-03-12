#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "../../include/ciphers/transposition.hpp"
#include "../../include/ciphers/vigenere.hpp"
#include "../../include/ciphers/affine.hpp"
#include "../../include/ciphers/caesar.hpp"
#include "../../include/ciphers/playfair.hpp"
#include "../../include/dictionary/dictionary.hpp"

// Function to display the help message
void showHelp() {
    std::cout << "Usage: fsct [ciphername] [options] [input]\n\n"
              << "Available ciphers:\n"
              << "  caesar    : Caesar cipher\n"
              << "  vigenere  : Vigenère cipher\n"
              << "  affine    : Affine cipher\n"
              << "  transposition: Transposition cipher\n"
              << "  playfair  : Playfair cipher\n\n"
              << "Options:\n"
              << "  -e [key]  : Encrypt with the specified key (integer or string depending on cipher)\n"
              << "  -d [key]  : Decrypt with the specified key (integer or string depending on cipher)\n"
              << "  -h        : Show this help message\n"
              << "  --dictionary=[filename] : Load a custom dictionary from the specified file\n"
              << "  --delim=[separator]    : Use the specified separator for dictionary\n"
              << "  -s        : Suggest possible decryptions (basic mode)\n"
              << "  -sa       : Suggest possible decryptions (advanced mode)\n\n"
              << "Input: Text to be encrypted or decrypted\n";
}

// Function to load dictionary
std::shared_ptr<Dictionary> loadDictionary(const std::string& filename = "", const std::string& delimiter = " ") {
    auto dictionary = std::make_shared<Dictionary>();
    if (!filename.empty() && !dictionary->loadFromFile(filename, delimiter[0])) {
        std::cerr << "Failed to load dictionary from " << filename << "\n";
        exit(1);
    }
    return dictionary;
}

enum CipherType {
    CAESAR,
    VIGENERE,
    AFFINE,
    TRANSPOSITION,
    PLAYFAIR,
    UNKNOWN
};

CipherType getCipherType(const std::string& cipherName) {
    static std::unordered_map<std::string, CipherType> cipherMap = {
        {"caesar", CAESAR},
        {"vigenere", VIGENERE},
        {"affine", AFFINE},
        {"transposition", TRANSPOSITION},
        {"playfair", PLAYFAIR}
    };

    auto it = cipherMap.find(cipherName);
    return (it != cipherMap.end()) ? it->second : UNKNOWN;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showHelp();
        return 1;
    }

    std::string cipherName = argv[1];
    std::string input = argv[argc - 1];
    std::string dictionaryFilename = "";
    std::string delimiter = " ";

    int key = 0;
    std::string strKey;
    bool encrypt = false, decrypt = false;
    bool suggest = false, advancedSuggest = false;

    // Load dictionary
    auto dictionary = loadDictionary(dictionaryFilename, delimiter);

    // Parse options
    for (int i = 2; i < argc - 1; ++i) {
        std::string option = argv[i];
        if (option == "-h") {
            showHelp();
            return 0;
        } else if (option == "-e" && i + 1 < argc) {
            encrypt = true;
            if (cipherName == "affine") {
                key = std::stoi(argv[++i]);
                if (i + 1 < argc) {
                    int b = std::stoi(argv[++i]);
                } else {
                    std::cerr << "Affine cipher requires two keys\n";
                    return 1;
                }
            } else if (cipherName == "vigenere" || cipherName == "playfair") {
                strKey = argv[++i];
            } else {
                key = std::stoi(argv[++i]);
            }
        } else if (option == "-d" && i + 1 < argc) {
            decrypt = true;
            if (cipherName == "affine") {
                key = std::stoi(argv[++i]);
                if (i + 1 < argc) {
                    int b = std::stoi(argv[++i]);
                } else {
                    std::cerr << "Affine cipher requires two keys\n";
                    return 1;
                }
            } else if (cipherName == "vigenere" || cipherName == "playfair") {
                strKey = argv[++i];
            } else {
                key = std::stoi(argv[++i]);
            }
        } else if (option == "-s") {
            suggest = true;
        } else if (option == "-sa") {
            suggest = true;
            advancedSuggest = true;
        } else if (option.substr(0, 12) == "--dictionary=") {
            dictionaryFilename = option.substr(12);
        } else if (option.substr(0, 9) == "--delim=") {
            delimiter = option.substr(9);
        } else {
            std::cerr << "Invalid option: " << option << "\n";
            showHelp();
            return 1;
        }
    }

    if (encrypt && decrypt) {
        std::cerr << "Cannot specify both encryption and decryption\n";
        return 1;
    }

    // Create cipher objects
    CipherType cipherType = getCipherType(cipherName);
    switch (cipherType) {
        case CAESAR: {
            Caesar caesar(input, dictionary.get());
            if (encrypt) {
                std::cout << "Encrypted text (Caesar): " << caesar.encrypt(key) << "\n";
            } else if (decrypt) {
                std::cout << "Decrypted text (Caesar): " << caesar.decrypt(key) << "\n";
            }
            break;
        }
        case VIGENERE: {
            Vigenere vigenere(input, dictionary.get(), strKey);
            if (encrypt) {
                std::cout << "Encrypted text (Vigenere): " << vigenere.encrypt() << "\n";
            } else if (decrypt) {
                std::cout << "Decrypted text (Vigenere): " << vigenere.decrypt() << "\n";
            }
            break;
        }
        case AFFINE: {
            int b = 0;
            Affine affine(input, dictionary.get(), key, b);
            if (encrypt) {
                std::cout << "Encrypted text (Affine): " << affine.encrypt() << "\n";
            } else if (decrypt) {
                std::cout << "Decrypted text (Affine): " << affine.decrypt() << "\n";
            }
            break;
        }
        case TRANSPOSITION: {
            Transposition transposition(input, dictionary.get());
            if (encrypt) {
                std::cout << "Encrypted text (Transposition): " << transposition.encrypt(key) << "\n";
            } else if (decrypt) {
                std::cout << "Decrypted text (Transposition): " << transposition.decrypt(key) << "\n";
            }
            break;
        }
        case PLAYFAIR: {
            Playfair playfair(input, dictionary.get(), strKey);
            if (encrypt) {
                std::cout << "Encrypted text (Playfair): " << playfair.encrypt() << "\n";
            } else if (decrypt) {
                std::cout << "Decrypted text (Playfair): " << playfair.decrypt() << "\n";
            }
            break;
        }
        case UNKNOWN:
            std::cerr << "Unknown cipher: " << cipherName << "\n";
            showHelp();
            return 1;
    }

    return 0;
}