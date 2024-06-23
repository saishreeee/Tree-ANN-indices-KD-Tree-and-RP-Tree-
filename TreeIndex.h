#include <bits/stdc++.h>
#include "DataVector.h"
#include "VectorDataset.h"
using namespace std;
# define ll long long
# define pdi pair<double,int>

struct Node{
    int idx;
    Node *left, *right;
    int axis;
    Node() : idx(-1), axis(-1) {left = right = nullptr;}
};

struct RPNode{
    DataVector rule;
    int idx;
    double delta;
    RPNode *left, *right;
    double median;
};

class TreeIndex { 
    protected: 
        TreeIndex() {} 
        ~TreeIndex() {} 
    public: 
        static TreeIndex& GetInstance(); 
}; 

class KDTreeIndex : public TreeIndex { 
    Node *root_;
    VectorDataset dataset_;

    public:  
        static KDTreeIndex *GetInstance();

        void MakeTree(VectorDataset&);

        void Search(const DataVector&, int);

        void Insert(const DataVector& point);

        void Delete(const DataVector& point);

        void Print();

    private: 
        KDTreeIndex(); 
        KDTreeIndex(const VectorDataset&); 
        ~KDTreeIndex(); 

        static KDTreeIndex *instance;

        void build(const VectorDataset&);

        Node* buildRecursive(int*, int, int);
	
        void clear();

        void clearRecursive(Node*);

        void knnSearchRecursive(const DataVector&, const Node*, priority_queue<pdi>&, int) const;

        void print(Node*, int);
}; 

class RPTreeIndex : public TreeIndex { 
    RPNode *root_;
    VectorDataset dataset_;

    public:  
        static RPTreeIndex *GetInstance();

        void MakeTree(VectorDataset&);

        void Search(const DataVector&, int);

        void Insert(const DataVector& point);
        
        void Delete(const DataVector& point);

        void Print();

    private: 
        RPTreeIndex(); 
        RPTreeIndex(const VectorDataset&); 
        ~RPTreeIndex(); 

        static RPTreeIndex *instance;

        void build(const VectorDataset&);

        RPNode* buildRecursive(int*, int, int);

        void clear();

        void clearRecursive(RPNode* RPNode);

        void knnSearchRecursive(const DataVector&, const RPNode*, priority_queue<pdi>&, int) const;

        void print(RPNode* n, int d);   
}; 