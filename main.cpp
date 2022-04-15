#include <bits/stdc++.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <time.h>
#include <stdlib.h>
using namespace std;
using namespace std::chrono;

// AVL tree node
struct AVLNode {
    struct AVLNode* left;
    struct AVLNode* right;
    int key;
    struct AVLNode* parent;
    int AVLheight;
};

void AdjustHeight(struct AVLNode* root) // Function to adjust height of the AVL tree
{
    if (root != NULL) {

        int val = 1;

        if (root->left != NULL) // Current
            val = root->left->AVLheight + 1;

        if (root->right != NULL)
            val = max(
                val, root->right->AVLheight + 1);

        root->AVLheight = val; // Update
    }
}

struct AVLNode* LLRotation(struct AVLNode* root){ // This is function to perform LL rotation on AVL tree.

    struct AVLNode* tmpnode = root->left; // get reference of left node
    root->left = tmpnode->right; // update with swaping left to right node

    if (tmpnode->right != NULL)
        tmpnode->right->parent = root; // update parent

    tmpnode->right = root; // update right node

    tmpnode->parent = root->parent; // update parent node pointer

    root->parent = tmpnode; // update root

    if (tmpnode->parent != NULL // backtrace and update all
        && root->key < tmpnode->parent->key) {
        tmpnode->parent->left = tmpnode;
    }
    else {
        if (tmpnode->parent != NULL)
            tmpnode->parent->right = tmpnode;
    }

    root = tmpnode;
    // perform height adjustments
    AdjustHeight(root->left); // adjust left node height
    AdjustHeight(root->right); // adjust right node height
    AdjustHeight(root); // check root height
    AdjustHeight(root->parent); 

    return root;
}


struct AVLNode* RRRotation(struct AVLNode* root){ // function to perform RR rotation on AVL tree
    struct AVLNode* tmpnode = root->right; // here we get right node reference as we performing RR rotation

    // After this all actions are same as LL rotation with respect to right node
    root->right = tmpnode->left;

    if (tmpnode->left != NULL)
        tmpnode->left->parent = root;

    tmpnode->left = root;

    tmpnode->parent = root->parent;

    root->parent = tmpnode;

    if (tmpnode->parent != NULL
        && root->key < tmpnode->parent->key) {
        tmpnode->parent->left = tmpnode;
    }
    else {
        if (tmpnode->parent != NULL)
            tmpnode->parent->right = tmpnode;
    }

    root = tmpnode;

    AdjustHeight(root->left); // same as before perform  all height adjustments
    AdjustHeight(root->right);
    AdjustHeight(root);
    AdjustHeight(root->parent);

    return root;
}

struct AVLNode* LRRotation(struct AVLNode* root){ // Function to perform LR rotation
    root->left = RRRotation(root->left); // LR can be achieved by performing RR rotation with referencing left node
    return LLRotation(root);
}

struct AVLNode* RLRotation(struct AVLNode* root){ // Function to perform RL rotation
    root->right = LLRotation(root->right); // RL rotation can be achieved by performing LL rotaion with referencing right node
    return RRRotation(root);
}


struct AVLNode* Insert( struct AVLNode* root, struct AVLNode* parentent, int key){ // Function to insert node to AVL tree

    if (root == NULL) { // base condition 

        root = new struct AVLNode; // create new node     
        // Assigning default values to new node variables
        root->AVLheight = 1;
        root->left = NULL;
        root->right = NULL;
        root->parent = parentent;
        root->key = key;
    
    } // else check if key is less than root

    else if (root->key > key) {
        // if so then insert new key to left tree
        root->left = Insert(root->left,
                            root, key);

        int firstAVLheight = 0;
        int secondAVLheight = 0;

        if (root->left != NULL)
            firstAVLheight = root->left->AVLheight;

        if (root->right != NULL)
            secondAVLheight = root->right->AVLheight;

        // tree balancer logic
        if (abs(firstAVLheight - secondAVLheight) == 2) {

            if (root->left != NULL
                && key < root->left->key) {

                // if key is less than left node then perform LL rotaion
                root = LLRotation(root);
            }
            else {

                // Else LR rotation
                root = LRRotation(root);
            }
        }
    }

