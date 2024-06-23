#include "TreeIndex.h"

KDTreeIndex* KDTreeIndex::GetInstance(){
            if (!instance)
                instance = new KDTreeIndex();
            return instance;
        }

void KDTreeIndex::MakeTree(VectorDataset& dataset){
    build(dataset);
    //print(root_,0);
}

void KDTreeIndex::Search(const DataVector& query, int k){
    vector<int>ans(k);
    priority_queue<pdi>knnheap;
    knnSearchRecursive(query, root_, knnheap, k); int j=k-1;
    while(!knnheap.empty()){
        ans[j]=knnheap.top().second; //cout<<ans[j]<<endl;
        knnheap.pop(); j--;
    }
    for(int i=0; i<k; i++) {
        cout<<ans[i]<<endl;
        //dataset_[ans[i]].display();
    }
}

void KDTreeIndex::Print(){
    print(root_,0);
}

KDTreeIndex::KDTreeIndex():root_(nullptr){}; 
KDTreeIndex::KDTreeIndex(const VectorDataset& dataset):root_(nullptr){build(dataset);}; 
KDTreeIndex::~KDTreeIndex() {clear();}; 

void KDTreeIndex::Insert(const DataVector& point){
    dataset_.push_back(point);
    int index = dataset_.size()-1;
    Node* node = root_;
    int depth = 0;
    while(node != NULL){
        int axis = depth % point.size();
        if(point[axis] < dataset_[node->idx][axis]){
            if(node->left== NULL){
                node->left = new Node();
                node->left->idx = index;
                node->left->axis = axis;
                break;
            }
            node = node->left;
        }else{
            if(node->right == NULL){
                node->right = new Node();
                node->right->idx = index;
                node->right->axis = axis;
                break;
            }
            node = node->right;
        }
        depth++;
    }
}

void KDTreeIndex::Delete(const DataVector& point){
    Node* node = root_;
    Node* parent = NULL;
    int depth = 0;
    while(node != NULL){
        int axis = depth % point.size();
        if(point[axis] == dataset_[node->idx][axis]){
            break;
        }
        parent = node;
        if(point[axis] < dataset_[node->idx][axis]){
            node = node->left;
        }else{
            node = node->right;
        }
        depth++;
    }
    if(node == NULL){
        return;
    }
    if(node->left == NULL && node->right == NULL){
        if(parent == NULL){
            root_ = NULL;
        }else if(parent->left == node){
            parent->left = NULL;
        }else{
            parent->right = NULL;
        }
        delete node;
    }else if(node->left == NULL){
        if(parent == NULL){
            root_ = node->right;
        }else if(parent->left == node){
            parent->left = node->right;
        }else{
            parent->right = node->right;
        }
        delete node;
    }else if(node->right == NULL){
        if(parent == NULL){
            root_ = node->left;
        }else if(parent->left == node){
            parent->left = node->left;
        }else{
            parent->right = node->left;
        }
        delete node;
    }else{
        Node* successor = node->right;
        Node* successorParent = node;
        while(successor->left != NULL){
            successorParent = successor;
            successor = successor->left;
        }
        node->idx = successor->idx;
        if(successorParent->left == successor){
            successorParent->left = successor->right;
        }else{
            successorParent->right = successor->right;
        }
        delete successor;
    }
}

void KDTreeIndex::build(const VectorDataset& dataset){
	clear();
    dataset_=dataset;
    vector<int>indices(dataset.size());
    iota(begin(indices),end(indices),0);
    root_=buildRecursive(indices.data(),dataset.size(),0);
}

Node* KDTreeIndex::buildRecursive(int* indices, int size, int depth){
	if (size <= 0)
		return nullptr;
    int d=dataset_[0].size();
	int axis=depth%d;   // ChooseRule: choosing a coordinate direction i
    int mid=(size-1)/2;
	nth_element(indices, indices+mid, indices+size, [&](int a, int b)
	{
		return dataset_[a][axis] < dataset_[b][axis];
	});
	Node* newnode=new Node();
	newnode->idx=indices[mid];  //cout<<"Level "<<depth<<": "<<newnode->idx<<endl;
	newnode->axis=axis;
	newnode->left=buildRecursive(indices, mid, depth + 1);
	newnode->right=buildRecursive(indices + mid + 1, size - mid - 1, depth + 1);
	return newnode;
}

