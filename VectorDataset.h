#ifndef VECTOR_DATASET_H
#define VECTOR_DATASET_H
#include <fstream>
#include <sstream>
#include <string>
#include "DataVector.h"

class VectorDataset {
    vector<DataVector> dataset;

public:
    // Constructors and destructor
    VectorDataset() {};
    VectorDataset(int dimension) : dataset(dimension){};
    ~VectorDataset(){dataset.clear();};

    // Data access functions
    // Returns the number of vectors in the VectorDataset.
    // Used to determine the size of the dataset, helpful when iterating through or manipulating the dataset.
    int size() const{
        return dataset.size();
    }
    // Returns a reference to the vector at the specified index in the VectorDataset.
    // Allows users to access individual vectors in the dataset using array-like indexing.
    const DataVector& operator[](int index) const{
        return dataset[index];
    }

    // Function to add a new DataVector to the dataset
    void push_back(const DataVector& newDataVector){
            dataset.push_back(newDataVector);
    }

    // Read dataset from file
    void ReadDataset(const string& filename){
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        cout<<"Reading dataset from file: "<<filename<<endl;

        // Read the entire dataset
        string line;
        getline(file, line); //Reading first line which does not give a data vector (just the name of columns)
        while (getline(file, line))
        {
            stringstream sline(line);
            string value;
            DataVector data;
            while (getline(sline, value, ','))
                data.push_back(stod(value));
            dataset.push_back(data);
        }
        file.close();
    }


    // Function to display the VectorDataset
    void display() const{
        cout << "VectorDataset:" << endl;
        for (int i = 0; i < dataset.size(); ++i) {
            dataset[i].display();
        }
        // for (int i = 0; i < 5; ++i) {
        //     dataset[i].display();
        // }
    }

    void clear(){
        dataset.clear();
    }
   
};

#endif // VECTOR_DATASET_H