    else if (root->key < key) {
        // if new key is greater than root key
        root->right = Insert(root->right, root, key);

        // same logic before that
        int firstAVLheight = 0;
        int secondAVLheight = 0;

        if (root->left != NULL)
            firstAVLheight = root->left->AVLheight;

        if (root->right != NULL)
            secondAVLheight = root->right->AVLheight;

        // check balance
        if (abs(firstAVLheight - secondAVLheight) == 2) {
            if (root->right != NULL
                && key < root->right->key) {

                // Right Left Case
                root = RLRotation(root);
            }
            else {

                // Right Right Case
                root = RRRotation(root);
            }
        }
    } else {
        //if key already exits in tree as root
        cout << "Key already exits.\n";
    }

    // perform height adjustment again
    AdjustHeight(root);

    return root;
}


bool SearchAVLTree(struct AVLNode* root, int key){ // perform search operation on avl
    
    if (root == NULL) // base condition
        return false;

    // If found, return true
    else if (root->key == key){
        cout << "Found Data: " << key << endl;
        return true;
    }
    else if (root->key > key) { // traverse left side tree if key is smaller than root key
        bool val = SearchAVLTree(root->left, key);
        return val;
    }else { // otherwise traverse right side tree
        bool val = SearchAVLTree(root->right, key);
        return val;
    }

}


int main(){
    // this main function is same as main function in BST script as we are just inserting and searching. only the function call changes
    // also this ensures results.    

    srand(time(NULL)); // Initiating the random seed

    string infiles[3] = {"1.txt", "2.txt", "3.txt"}; // input file

    
    float avg_time[3]; // stores avg times 
    float search_avg_times[3];

    
    
    for (int j = 0; j < 3; ++j){ // Iterate through all 3 files
        cout << "\n\nFile: "<< j << endl;
        ifstream input_file; // input file stream

        struct AVLNode* root;
        root = NULL; // Initiate root        
        int inserted_values[100];

        input_file.open(infiles[j]);

        float times[100]; // store time time taken for each insert operation
        
        for (int i = 0; i < 100; ++i){
            
            string temp_num; // as it will be in form of string we need to convert it into integer
            input_file >> temp_num; // read input file data 
            stringstream temp_stream(temp_num);
            int temp_int;
            temp_stream >> temp_int;
            
            auto start = high_resolution_clock::now(); // start time

            if (i == 0){
                root = Insert(root, NULL, temp_int); // make root for 1st element
            }else{
                Insert(root, NULL, temp_int); // insert rest
            }

            auto stop = high_resolution_clock::now(); // end time

            times[i] = duration_cast<microseconds>(stop - start).count(); // total time taken for that input to insert

            inserted_values[i] = temp_int;

        }

        float avg = 0.0;

        for (int i = 0; i < 100; ++i){  // loop to calculate average
            avg = avg + times[i];
        }

        avg_time[j] = avg / 100; // store average



        float search_times[100];
        
        for (int m = 0; m < 100; ++m)
        {
            auto start = high_resolution_clock::now(); // start time

            SearchAVLTree(root, inserted_values[rand() % 100]);

            auto stop = high_resolution_clock::now(); // end time

            search_times[m] = duration_cast<microseconds>(stop - start).count(); // total time taken for that input to insert

        }

        float search_avg = 0.0;

        for (int k = 0; k < 100; ++k)
        {
            search_avg = search_avg + search_times[k];
        }
        search_avg_times[j] = search_avg/100;

    }

    for (int i = 0; i < 3; ++i){
        cout << "\n\nFor input file " << i + 1 << " Average time for insetion is: " << avg_time[i] << endl << "and Average search time: " << search_avg_times[i] << endl ; // print result
    }

    return 0;
}
