# Test with default dictionary and Caesar cipher (encrypt)
echo "Testing Caesar cipher with default dictionary (encrypt)"
./bin/fsct caesar -e 3 "apple"  # Caesar cipher with key 3

# Test with default dictionary and Caesar cipher (decrypt)
echo "Testing Caesar cipher with default dictionary (decrypt)"
./bin/fsct caesar -d 3 "dssoh"  # Decrypt the text "apple" with key 3

# test with default dictionary and affine cipher (encrypt)
echo "Testing affine cipher with default dictionary (encrypt)"
./bin/fsct affine -e 5 12 "apple"  # Affine cipher with key 5, 12

# test with default dictionary and affine cipher (decrypt)
echo "Testing affine cipher with default dictionary (decrypt)"
./bin/fsct affine -d 5 12 "dssoh"  # Decrypt the text "apple" with key 5, 12

# test with default dictionary and vigenere cipher (encrypt)
echo "Testing vigenere cipher with default dictionary (encrypt)"
./bin/fsct vigenere -e "hello" "world"  # Vigenere cipher with key "hello"

# test with default dictionary and vigenere cipher (decrypt)
echo "Testing vigenere cipher with default dictionary (decrypt)"
./bin/fsct vigenere -d "hello" "dssoh"  # Decrypt the text "world" with key "hello"

