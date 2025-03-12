#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>
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

// Function to load dictionary from file or use predefined dictionary
std::shared_ptr<Dictionary> loadDictionary(const std::string& filename = "", const std::string& delimiter = " ") {
    if (filename.empty()) {
        return std::make_shared<Dictionary>();
    }
    
    auto dictionary = std::make_shared<Dictionary>();
    if (!dictionary->loadFromFile(filename, delimiter[0])) {
        std::cerr << "Failed to load dictionary from " << filename << "\n";
        exit(1);
    }
    return dictionary;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showHelp();
        return 1;
    }

    std::string cipherName = argv[1];
    std::string input = argv[argc - 1]; // The last argument is the input text

    int key = 0;
    std::string strKey;

    std::string dictionaryFilename = "";
    std::string delimiter = " ";

    bool encrypt = false;
    bool decrypt = false;
    bool suggest = false;
    bool advancedSuggest = false;
    // Create the cipher object based on user input
    std::shared_ptr<Transposition> transpositionCipher;
    std::shared_ptr<Vigenere> vigenereCipher;
    std::shared_ptr<Affine> affineCipher;
    std::shared_ptr<Caesar> caesarCipher;
    std::shared_ptr<Playfair> playfairCipher;
    // Load dictionary
    std::shared_ptr<Dictionary> dictionary = loadDictionary(dictionaryFilename, delimiter);

    // Parse command-line arguments
    for (int i = 2; i < argc - 1; ++i) {
        std::string option = argv[i];

        if (option == "-h") {
            showHelp();
            return 0;
        } else if (option == "-e" && i + 1 < argc) {
    encrypt = true;
    if (cipherName == "vigenere" || cipherName == "playfair") {
        strKey = argv[++i];
    } else if (cipherName == "affine") {
        key = std::stoi(argv[++i]); // First key (a)
        if (i + 1 < argc) {
            int b = std::stoi(argv[++i]); // Second key (b)
            affineCipher = std::make_shared<Affine>(input, dictionary.get(), key, b); // Pass both a and b to the affine cipher
        } else {
            std::cerr << "Error: Affine cipher requires two keys (a and b)\n";
            return 1;
        }
    } else {
        key = std::stoi(argv[++i]);
    }
} else if (option == "-d" && i + 1 < argc) {
    decrypt = true;
    if (cipherName == "vigenere" || cipherName == "playfair") {
        strKey = argv[++i];
    } else if (cipherName == "affine") {
        key = std::stoi(argv[++i]); // First key (a)
        if (i + 1 < argc) {
            int b = std::stoi(argv[++i]); // Second key (b)
            affineCipher = std::make_shared<Affine>(input, dictionary.get(), key, b); // Pass both a and b to the affine cipher
        } else {
            std::cerr << "Error: Affine cipher requires two keys (a and b)\n";
            return 1;
        }
    } else {
        key = std::stoi(argv[++i]);
    }
} else if (option == "-s") {
            suggest = true;  // Basic suggestion mode
        } else if (option == "-sa") {
            suggest = true;  // Advanced suggestion mode
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
        std::cerr << "Error: Cannot specify both encryption and decryption\n";
        return 1;
    }

    
    

    if (cipherName == "caesar") {
        caesarCipher = std::make_shared<Caesar>(input, dictionary.get());
    } else if (cipherName == "vigenere") {
        vigenereCipher = std::make_shared<Vigenere>(input, dictionary.get(), strKey);
    } else if (cipherName == "affine") {
        affineCipher = std::make_shared<Affine>(input, dictionary.get(), key, 0);
    } else if (cipherName == "transposition") {
        transpositionCipher = std::make_shared<Transposition>(input, dictionary.get());
    } else if (cipherName == "playfair") {
        playfairCipher = std::make_shared<Playfair>(input, dictionary.get(), strKey);
    } else {
        std::cerr << "Unknown cipher: " << cipherName << "\n";
        showHelp();
        return 1;
    }

    if (suggest) {
        if (cipherName == "affine") {
            if (advancedSuggest) {
                affineCipher->suggestDecryptions(5, "advanced", true);  // Advanced mode
            } else {
                affineCipher->suggestDecryptions(5, "basic", false);  // Basic mode
            }
        } else if (cipherName == "caesar") {
            if (advancedSuggest) {
                caesarCipher->suggestDecryptions(5, "advanced");
            } else {
                caesarCipher->suggestDecryptions(5, "basic");
            }
        } else if (cipherName == "vigenere") {
            if (advancedSuggest) {
                vigenereCipher->suggestDecryptions(5, "advanced", true);
            } else {
                vigenereCipher->suggestDecryptions(5, "basic", false);
            }
        } else if (cipherName == "transposition") {
            if (advancedSuggest) {
                transpositionCipher->suggestDecryptions(5, "advanced");
            } else {
                transpositionCipher->suggestDecryptions(5, "basic");
            }
        } else if (cipherName == "playfair") {
            if (advancedSuggest) {
                playfairCipher->suggestDecryptions(5, "advanced");
            } else {
                playfairCipher->suggestDecryptions(5, "basic");
            }
        } else {
            std::cerr << "Suggestion mode is only supported for certain ciphers currently.\n";
            return 1;
        }
    }
    

    // Perform encryption or decryption
    if (encrypt) {
        if (cipherName == "caesar") {
            std::cout << "Encrypted text (Caesar): " << caesarCipher->encrypt(key) << "\n";
        } else if (cipherName == "vigenere") {
            std::cout << "Encrypted text (Vigenere): " << vigenereCipher->encrypt() << "\n";
        } else if (cipherName == "affine") {
            std::cout << "Encrypted text (Affine): " << affineCipher->encrypt() << "\n";
        } else if (cipherName == "transposition") {
            std::cout << "Encrypted text (Transposition): " << transpositionCipher->encrypt(key) << "\n";
        } else if (cipherName == "playfair") {
            std::cout << "Encrypted text (Playfair): " << playfairCipher->encrypt() << "\n";
        }
    } else if (decrypt) {
        if (cipherName == "caesar") {
            std::cout << "Decrypted text (Caesar): " << caesarCipher->decrypt(key) << "\n";
        } else if (cipherName == "vigenere") {
            std::cout << "Decrypted text (Vigenere): " << vigenereCipher->decrypt() << "\n";
        } else if (cipherName == "affine") {
            std::cout << "Decrypted text (Affine): " << affineCipher->decrypt() << "\n";
        } else if (cipherName == "transposition") {
            std::cout << "Decrypted text (Transposition): " << transpositionCipher->decrypt(key) << "\n";
        } else if (cipherName == "playfair") {
            std::cout << "Decrypted text (Playfair): " << playfairCipher->decrypt() << "\n";
        }
    }

    return 0;
}