void KDTreeIndex::clear(){ 
	clearRecursive(root_);
	root_ = nullptr;
	dataset_.clear();
}

void KDTreeIndex::clearRecursive(Node* node){
	if (node==nullptr) return;
	if (node->left) clearRecursive(node->left);
	if (node->right) clearRecursive(node->right);
	delete node;
}

void KDTreeIndex::knnSearchRecursive(const DataVector& query, const Node* node, priority_queue<pdi>& knnheap, int k) const{
	if (node == nullptr)
		return;

	const DataVector& train=dataset_[node->idx];
	const double dist = query.dist(train);
    if(knnheap.size()>=k && dist<knnheap.top().first) knnheap.pop();
	if(knnheap.size()<k) knnheap.push(make_pair(dist, node->idx));

	const int axis=node->axis;
	const int dir=query[axis]<train[axis]?0:1;
    if(dir) knnSearchRecursive(query, node->right, knnheap, k);
    else knnSearchRecursive(query, node->left, knnheap, k);

	const double diff = fabs(query[axis]-train[axis]);
	if((int)knnheap.size()<k||diff<knnheap.top().first){
        if(dir) knnSearchRecursive(query, node->left, knnheap, k);
        else knnSearchRecursive(query, node->right, knnheap, k);
    }
}

void KDTreeIndex::print(Node* n, int d){
    if(n->left) print(n->left, d+1);    
    for (int i = 0; i < d; i++) cout<<"    ";   
    cout<<"Level "<<d<<": "<<n->idx<<endl;
    if(n->right) print(n->right, d+1);
}
 
RPTreeIndex* RPTreeIndex::GetInstance(){
    if (!instance) instance = new RPTreeIndex();
    return instance;
}

void RPTreeIndex::MakeTree(VectorDataset& dataset){
    //cout<<"make tree"<<endl;
    build(dataset);
    //print(root_,0);
}

void RPTreeIndex::Search(const DataVector& query, int k){
    vector<int>ans(k);
    priority_queue<pdi>knnheap;
    knnSearchRecursive(query, root_, knnheap, k); int j=k-1;
    while(!knnheap.empty()){
        ans[j]=knnheap.top().second; //cout<<ans[j]<<endl;
        knnheap.pop(); j--;
    }
    for(int i=0; i<k; i++) {
        cout<<ans[i]<<endl;
        //dataset_[ans[i]].display();
    }
}

void RPTreeIndex::Print(){
    print(root_,0);
}

RPTreeIndex::RPTreeIndex():root_(nullptr){}; 
RPTreeIndex::RPTreeIndex(const VectorDataset& dataset):root_(nullptr){build(dataset);}; 
RPTreeIndex::~RPTreeIndex() {clear();}; 

void RPTreeIndex::build(const VectorDataset& dataset){   
    //cout<<"build\n";
	clear();
    dataset_=dataset;
    vector<int>indices(dataset.size());
    iota(begin(indices),end(indices),0);
    root_=buildRecursive(indices.data(),dataset.size(),0);
}

RPNode* RPTreeIndex::buildRecursive(int* indices, int size, int depth){
    if (size <= 0)
		return nullptr;

    int dim=dataset_[0].size();
    RPNode* newRPNode=new RPNode();
	newRPNode->rule.random_unit(dim);   // ChooseRule: choose random unit direction v ∈ Rᴰ
    int mid=(size-1)/2;
	nth_element(indices, indices+mid, indices+size, [&](int a, int b)
	{
		return (dataset_[a]*newRPNode->rule) < (dataset_[b]*newRPNode->rule);
	});
	newRPNode->idx=indices[mid];    //cout<<"Level "<<depth<<": "<<newRPNode->idx<<endl;
	newRPNode->median=dataset_[newRPNode->idx]*newRPNode->rule;    // median(z·v: z ∈ S) 

    int x=indices[rand()%size];  // pick any x ∈ S
    double farthest=-1, ff; int y=0;
    for(int i=0; i<size; i++){ 
        ff=dataset_[x].dist(dataset_[indices[i]]);
        if(ff>=farthest){
            farthest=ff;
            y=indices[i];
        }
    }
    // y ∈ S is the farthest point from x
    // choose delta uniformly at random in [-1,1]·6||x-y||/sqrt(D)
    newRPNode->delta=(((float)rand()/RAND_MAX)*2-1)*6*(dataset_[x].dist(dataset_[y]))/sqrt(dim);

    vector<int>left, right;
    for(int i=0; i<size; i++){ 
        if(indices[i]==newRPNode->idx) continue;
        if((dataset_[indices[i]]*newRPNode->rule) <= (newRPNode->median + newRPNode->delta)) left.push_back(indices[i]);
        else right.push_back(indices[i]);
    }
	newRPNode->left=buildRecursive(left.data(), left.size(), depth+1);
	newRPNode->right=buildRecursive(right.data(), right.size(), depth+1);
	
    return newRPNode;
}

