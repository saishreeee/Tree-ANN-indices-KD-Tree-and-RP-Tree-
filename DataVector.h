#ifndef DATA_VECTOR_H
#define DATA_VECTOR_H
#include <ctime>
#include<vector>
#include<iostream>
#include<cmath>
using namespace std;

class DataVector {
    vector<double> v;

public:
    // Constructor, destructor, and setDimension function
    DataVector(int dimension = 0);
    ~DataVector();
    DataVector(const DataVector &other);
    DataVector &operator=(const DataVector &other);
    void setDimension(int dimension = 0);
    void push_back(double value);
    int size() const;
    double operator[](int i) const;

    // Operators for vector addition and subtraction
    DataVector operator+(const DataVector &other) const;
    DataVector operator-(const DataVector &other) const;

    // Operator for computing the dot product
    double operator*(const DataVector &other) const;

    // Member functions for norm and distance calculations
    double norm() const;
    double dist(const DataVector &other) const;

    // Function to display the DataVector
    void display() const;

    void random_unit(int d);

    //Overloaded >> operator to input the vector
    friend istream& operator>>(istream& in, DataVector& v);
};

#endif // DATA_VECTOR_H