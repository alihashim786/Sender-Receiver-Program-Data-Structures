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

class EncoderTree {
public:
    Node* root;
    string codes[256];


    Node* mergeNodes(Node* left, Node* right) {
        int frequencySum = left->frequency + right->frequency;
        Node* mergedNode = new Node('|', frequencySum); // Use a special character for internal nodes
        mergedNode->left = left;
        mergedNode->right = right;
        return mergedNode;
    }


    void assignCodesHelper(Node* root, string code) {
        if (!root) return;

        if (root->data != '|') {
            codes[static_cast<unsigned char>(root->data)] = code;
        }

        assignCodesHelper(root->left, code + '0');
        assignCodesHelper(root->right, code + '1');
    }


    EncoderTree(const string& text) : root(nullptr) {
        int charFreq[256] = {0};
        for (char c : text) { //itterates at every character
        //The unsigned char allows to hold larger positive values than a regular char,which uses one bit for sign representation.
            charFreq[static_cast<unsigned char>(c)]++; //converting normal char to unsigned char
            //++ shows us to move to next index
        }

        Node* nodes[256];
        int count = 0;
        for (int i = 0; i < 256; ++i) {
            if (charFreq[i] != 0) {
                //Let's say count initially holds the value 3.
                // If we use nodes[count++], it will access nodes[3] and then increment count to 4.
                nodes[count++] = new Node(static_cast<char>(i), charFreq[i]); //nodes class constructor will be called here
            }
            
        }

        while (count > 1) {
            int min1 = 0, min2 = 1;
            if (nodes[min1]->frequency > nodes[min2]->frequency) {
                swap(min1, min2);
            }

            for (int i = 2; i < count; ++i) {
                if (nodes[i]->frequency < nodes[min1]->frequency) {
                    min2 = min1;
                    min1 = i;
                } else if (nodes[i]->frequency < nodes[min2]->frequency) {
                    min2 = i;
                }
            }

            Node* mergedNode = mergeNodes(nodes[min1], nodes[min2]);
            //now we will ignore the least nodes and now we will consider new nodes as a part of tree.
            /*IMPLEMENTED STEP 4 Point 5 AND 6 HERE 
            5. Exclude the left and right nodes from the merging process
            6. The new node can be selected for merging now.*/
            nodes[min1] = mergedNode; 
            nodes[min2] = nodes[count - 1];
            count--;
        }

        root = nodes[0];
        assignCodesHelper(root, "");
    }


    string encodeText(const string& text) {
        string encodedText = "";
        for (char c : text) {
            encodedText += codes[static_cast<unsigned char>(c)];
        }
        return encodedText;
    }


    void saveCodesToFile(const string& filename) {
        ofstream codesFile(filename);
        if (codesFile.is_open()) {
            for (int i = 0; i < 256; ++i) {
                if (!codes[i].empty()) {
                    codesFile << static_cast<char>(i) << ": " << codes[i] << '\n';
                }
            }
            codesFile.close();
            cout << "Characters Codes saved to '" << filename << "'.\n";
        } else {
            cerr << "Unable to open the codes file.\n";
        }
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

void saveEncodedText(const string& encodedText, const string& filename) {
    ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << encodedText;
        outputFile.close();
        cout << "Encoded text saved to '" << filename << "'.\n";
    } else {
        cerr << "Unable to open the output file.\n";
    }
}

int main() {
    string filename = "input.txt";
    string fileContent = readFile(filename);
    if (fileContent.empty()) {
        cerr << "File is empty.\n";
        return 1;
    }

    EncoderTree encoderTree(fileContent);

    string encodedText = encoderTree.encodeText(fileContent);
    saveEncodedText(encodedText, "output.txt");

    encoderTree.saveCodesToFile("codes.txt");

    return 0;
}