void RPTreeIndex::clear(){ 
	clearRecursive(root_);
	root_ = nullptr;
	dataset_.clear();
}

void RPTreeIndex::clearRecursive(RPNode* RPNode){
	if (RPNode==nullptr) return;
	if (RPNode->left)
		clearRecursive(RPNode->left);
	if (RPNode->right)
		clearRecursive(RPNode->right);
	delete RPNode;
}

void RPTreeIndex::knnSearchRecursive(const DataVector& query, const RPNode* RPnode, priority_queue<pdi>& knnheap, int k) const{
    if (RPnode == nullptr)
		return;

	const DataVector& train=dataset_[RPnode->idx];
	const double dist = query.dist(train);
    if(knnheap.size()>=k && dist<knnheap.top().first) knnheap.pop();
	if(knnheap.size()<k) knnheap.push(make_pair(dist, RPnode->idx));

    const double dot=query*RPnode->rule;
	const int dir= (dot <= (RPnode->median + RPnode->delta)) ?0:1;
    if(dir) knnSearchRecursive(query, RPnode->right, knnheap, k);
    else knnSearchRecursive(query, RPnode->left, knnheap, k);

	const double diff = fabs(dot-RPnode->median);
	if((int)knnheap.size()<k||diff<knnheap.top().first){
        if(dir) knnSearchRecursive(query, RPnode->left, knnheap, k);
        else knnSearchRecursive(query, RPnode->right, knnheap, k);
    }
}

void RPTreeIndex::print(RPNode* n, int d){
    cout<<"Level "<<d<<": "<<n->idx<<endl;
    if(n->left) print(n->left, d+1);
    if(n->right) print(n->right, d+1);
}

void RPTreeIndex::Insert(const DataVector& point){
    dataset_.push_back(point);
    int index = dataset_.size()-1;
    RPNode* node = root_;
    int depth = 0;
    while(node != NULL){
        int axis = depth % point.size();
        if(point*node->rule <= node->median + node->delta){
            if(node->left == NULL){
                node->left = new RPNode();
                node->left->idx = index;
                node->left->rule.random_unit(point.size());
                break;
            }
            node = node->left;
        }else{
            if(node->right == NULL){
                node->right = new RPNode();
                node->right->idx = index;
                node->right->rule.random_unit(point.size());
                break;
            }
            node = node->right;
        }
        depth++;
    }
}

void RPTreeIndex::Delete(const DataVector& point){
    RPNode* node = root_;
    RPNode* parent = NULL;
    int depth = 0;
    while(node != NULL){
        if(node->idx == -1){
            break;
        }
        if(point*node->rule == dataset_[node->idx]*node->rule){
            break;
        }
        parent = node;
        if(point*node->rule <= node->median + node->delta){
            node = node->left;
        }else{
            node = node->right;
        }
        depth++;
    }
    if(node == NULL){
        return;
    }
    if(node->left == NULL && node->right == NULL){
        if(parent == NULL){
            root_ = NULL;
        }else if(parent->left == node){
            parent->left = NULL;
        }else{
            parent->right = NULL;
        }
        delete node;
    }else if(node->left == NULL){
        if(parent == NULL){
            root_ = node->right;
        }else if(parent->left == node){
            parent->left = node->right;
        }else{
            parent->right = node->right;
        }
        delete node;
    }else if(node->right == NULL){
        if(parent == NULL){
            root_ = node->left;
        }else if(parent->left == node){
            parent->left = node->left;
        }else{
            parent->right = node->left;
        }
        delete node;
    }else{
        RPNode* successor = node->right;
        RPNode* successorParent = node;
        while(successor->left != NULL){
            successorParent = successor;
            successor = successor->left;
        }
        node->idx = successor->idx;
        if(successorParent->left == successor){
            successorParent->left = successor->right;
        }else{
            successorParent->right = successor->right;
        }
        delete successor;
    }
}


