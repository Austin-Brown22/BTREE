#include <iostream>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

class Node{
    public:
    int num;
    Node* next;
    Node* parent;

    Node(){
        next = nullptr;
        parent = nullptr;
        num = NULL;
    }
    Node(int number){
        num = number;
        next = nullptr;
        parent = nullptr;
    }

};

class list{// dosent mess with left and right pointers on nodes
    public:
    Node* root;
    list** kids; // list of pointers to other lists
    list* parent;
    int size;
    int degree;

    list(){
        root = nullptr;
        kids = nullptr;
        parent = nullptr;
        size = 0;
        degree = 0;
    }

    list(int degree1){
        root = nullptr;
        parent = nullptr;
        size = 0;
        degree = degree1;
        kids = new list*[degree+1];
    }

    void insert(int number){// dosent handle spliting, or kids at all, the ascend function does that

        if(root == nullptr){
            Node* tmp = new Node(number);
            root = tmp;
            size++;
            return;
        }
        Node* t = root;
        while(t != nullptr){
            if(number > t->num){
                t = t->next;
            }else{
                break;
            }
        }
        Node* tmp = new Node(number);
        if(t == root){
            tmp->next = t;
            t->parent = tmp;
            root = tmp;
            size++;
        }else{
            tmp->next = t;
            tmp->parent = t->parent;
            t->parent->next = tmp;
            t->parent = tmp;
            size++;
        }
    }

    void ascend(int num, list* kidLeft, list* kidRight){ // node will be inserted into this list and kids will be added as kids

        insert(num);

        //go through kids and insert and sort
        for(int i=0; i<degree; i++){ // once for left kid
            if(kids[i] == nullptr){ // insert kid into empty spot
                kids[i] = kidLeft;
            }else if(kids[i]->root->num > kidLeft->root->num){
                // do the cupid shuffle
                list* tmp = kidLeft;
                for(int j=i; j<degree; i++){
                    if(tmp = nullptr){
                        break;
                    }
                    list* hold = kids[i];
                    kids[i] = tmp;
                    tmp = hold;
                }
            }
        }
        for(int i=0; i<degree; i++){ // once for right kid
            if(kids[i] == nullptr){ // insert kid into empty spot
                kids[i] = kidRight;
            }else if(kids[i]->root->num > kidLeft->root->num){
                // do the cupid shuffle
                list* tmp = kidRight;
                for(int j=i; j<degree; i++){
                    if(tmp = nullptr){
                        break;
                    }
                    list* hold = kids[i];
                    kids[i] = tmp;
                    tmp = hold;
                }
            }
        }

    }

    Node* remove(int number){
        Node* t = root;
        while(t != nullptr){
            if(t->num == number){
                break;
            }
            t = t->next;
        }
        if(t == nullptr){// will catch empty lists too
            return nullptr;
        }
        if(t == root){
            root = t->next;
            root->parent = nullptr;
            t->next = nullptr;
            t->parent = nullptr;
            size--;
            return t;
        }else{
            t->parent->next = t->next;
            if(t->next != nullptr){
                t->next->parent = t->parent;
            }
            t->next = nullptr;
            t->parent = nullptr;
            size--;
            return t;
        }


    }

    void display(){
        Node* t = root;
        while(t != nullptr){
            cout << t->num << " -> ";
        }
        cout << endl;
    }
};

class BTree{// dosent mess with parent and next pointers on nodes
    public:
    int degree;
    list* root;
    BTree(int degree1){
        degree = degree1;
        root = nullptr;
    }

    void insert(int num){
        if(root = nullptr){
            list* tmp = new list();
            tmp->insert(num);
            root = tmp;
            return;
        }
        //insert into leafes only!!! 
        list* t = root; // is a list, is one node of a BTree
        
        Node* tmp = t->root; // is a Node, one node of a list

        for(int i=0; i<t->size;i++){//to go through the nodes in a list, if this loop ends then it should either be inserted or its = to existing

            list* kidLeft = t->kids[i];
            list* kidRight = t->kids[i+1]; // at this point size should be < degree so i+1 is never out of bounds

            if(num<tmp->num){ // if less than tmp then try to go to kidLeft

                if(kidLeft != nullptr){ // try a lower node
                    t = kidLeft;
                    i=0;
                    continue;
                }else{// no appropriate kid, insert here

                    t->insert(num);
                    break;
                }

            }else if(num>tmp->num){ //if greater than tmp check a couple options

                if(tmp->next == nullptr){ // if no next node

                    if (kidRight != nullptr){ // try right child

                        t = kidRight;
                        i=0;
                        continue;
                    }else{ // no appropriate kid, insert here

                        t->insert(num);
                        break;
                    }
                }else if(num < tmp->next->num){ // if less than next node

                    if (kidRight != nullptr){ // try right child

                        t = kidRight;
                        i=0;
                        continue;
                    }else{ // no appropriate kid, insert here

                        t->insert(num);
                        break;
                    }
                }else{ // if greater than next node then go to next node
                    tmp = tmp->next;
                }
            }
        }

        //Check if t needs to be split, then split it if it needs to be
        if(t->size == degree){
            int mid = 0;
            if(degree%2==0){
                mid = degree / 2 - 1;
            }else{
                mid = degree / 2;
            }
            
            // grab the mid number
            Node* p = t->root;
            int tmpNum = 0;
            for(int i=0; i<mid;i++){
                p = p->next;
            }
            tmpNum = p->num;

            //create 2 lists and fill them
            list* tmpLeft = new list(degree);
            list* tmpRight = new list(degree);
            p = t->root;
            for(int i=0; i<t->size; i++){
                if(i<mid){
                    tmpLeft->insert(p->num);
                }else if(i>mid){
                    tmpRight->insert(p->num);
                }
                p = p->next;
            }

            // TODO assign children to these two lists

            //ascend them into t's parent
            if(t == root){ // t is root so just have root point to t and lrfy and right be kids
                root = t;
                t->kids[0] = tmpLeft;
                t->kids[1] = tmpRight;
            }else{
                //first we gotta tell parent to stop pointing to t as a kid
                for(int i=0; i<degree;i++){
                    if(t->parent->kids[i] == t){
                        t->parent->kids[i] = nullptr;
                    }
                }
                t->parent->ascend(tmpNum, tmpLeft, tmpRight);
            }
        }


    }

    void display(){

    }

    void displayLevel(int level){

    }
};

int main(int argc, char *argv[]){
    ArgumentManager am(argc,argv);
    string inputfilename = am.get("input");
    string outputfilename = am.get("output");

    ifstream inf;
    inf.open(inputfilename);




    inf.close();
}

//TODO LIST

//make BTREE handle parents of lists

//make BTREE display

//make BTREE display level

//make ascend function
//      make it handle list of kids properly