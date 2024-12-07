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

// MinHeap definition (from course website, I modified it for this particular project)
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
    void printHeap() const;  // New method to print the heap
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
        while (i != 1 && x->frequency < heap[i / 2]->frequency) {
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
        // Compare nodes based on frequency directly
        if (child < Size && heap[child + 1]->frequency < heap[child]->frequency) ++child;
        if (!(heap[child]->frequency < y->frequency)) break;
        heap[vacant] = heap[child]; // move smaller child
        vacant = child; // new vacant
        child = child * 2; // new child of vacant
    }
    heap[vacant] = y;
    return *this;
}

template <class T>
void MinHeap<T>::printHeap() const {
    if (Size == 0) {
        std::cout << "Heap is empty.\n";
        return;
    }

    std::cout << "Heap elements (Letter, Frequency): ";
    for (int i = 1; i <= Size; ++i) {
        std::cout << "(" << heap[i]->letter << ", " << heap[i]->frequency << ") ";
    }
    std::cout << std::endl;
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
        MinHeap<Node*> heap(nodes.size());

        // Insert all nodes into the MinHeap
        for (Node* node : nodes) {
            heap.Insert(node);
        }

        // Build the Huffman Tree
        while (heap.Size > 1) {
            heap.printHeap();

            // Delete the two smallest nodes from the heap
            Node* leftNode = nullptr;
            Node* rightNode = nullptr;
            heap.Delete(leftNode);
            heap.Delete(rightNode);

            std::cout << "Deleted nodes: (" << leftNode->letter << ", " << leftNode->frequency << ") "
                << "and (" << rightNode->letter << ", " << rightNode->frequency << ")\n";

            // Create a new parent node with the combined frequency
            Node* parentNode = new Node();
            parentNode->frequency = leftNode->frequency + rightNode->frequency;
            parentNode->left = leftNode;
            parentNode->right = rightNode;

            std::cout << "Inserted parent node: (" << parentNode->frequency << ")\n";

            // Insert the parent node back into the heap
            heap.Insert(parentNode);
        }

        // The final node is the root of the Huffman Tree
        heap.Delete(this->root);

        std::cout << "Huffman Tree built successfully. Root frequency: " << root->frequency << "\n";
    }

    void viewCodes() {
        if (!root) {
            std::cout << "Huffman Tree has not been built yet. Please run 'buildTree' first.\n";
            return;
        }

        std::cout << "Huffman Codes for each letter:\n";
        std::vector<std::string> codes(nodes.size(), ""); 

        // Traverse the tree and generate the codes for each letter
        generateCodes(root, "", codes);

        // Output the codes for each letter in the order the letters were provided
        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << nodes[i]->letter << ": "
                << (codes[i].empty() ? "Not assigned" : codes[i]) << "\n";
        }
    }

    ~HuffmanTree() {
        for (Node* node : nodes) {
            delete node;
        }
    }

private:
    Node* root = nullptr; 
    std::vector<Node*> nodes;

    void generateCodes(Node* node, const std::string& currentCode, std::vector<std::string>& codes) {
        if (!node) return;

        // If the node is a leaf (it has no children), store its code
        if (!node->left && !node->right) {
            // Find the index based on the position in the nodes list (order of input letters)
            auto it = std::find_if(nodes.begin(), nodes.end(), [node](Node* n) {
                return n->letter == node->letter;
                });
            if (it != nodes.end()) {
                size_t index = std::distance(nodes.begin(), it);
                codes[index] = currentCode;  
            }
        }

        // Traverse left (add '0' to the code)
        generateCodes(node->left, currentCode + "0", codes);

        // Traverse right (add '1' to the code)
        generateCodes(node->right, currentCode + "1", codes);
    }
};

int main() {
    HuffmanTree huffmanTree;
    huffmanTree.initialize();
    huffmanTree.buildTree();
    huffmanTree.viewCodes();

    return 0;
}