KDTreeIndex *KDTreeIndex::instance = nullptr;
RPTreeIndex *RPTreeIndex::instance = nullptr;

int main(){
    // int k;
    // cout<<"Enter k:";
    // cin>>k;
    VectorDataset dataset;
    dataset.ReadDataset("train.csv");
    //dataset.display();

    VectorDataset test;
    test.ReadDataset("train copy.csv");
    // test.display();

    // auto start=chrono::high_resolution_clock::now();
    // KDTreeIndex::GetInstance()->MakeTree(dataset);
    // for (int i = 0; i < 1; i++)
    // {
    //     KDTreeIndex::GetInstance()->Search(test[i],k);
    //     cout<<"-----"<<endl;
    // }
    // auto end=chrono::high_resolution_clock::now();
    // chrono::duration<double> duration = end - start;
    // cout<<"Time taken by KD Tree: "<<duration.count()<<" seconds"<<endl;

    // start=chrono::high_resolution_clock::now();
    // RPTreeIndex::GetInstance()->MakeTree(dataset);
    // for (int i = 0; i < 1; i++)
    // {
    //     RPTreeIndex::GetInstance()->Search(test[i],k);
    //     cout<<"-----"<<endl;
    // }
    // end=chrono::high_resolution_clock::now();
    // duration = end - start;
    // cout<<"Time taken RP Tree: "<<duration.count()<<" seconds"<<endl;

    cout<<"Options to build the Tree Index"<<endl;
    cout<<"1. KDTree"<<endl;
    cout<<"2. RPtree"<<endl;
    cout<<"Enter your choice: ";
    int choice;
    cin>>choice;
    if(choice == 1){
        KDTreeIndex* kdTree = KDTreeIndex::GetInstance();
        kdTree->MakeTree(dataset);
        while(1){
            int operation;
            cout<<"1. Search for kNN"<<endl;
            cout<<"2. Insert a point"<<endl;
            cout<<"3. Delete a point"<<endl;
            cout<<"4. Print the KDTree"<<endl;
            cout<<"5. Exit"<<endl;
            cout<<"Enter your choice: ";
            cin>>operation;
            if(operation == 1){
                int k;
                cout<<"Enter the value of k: ";
                cin>>k;
                for(int i = 0; i < 1; i++){
                    kdTree->Search(test[i], k);
                }
            }
            else if(operation == 2){
                DataVector point(dataset[0].size());
                cout<<"Enter the point to be inserted: ";
                cin>>point;
                kdTree->Insert(point);
            }
            else if(operation == 3){
                DataVector point(dataset[0].size());
                cout<<"Enter the point to be deleted: ";
                cin>>point;
                kdTree->Delete(point);
            }
            else if(operation == 4){
                kdTree->Print();
            }
            else if(operation == 5){
                break;
            }
        }
    }
    else{
        RPTreeIndex* rpTree = RPTreeIndex::GetInstance();
        rpTree->MakeTree(dataset);
        while(1){
            int operation;
            cout<<"1. Search for kNN"<<endl;
            //cout<<"2. Insert a point"<<endl;
            //cout<<"3. Delete a point"<<endl;
            cout<<"2. Print the RPTree"<<endl;
            cout<<"3. Exit"<<endl;
            cout<<"Enter your choice: ";
            cin>>operation;
            if(operation == 1){
                int k;
                cout<<"Enter the value of k: ";
                cin>>k;
                for(int i = 0; i < 1; i++){
                    rpTree->Search(test[i], k);
                }
            }
            // else if(operation == 2){
            //     DataVector point(dataset[0].size());
            //     cout<<"Enter the point to be inserted: ";
            //     cin>>point;
            //     rpTree->Insert(point);
            // }
            // else if(operation == 3){
            //     DataVector point(dataset[0].size());
            //     cout<<"Enter the point to be deleted: ";
            //     cin>>point;
            //     rpTree->Delete(point);
            // }
            else if(operation == 2){
                rpTree->Print();
            }
            else if(operation == 3){
                break;
            }
        }
    }    
    return 0;
}