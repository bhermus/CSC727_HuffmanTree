#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

class HuffmanTree {
public:
    // Struct to hold letter and frequency
    struct Node {
        char letter;
        int frequency;
        Node* left;
        Node* right;

        Node(char l, int f) : letter(l), frequency(f), left(nullptr), right(nullptr) {}
    };

    // Method to initialize letters and frequencies
    void initialize() {
        std::vector<char> letters;
        std::vector<int> frequencies;

        while (true) {
            std::cout << "Enter 10 letters (a-z or A-Z) separated by spaces or commas, or press Enter to use default values (A-J):\n";
            std::string input;
            std::getline(std::cin, input);

            // Use default values if input is empty
            if (input.empty()) {
                letters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
                frequencies = { 10, 15, 12, 3, 4, 13, 1, 8, 20, 14 };
                break;
            }

            // Parse the input for letters
            std::replace(input.begin(), input.end(), ',', ' '); // Replace commas with spaces
            std::istringstream iss(input);
            std::string token;
            while (iss >> token) {
                if (token.size() != 1 || !std::isalpha(token[0])) {
                    std::cerr << "Invalid input! Each letter must be a single alphabetical character.\n";
                    letters.clear(); // Clear to restart
                    break;
                }
                letters.push_back(std::toupper(token[0]));
            }

            if (letters.size() == 10) {
                break; // Valid input
            }
            else {
                std::cerr << "You must enter exactly 10 letters. Please try again.\n";
                letters.clear();
            }
        }

        // Prompt for frequencies
        if (frequencies.empty()) {
            int totalFrequency = 0;
            for (size_t i = 0; i < 9; ++i) {
                int frequency;
                while (true) {
                    std::cout << "Enter frequency for " << letters[i] << ": ";
                    std::cin >> frequency;

                    if (frequency < 0 || totalFrequency + frequency >= 100) {
                        std::cerr << "Invalid frequency! Ensure frequencies are positive and total does not exceed 99.\n";
                    }
                    else {
                        break; // Valid input
                    }
                }
                frequencies.push_back(frequency);
                totalFrequency += frequency;
            }

            // Calculate frequency for the last letter
            frequencies.push_back(100 - totalFrequency);

            std::cout << "Remaining frequency assigned to " << letters[9] << ": " << frequencies[9] << "%\n";
        }
        // Print the initialized letters and frequencies
        std::cout << "Initialized letters and frequencies:\n";
        for (size_t i = 0; i < letters.size(); ++i) {
            std::cout << letters[i] << ": " << frequencies[i] << "%\n";
        }
    }
};

int main() {
    HuffmanTree huffmanTree;
    huffmanTree.initialize();
    return 0;
}
