#include <iostream>
#include <fstream>
#include <string>
//#include <algorithm> // for swap()
using namespace std;

class Node {
public:
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char d, int freq){
        data = d;
        frequency = freq;
        left = nullptr;
        right = nullptr;
        }
};

class DecoderTree {
public:
    Node* root;

    DecoderTree(){
        root = nullptr;
    }


    // Reconstructing the Huffman tree using the codes from "codes.txt"
    void reconstructTreeFromFile(const string& filename) {
        ifstream codesFile(filename);
        if (!codesFile.is_open()) {
            cerr << "Error opening codes file.\n";
            return;
    }

        // Now Reconstructing the tree based on the codes in "codes.txt"
        // Parsing each line to get the character and its code
        string line;
        while (getline(codesFile, line)) {
            char character;
            string code;
            if (line.length() > 3) { //Now Ensurimg the line has necessary data (character, code)
                character = line[0];
                code = line.substr(3); // here i am Extracting the code part of the line

                //Now Building the Huffman tree using the code and character
                buildTreeFromCode(character, code);
            }
        }
        codesFile.close();
    }


    //Building the Huffman tree based on the character and its code
    void buildTreeFromCode(char character, const string& code) {
        if (root == nullptr) {
            root = new Node('|', 0); // Creating just a dummy root node
        }
        Node* current = root;
        for (char bit : code) {
            if (bit == '0') {
                if (current->left == nullptr) {
                    current->left = new Node('|', 0);
                }
                current = current->left;
            } else if (bit == '1') {
                if (current->right == nullptr) {
                    current->right = new Node('|', 0);
                }
                current = current->right;
            }
        }
        current->data = character;
    }


    //Now finally Decoding the encoded text based on the reconstructed Huffman tree
    string decodeText(const string& encodedText) {
        string decodedText = "";
        Node* current = root;
        for (char bit : encodedText) {
            if (bit == '0') {
                current = current->left;
            } else if (bit == '1') {
                current = current->right;
            }

            if (current->left == nullptr && current->right == nullptr) {
                decodedText += current->data;
                current = root; // Reseting back to the root for the next character
            }
        }
        return decodedText;
    }

};

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}


string readEncodedText(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}


int main() {

DecoderTree decoderTree;

    // Reconstructing the Huffman tree using the codes from "codes.txt"
    decoderTree.reconstructTreeFromFile("codes.txt");

    //Reading the encoded text from "output.txt"
    string encodedText = readEncodedText("output.txt");

    // Decoding the encoded text using the reconstructed decoder tree
    string decodedText = decoderTree.decodeText(encodedText);

    // Now Saving the decoded text to "decoded.txt" and display it
    ofstream decodedFile("decoded.txt");
    if (decodedFile.is_open()) {
        decodedFile << decodedText;
        decodedFile.close();
        cout << "Decoded text saved to 'decoded.txt'. Decoded Text: \n" << decodedText << endl;
    } else {
        cerr << "Unable to open the decoded file.\n";
    }

    return 0;
}