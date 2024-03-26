#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
struct Data {
    int id;
    string name;
    double value;

    // Serialize the data to a file
    void serialize(ofstream& ofs) const {
        ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));
        size_t size = name.size();
        ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
        ofs.write(name.c_str(), size);
        ofs.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

    // Deserialize the data from a file
    void deserialize(ifstream& ifs) {
        ifs.read(reinterpret_cast<char*>(&id), sizeof(id));
        size_t size;
        ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
        char buffer[size + 1];
        ifs.read(buffer, size);
        buffer[size] = '\0';
        name = buffer;
        ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
    }

    // Overload less than operator for sorting
    bool operator<(const Data& other) const {
        return id < other.id;
    }
};

int main() {
    // Create sample data
    vector<Data> data = {
        {2, "John", 45.6},
        {1, "Alice", 23.4},
        {3, "Bob", 78.9}
    };

    // Write data to a file
    ofstream outfile("data.txt", ios::binary);
    for (const auto& d : data) {
        d.serialize(outfile);
    }
    outfile.close();

    // Read data from the file and deserialize
    ifstream infile("data.txt", ios::binary);
    vector<Data> readData;
    Data tempData;
    while (!infile.eof()) {
        tempData.deserialize(infile);
        readData.push_back(tempData);
    }
    infile.close();

    // Sort the data
    sort(readData.begin(), readData.end());

    // Write sorted data to another file
    ofstream sortedFile("sorted_data.txt", ios::binary);
    for (const auto& d : readData) {
        d.serialize(sortedFile);
    }
    sortedFile.close();

    cout << "Data sorted and written to 'sorted_data.txt'." <<endl;

    return 0;
}
 