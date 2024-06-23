#include "DataVector.h"

// Constructor
DataVector::DataVector(int dimension) : v(dimension, 0.0) {}

// Destructor
DataVector::~DataVector() {v.clear();}

// Copy constructor
DataVector::DataVector(const DataVector &other) : v(other.v) {}

// Copy assignment operator
DataVector &DataVector::operator=(const DataVector &other) {
    if (this != &other) {
        v = other.v;
    }
    return *this;
}

// Set dimension function
void DataVector::setDimension(int dimension) {
    v.clear();
    v.resize(dimension, 0.0);
}

// Pushback values into the DataVector
void DataVector::push_back(double value){
    v.push_back(value);
}

int DataVector::size() const{
        return v.size();
    }

double DataVector::operator[](int i) const {
    // Check if the index is valid
    if (i < 0 || i >= v.size()) {
        cerr << "Index out of range!" << endl;
        return 0.0;
    }
    return v[i];
}

// Operator+ for vector addition
DataVector DataVector::operator+(const DataVector &other) const{
    DataVector result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result.v[i] = v[i] + other.v[i];
    }
    return result;
}

// Operator- for vector subtraction
DataVector DataVector::operator-(const DataVector &other) const{
    DataVector result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result.v[i] = v[i] - other.v[i];
    }
    return result;
}

// Operator* for dot product
double DataVector::operator*(const DataVector &other) const{
    double result = 0.0;
    for (int i = 0; i < v.size(); ++i) {
        result += v[i] * other.v[i];
    }
    return result;
}

// Member function for norm calculation
double DataVector::norm() const{
    return sqrt((*this) * (*this));
}

// Member function for distance calculation
double DataVector::dist(const DataVector &other) const{
    DataVector diff = (*this) - other;
    return diff.norm();
}

void DataVector::display() const {
    cout << "DataVector: ";
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    // for (int i = 10; i < 15; ++i) { 
    //     cout << v[i] << " ";
    // }
    cout << endl;
}

void DataVector::random_unit(int dim)
{
    srand(time(NULL));
    v.clear();
    v.resize(dim);
    double x=0;
    do {
        for(int i=0;i<dim;i++){
            v[i]=(float)rand()/RAND_MAX;
            x += v[i]*v[i];
        }
    }while(x==0);
    x=sqrt(x);
    for(int i=0;i<dim;i++) v[i]/=x;
    return;
}

//Overloaded >> operator to input the vector
istream& operator>>(istream& in, DataVector& v){
    for (int i = 0; i < v.size(); i++){
        in >> v.v[i];
    }
    return in;
}
