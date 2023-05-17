#include <iostream>
#include <vector>
#include <map>
using namespace std;

class MinHeap {
private:
    vector<pair<int, string>> heap_array;

    void up_heapify() {
        int current_index = size() - 1;
        while (current_index > 0) {
            pair<int, string> current_element = heap_array[current_index];
            int parent_index = (current_index - 1) / 2;
            pair<int, string> parent_element = heap_array[parent_index];

            if (parent_element.first < current_element.first) {
                break;
            }
            else {
                heap_array[parent_index] = current_element;
                heap_array[current_index] = parent_element;
                current_index = parent_index;
            }
        }
    }

    void down_heapify() {
        int current_index = 0;
        pair<int, string> current_element = heap_array[0];
        while (current_index < size()) {
            int child_index1 = (current_index * 2) + 1;
            int child_index2 = (current_index * 2) + 2;
            if (child_index1 >= size() && child_index2 >= size()) {
                break;
            }
            else if (child_index2 >= size()) {
                pair<int, string> child_element1 = heap_array[child_index1];
                if (current_element.first < child_element1.first) {
                    break;
                }
                else {
                    heap_array[child_index1] = current_element;
                    heap_array[current_index] = child_element1;
                    current_index = child_index1;
                }
            }
            else {
                pair<int, string> child_element1 = heap_array[child_index1];
                pair<int, string> child_element2 = heap_array[child_index2];
                if (current_element.first < child_element1.first && current_element.first < child_element2.first) {
                    break;
                }
                else {
                    if (child_element1.first < child_element2.first) {
                        heap_array[child_index1] = current_element;
                        heap_array[current_index] = child_element1;
                        current_index = child_index1;
                    }
                    else {
                        heap_array[child_index2] = current_element;
                        heap_array[current_index] = child_element2;
                        current_index = child_index2;
                    }
                }
            }
        }
    }

public:
    MinHeap() {}

    int size() {
        return heap_array.size();
    }

    bool empty() {
        return size() == 0;
    }

    void push(pair<int, string> value) {
        heap_array.push_back(value);
        up_heapify();
    }

    pair<int, string> top() {
        return heap_array[0];
    }

    void pop() {
        if (!empty()) {
            int last_index = size() - 1;
            heap_array[0] = heap_array[last_index];
            heap_array.pop_back();
            down_heapify();
        }
    }
};

class Codec {
private:
    MinHeap heap;
    map<char, int> frequency;
    map<char, string> codes;

    void getCodes(vector<pair<int, string>>& node, string curr_code) {
        if (node[1].empty()) {
            codes[node[0].second] = curr_code;
            return;
        }
        getCodes(node[1][0], curr_code + '0');
        getCodes(node[1][1], curr_code + '1');
    }

    string makeString(vector<pair<int, string>>& node) {
        if (node[1].empty()) {
            return  "'" + node[0].second;
}
return '0' + makeString(node[1][0]) + '1' + makeString(node[1][1]);
}
vector<pair<int, string>> makeTree(string& tree_string, int& index) {
    vector<pair<int, string>> node;
    if (tree_string[index] == '\'') {
        index++;
        node.push_back({ 0, string(1, tree_string[index]) });
        index++;
        return node;
    }
    index++;
    node.push_back(makeTree(tree_string, index));
    index++;
    node.push_back(makeTree(tree_string, index));
    return node;
}
public:
pair<string, string> encode(string data) {
for (char ch : data) {
frequency[ch]++;
}
if (frequency.size() == 0) {
string final_string = "zer#";
string output_message = "Compression complete and file will be downloaded automatically.\nCompression Ratio: " + to_string(data.length() / final_string.length());
return { final_string, output_message };
}
    if (frequency.size() == 1) {
        char key;
        int value;
        for (auto it = frequency.begin(); it != frequency.end(); ++it) {
            key = it->first;
            value = it->second;
        }
        string final_string = "one#" + string(1, key) + '#' + to_string(value);
        string output_message = "Compression complete and file will be downloaded automatically.\nCompression Ratio: " + to_string(data.length() / final_string.length());
        return { final_string, output_message };
    }

    for (auto it = frequency.begin(); it != frequency.end(); ++it) {
        heap.push({ it->second, string(1, it->first) });
    }

    while (heap.size() >= 2) {
        vector<pair<int, string>> min_node1 = heap.top();
        heap.pop();
        vector<pair<int, string>> min_node2 = heap.top();
        heap.pop();
        heap.push({ min_node1[0] + min_node2[0], { min_node1, min_node2 } });
    }

    vector<pair<int, string>> huffman_tree = heap.top();
    heap.pop();
    getCodes(huffman_tree, "");

    string binary_string = "";
    for (char ch : data) {
        binary_string += codes[ch];
    }

    int padding_length = (8 - (binary_string.length() % 8)) % 8;
    for (int i = 0; i < padding_length; i++) {
        binary_string += '0';
    }

    string encoded_data = "";
    for (int i = 0; i < binary_string.length(); i += 8) {
        string curr_byte = binary_string.substr(i, 8);
        int curr_num = stoi(curr_byte, nullptr, 2);
        encoded_data += static_cast<char>(curr_num);
    }

    string tree_string = makeString(huffman_tree);
    int ts_length = tree_string.length();
    string final_string = to_string(ts_length) + '#' + to_string(padding_length) + '#' + tree_string + encoded_data;
    string output_message = "Compression complete and file will be downloaded automatically.\nCompression Ratio: " + to_string(data.length() / final_string.length());
    return { final_string, output_message };
}

pair<string, string> decode(string data) {
    int k = 0;
    string temp = "";
    while (k < data.length() && data[k] != '#') {
        temp += data[k];
        k++;
    }
    if (k== data.length()) {
string output_message = "Invalid File! Please submit a valid De-Compressed file";
return { "", output_message };
}
if (temp == "zer") {
string decoded_data = "";
string output_message = "De-Compression complete and file will be downloaded automatically.";
return { decoded_data, output_message };
}
if (temp == "one") {
data = data.substr(k + 1);
k = 0;
temp = "";
while (data[k] != '#') {
temp += data[k];
k++;
}
char one_char = temp[0];
data = data.substr(k + 1);
int str_len = stoi(data);
string decoded_data = string(str_len, one_char);
string output_message = "De-Compression complete and file will be downloaded automatically.";
return { decoded_data, output_message };
}
data = data.substr(k + 1);
int ts_length = stoi(temp);
k = 0;
temp = "";
while (data[k] != '#') {
temp += data[k];
k++;
}
data = data.substr(k + 1);
int padding_length = stoi(temp);
temp = data.substr(0, ts_length);
data = data.substr(ts_length);
string tree_string = temp;
temp = data;
string encoded_data = temp;
int index = 0;
vector<pair<int, string>> huffman_tree = makeTree(tree_string, index);
    string binary_string = "";
    for (char ch : encoded_data) {
        bitset<8> bits(ch);
        binary_string += bits.to_string();
    }
    binary_string = binary_string.substr(0, binary_string.length() - padding_length);

    string decoded_data = "";
    vector<pair<int, string>> node = huffman_tree;
    for (char bit : binary_string) {
        if (bit == '1') {
            node = node[1];
        }
        else {
            node = node[0];
        }

        if (node[0].first == 0) {
            decoded_data += node[0].second;
            node = huffman_tree;
        }
    }

    string output_message = "De-Compression complete and file will be downloaded automatically.";
    return { decoded_data, output_message };
}
};
