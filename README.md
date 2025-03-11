# fsct - Free Simple Cryptography Tool

## Description
The Cipher App is a collection of encryption and decryption algorithms implemented in C++. It includes classical ciphers such as Vigenère, Caesar, Playfair, and Affine. The program allows users to input encrypted text and attempt to decipher it using different cipher algorithms, and it can also suggest the most likely decrypted texts based on common word patterns.

### Supported Ciphers:
- **Vigenère Cipher**: Decrypts text using the Vigenère encryption technique with a user-supplied key.
- **Caesar Cipher**: Decrypts text using a Caesar shift, with suggestions for the best shifts based on common word patterns.
- **Playfair Cipher**: Implements the Playfair encryption algorithm for digraph substitution.
- **Affine Cipher**: Implements the Affine cipher for simple letter substitutions based on a mathematical function.

## Cryptanalysis Feature
The cryptanalysis feature analyzes the encrypted text and suggests the best possible decryptions based on common word patterns and word frequency. Here's how it works:

1. The program attempts all possible Caesar shifts (0–25) and ranks the results based on:
   - **Dictionary Match Count**: The number of recognized words from common English vocabulary.
   - **Average Word Length**: The average length of words in the decrypted text (longer words tend to be more natural).
   - **Common Word Score**: A score based on the presence of highly common English words like "the", "and", "to", etc.

2. The best decryption results are displayed with the highest score first. The user can choose to display the full decryption for a specific shift.

3. The cryptanalysis feature works for both **Caesar** and **Vigenère** ciphers by trying all possible keys and scoring the results.

## Requirements
- A C++17 compatible compiler (e.g., `g++`).

## Building the Project

### Steps:
1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/amar454/fsct---Free-Simple-Crytography-Tool.cpp
    ```
2. Navigate into the project directory:
    ```bash
    cd fsct---Free-Simple-Crytography-Tool
    ```
3. Build the project using `make`:
    ```bash
    make
    ```

4. Run the program:
    ```bash
    make run
    ```

## License
This project is licensed under the GNU General Public License v3.0.

Copyright (C) 2025 Amar Djulovic

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

## Acknowledgements
- [GNU General Public License](https://www.gnu.org/licenses/)
- Contributions from the open-source cryptography community.
