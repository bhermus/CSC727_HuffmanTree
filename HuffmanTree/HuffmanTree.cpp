#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept> // For exception inherited classes

// Exception classes
class NoMem : public std::exception {
    const char* what() const noexcept override { return "Heap is full."; }
};

class OutOfBounds : public std::exception {
    const char* what() const noexcept override { return "Heap is empty."; }
};

// MinHeap definition (from course website)
template <class T>
class MinHeap {
public:
    MinHeap(int MSize) {
        MaxSize = MSize;
        heap = new T[MaxSize + 1];
        Size = 0;
    }
    ~MinHeap() { delete[] heap; }
    MinHeap<T>& Insert(T& x);
    MinHeap<T>& Delete(T& x);
    int Size;

private:
    int MaxSize;
    T* heap;
};

template <class T>
MinHeap<T>& MinHeap<T>::Insert(T& x) {
    if (Size == MaxSize)
        throw NoMem();
    else {
        int i = ++Size;
        while (i != 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }
        heap[i] = x;
        return *this;
    }
}

template <class T>
MinHeap<T>& MinHeap<T>::Delete(T& x) {
    if (Size == 0) throw OutOfBounds();
    x = heap[1];  // root has the smallest key
    T y = heap[Size--]; // last element
    int vacant = 1;
    int child = 2; // make child = left child
    while (child <= Size) {
        if (child < Size && heap[child + 1] < heap[child]) ++child;
        // right child < left child
        if (!(heap[child] < y)) break;
        heap[vacant] = heap[child]; // move smaller child
        vacant = child; // new vacant
        child = child * 2; // new child of vacant
    }
    heap[vacant] = y;
    return *this;
}

class HuffmanTree {
public:
    // Struct to hold each letter and its frequency
    struct Node {
        char letter;
        int frequency;
        Node* left;
        Node* right;

        Node() : letter('\0'), frequency(0), left(nullptr), right(nullptr) {}
        Node(char l, int f) : letter(l), frequency(f), left(nullptr), right(nullptr) {}

        // Comparison operator overload
        bool operator<(const Node& other) const {
            return frequency < other.frequency;
        }
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

        // Prompt for frequencies if not using default
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

        // Populate the nodes vector
        for (size_t i = 0; i < letters.size(); ++i) {
            nodes.push_back(new Node(letters[i], frequencies[i]));
        }

        // Print the initialized letters and frequencies
        std::cout << "Initialized letters and frequencies:\n";
        for (size_t i = 0; i < letters.size(); ++i) {
            std::cout << letters[i] << ": " << frequencies[i] << "%\n";
        }
    }

    void buildTree() {
        // Create a MinHeap with size equal to the number of nodes
        MinHeap<Node> heap(nodes.size());

        // Insert all nodes into the MinHeap
        for (Node* node : nodes) {
            heap.Insert(*node);
        }

        // Output heap elements to verify
        std::cout << "Nodes added to MinHeap (in frequency order):\n";
        while (heap.Size > 0) {
            Node minNode(' ', 0);
            heap.Delete(minNode);
            std::cout << "Letter: " << minNode.letter << ", Frequency: " << minNode.frequency << "\n";
        }
    }
private:
    std::vector<Node*> nodes;
};

int main() {
    HuffmanTree huffmanTree;
    huffmanTree.initialize();
    huffmanTree.buildTree();
    return 0;
}
