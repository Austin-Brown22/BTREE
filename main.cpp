#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
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
        num = -1;
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
        for(int i=0; i<=degree; i++){
            kids[i] = nullptr;
        }
    }

    void insert(int number){// dosent handle spliting, or kids at all, the ascend function does that
        //cout << "inserting in a list... " << endl;
        if(root == nullptr){
            //cout << "inserting first node in list" << endl;
            Node* tmp = new Node(number);
            root = tmp;
            size++;
            return;
        }
        Node* t = root;
        while(t->next != nullptr){
            
            if(number > t->num){
                t = t->next;
            }else{
                break;
            }
        }
        Node* tmp = new Node(number);
        if(t == root){ // if root check if num is bigger or smaller
            if(number < root->num){ // replace root
                tmp->next = t;
                t->parent = tmp;
                root = tmp;
                size++;
            }else{
                tmp->parent = t;
                t->next = tmp;
                size++;
            }
        }else if(number > t->num){ // add to end
            tmp->parent = t;
            t->next = tmp;
            size++;
        }else{ // add between 2 nodes
            tmp->next = t;
            tmp->parent = t->parent;
            t->parent->next = tmp;
            t->parent = tmp;
            size++;
        }
        //cout << "finished inserting into list" << endl;
    }

    void ascend(int num, list* kidLeft, list* kidRight){ // node will be inserted into this list and kids will be added as kids and sorted
        //cout << "top of ascension, root is " << root->num << endl;
        insert(num);
        //cout << "list of kids" << endl;
        //for(int i=0; i<=degree; i++){
        //    cout << " kid[" << i << "] is " << kids[i];
        //    if(kids[i] != nullptr){
        //        cout << ":" << kids[i]->root->num;
        //    }
        //}
        //cout << endl;
        //go through kids and insert and sort
        //cout << "fixin kids on leftsplit" << endl;
        for(int i=0; i<=degree; i++){ // once for left kid
            //cout << "lookin at kids[" << i << "] "<< kids[i] << endl;
            if(kids[i] == nullptr){ // insert kid into empty spot
                //cout << "inserted into empty kid" << endl;
                kids[i] = kidLeft;
                break;
            }else if(kids[i]->root->num > kidLeft->root->num){
                // do the cupid shuffle
                //cout << "shuffle" << endl;
                list* tmp = kidLeft;
                for(int j=i; j<=degree; j++){
                    list* hold = kids[j];
                    kids[j] = tmp;
                    tmp = hold;
                    if(tmp == nullptr){
                        break;
                    }
                }
                break;
            }
        }
        //cout << "fixin kids on rightsplit" << endl;
        for(int i=0; i<=degree; i++){ // once for right kid
            //cout << "lookin at kids[" << i << "] "<< kids[i] << endl;
            //if(kids[i] != nullptr){
            //    kids[i]->display();
            //}
            if(kids[i] == nullptr){ // insert kid into empty spot
                //cout << "inserted into empty kid" << endl;
                kids[i] = kidRight;
                break;
            }else if(kids[i]->root->num > kidRight->root->num){
                // do the cupid shuffle
                //cout << "shuffle" << endl;
                list* tmp = kidRight;
                for(int j=i; j<=degree; j++){
                    list* hold = kids[j];
                    //cout << "to the right t:" << tmp << " H: " << hold << endl;
                    kids[j] = tmp;
                    tmp = hold;
                    if(tmp == nullptr){
                        //cout << "ascension complete" << endl;
                        return;
                    }
                }
            }
        }
        //cout << "ascension complete" << endl;
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
            //usleep(100000);
            cout << t->num << " -> ";
            t = t->next;
        }
        cout << endl;
    }
};

class BTree{// dosent mess with parent and next pointers on nodes
    public:
    int degree;
    int height;
    list* root;
    BTree(int degree1){
        degree = degree1;
        height = 0;
        root = nullptr;
    }

    void split(list* t){// t IS a list that needs to be split, @ end ask if "parent" list needs to be split and recursivly call
        //if(t!= root){
        //    cout << "list of kids for t:" << t->parent->root->num << endl;
        //    for(int i=0; i<=degree; i++){
        //        cout << " kid[" << i << "] is " << t->parent->kids[i];
        //        if(t->parent->kids[i]!=nullptr){
        //            cout << ":" << t->parent->kids[i]->root->num;
        //        }
        //    }
        //    cout << endl;
        //}
        //cout << "pre split display:" << endl;
        //display();
        
        //cout << endl;
        int mid = 0; // index of middle number
        if(degree%2==0){// when degree is even will choose left one
            mid = degree / 2 - 1;
        }else{
            mid = degree / 2;
        }
        //cout << "we are in split. Mid is: " << mid << endl;
        // grab the mid number
        Node* p = t->root;
        int tmpNum = 0;
        for(int i=0; i<mid;i++){
            p = p->next;
        }
        tmpNum = p->num;
        //cout << "mid number is: " << tmpNum << endl;
        //for(int i=0; i<degree; i++){
            //cout << "kid " << i << " is " << t->kids[i] << endl;
        //}
        
        //create 2 lists and fill them and assign them kids
        list* tmpLeft = new list(degree);
        list* tmpRight = new list(degree);
        for(int i=0; i<=degree; i++){
            tmpLeft->kids[i] = nullptr;
            tmpRight->kids[i] = nullptr;
        }
        p = t->root;
        for(int i=0; i<t->size; i++){
            
            if(i<mid){
                tmpLeft->insert(p->num);
            }else if(i>mid){
                tmpRight->insert(p->num);
            }
            if(i<=mid){
                tmpLeft->kids[i] = t->kids[i];
                
                //cout << "left_" << tmpLeft << ":" << tmpLeft->kids[i] << " | " << i << " - " << i << "|" << t->kids[i] << endl;
            }else{
                tmpRight->kids[i-mid-1] = t->kids[i];
                
                //cout << "right_" << tmpRight << ":" << tmpRight->kids[i-mid-1] << " | " << i-mid-1 << " - " << i << "|" << t->kids[i] << endl;
            }
            p = p->next;
        }
        
        //cout << "root of leftsplit List: " << tmpLeft->root->num << "  root of rightsplit List: " << tmpRight->root->num << endl;
        //there is size+1 kids so add the last to kidRight
        tmpRight->kids[t->size-mid-1] = t->kids[degree]; 
        if(t->kids[t->size-mid-1]!=nullptr){
            t->kids[t->size-mid-1]->parent = tmpRight;
        }
        //cout << "right_" << tmpRight << ":" << tmpRight->kids[t->size-mid-1] << " | " << t->size-mid-1 << " - " << degree << "|" << t->kids[degree] << endl;
        for(int i=0; i<=degree; i++){
            //cout << "what" << i << tmpLeft->kids[i] << endl;
            if(tmpLeft->kids[i] != nullptr){
                tmpLeft->kids[i]->parent = tmpLeft;
                //cout << tmpLeft->kids[i] << ":" << tmpLeft->kids[i]->root->num << "'s parent is " << tmpLeft << ":" << tmpLeft->root->num << endl;
            }
            //cout << "im a cuck" << i << tmpRight->kids[i] << endl;
            if(tmpRight->kids[i] != nullptr){
                tmpRight->kids[i]->parent = tmpRight;
                //cout << tmpRight->kids[i] << ":" << tmpRight->kids[i]->root->num << "'s parent is " << tmpRight << ":" << tmpRight->root->num << endl;
            }
        }
        //cout << "the Fuck" << endl;
        //cout << "list of kids for left:" << tmpLeft->root->num << endl;
        //for(int i=0; i<=degree; i++){
        //    cout << " kid[" << i << "] is " << tmpLeft->kids[i];
        //}
        //cout << endl;
        //cout << "list of kids for right:" << tmpRight->root->num << endl;
        //for(int i=0; i<=degree; i++){
        //    cout << " kid[" << i << "] is " << tmpRight->kids[i];
        //}
        //cout << endl;
        //set their parents to t->parent
        tmpLeft->parent = t->parent;
        tmpRight->parent = t->parent;
        //cout << "leftsplit parent: " << tmpLeft->parent << " rightsplit parent: " << tmpRight->parent << endl;
        //cout << tmpRight->kids[0] << endl;
        //display();
        //tmpLeft->display();
        //tmpRight->display();
        //------------------------------------------------------------------------------------------------------------------------------------------------------
        //ascend them into t's parent
        if(t == root){ // t is root so just have root point to new list and lrfy and right be kids
            //cout << "we are spliting the root node" << endl;
            //cout << "leftkid root is " << tmpLeft->root->num << endl;
            //for(int i=0; i<=degree; i++){
            //    cout << "leftkid[" << i << "] is " << tmpLeft->kids[i] << endl;
            //}
            height++;
            list* newRoot = new list(degree);
            newRoot->insert(tmpNum);
            root = newRoot;
            newRoot->kids[0] = tmpLeft;
            newRoot->kids[1] = tmpRight;
            tmpLeft->parent = newRoot;
            tmpRight->parent = newRoot;
            //cout << "leftsplit parent: " << tmpLeft->parent << " rightsplit parent: " << tmpRight->parent << endl;
            //usleep(1000000);
            //cout << "list of kids for left:" << tmpLeft->root->num << endl;
            //for(int i=0; i<=degree; i++){
            //    cout << " kid[" << i << "] is " << tmpLeft->kids[i];
            //}
            //cout << endl;
            //cout << "list of kids for right:" << tmpRight->root->num << endl;
            //for(int i=0; i<=degree; i++){
            //    cout << " kid[" << i << "] is " << tmpRight->kids[i];
            //}
            //cout << endl;
        }else{
            //first we gotta tell parent to stop pointing to t as a kid
            //cout << "we are not spliting root node" << endl;
            //cout << t->parent << endl;
            for(int i=0; i<=degree;i++){
                if(t->parent->kids[i] == t){
                    //cout << "removing myself from parent" << endl;
                    t->parent->kids[i] = nullptr;
                }
            }
            //cout << "not a child" << endl;
            t->parent->ascend(tmpNum, tmpLeft, tmpRight);
        }



        // check if "parent needs split"
        if(t->parent != nullptr && t->parent->size == degree){
            split(t->parent);
        }
    }

    void insert(int num){
        //cout << "Inside insert function!" << endl;
        //cout << "root: " << root <<endl;
        if(root == nullptr){
            //cout << "inserting first node" << endl;
            list* tmp = new list(degree);
            tmp->insert(num);
            root = tmp;
            //cout << "finished inserting first node" << endl;
            return;
        }
        //insert into leafes only!!! 
        list* t = root; // is a list, is one node of a BTree
        
        Node* tmp = t->root; // is a Node, one node of a list
        //cout << "attempting to find correct place for new number" << endl;
        for(int i=0; i<t->size;i++){//to go through the nodes in a list, if this loop ends then it should either be inserted or its = to existing
            //cout << "nut " << i << endl;  
            list* kidLeft = t->kids[i];
            //cout << "nut2" << endl;
            list* kidRight = t->kids[i+1]; // at this point size should be < degree so i+1 is never out of bounds
            //cout << "set up kids for comparison" << endl;
            
            //cout << "CHECKING: curr=" << tmp->num << "  leftkid=" << (kidLeft!=nullptr?to_string(kidLeft->root->num):"nullptr") << "  rightkid=" << (kidRight!=nullptr?to_string(kidRight->root->num):"nullptr") << endl;
            //if(t->root->num == 6){
                //cout << "list of kids for 6:" << endl;
            //    for(int i=0; i<=degree; i++){
            //        cout << " kid[" << i << "] is " << t->kids[i];
            //    }
            //    cout << endl;
            //}else if(t->parent != nullptr && t->parent->root->num == 6){
                //cout << "list of kids for 6 from 8:" << endl;
            //    for(int i=0; i<=degree; i++){
            //        cout << " kid[" << i << "] is " << t->parent->kids[i];
            //    }
            //    cout << endl;
            //}

            if(num<tmp->num){ // if less than tmp then try to go to kidLeft
                //cout << "newNum:" << num << " is less than " << tmp->num << endl;
                if(kidLeft != nullptr){ // try a lower node
                    //cout << "looking at left child" << endl;
                    t = kidLeft;
                    i=-1;
                    tmp = kidLeft->root;
                    continue;
                }else{// no appropriate kid, insert here

                    t->insert(num);
                    break;
                }

            }else if(num>tmp->num){ //if greater than tmp check a couple options
                //cout << "newNum:" << num << " is more than " << tmp->num << endl;
                //if(t->root->num == 6){
                    //cout << "list of kids for 6:" << endl;
                    //for(int i=0; i<=degree; i++){
                    //    cout << " kid[" << i << "] is " << t->kids[i];
                    //}
                    //cout << endl;
                //}
                if(tmp->next == nullptr){ // if no next node
                    //cout << "nonoextNODE" << endl;
                    if (kidRight != nullptr){ // try right child
                        //cout << "looking at right child" << endl;
                        
                        //cout << "6 adress: " << t << ":" << t->root->num << endl;
                        t = kidRight;
                        //cout << "ts parent:" << t->parent << ":" << t->parent->root->num << endl;
                        //if(t->parent->root->num == 6){
                        //    cout << "list of kids for 6:" << endl;
                        //    for(int i=0; i<=degree; i++){
                        //        cout << " kid[" << i << "] is " << t->parent->kids[i];
                        //    }
                        //    cout << endl;
                        //}
                        tmp = kidRight->root;
                        i=-1;
                        
                        continue;
                    }else{ // no appropriate kid, insert here
                        //cout << "no right kid, inserting " << num << " here" << endl;
                        t->insert(num);
                        break;
                    }
                }else if(num < tmp->next->num){ // if less than next node
                    //cout << "but less than " << tmp->next->num << endl;
                    if (kidRight != nullptr){ // try right child
                        //cout << "looking at left child" << endl;
                        t = kidRight;
                        tmp = kidRight->root;
                        i=-1;
                        //cout << "size: " << t->size << " i: " << i << endl;
                        //continue;
                    }else{ // no appropriate kid, insert here
                        //cout << "no right kid, inserting " << num << " here" << endl;
                        t->insert(num);
                        break;
                    }
                }else{ // if greater than next node then go to next node
                    tmp = tmp->next;
                }
            }
        }

        //Check if t needs to be split, then call split it if it needs to be
        //cout << "we ";
        if(t->size == degree){
            //cout << "need to split" << endl;
            //if(t->root->num == 6){
            //    cout << "list of kids for 6:" << endl;
            //    for(int i=0; i<=degree; i++){
            //        cout << " kid[" << i << "] is " << t->kids[i];
            //    }
            //    cout << endl;
            //}
            split(t);
        }else{
            //cout << "dont need to split" << endl; 
        }


    }

    void display(){
        int level = 1;
        string out = "1";
        for(int i=0; i<=height; i++){
            //usleep(1000000);
            out = "";
            //cout << "gathering level:" << level <<endl;
            out = displayLevel(level);
            //cout << "_" << out << "_" << endl;
            if(out != "" && out != " "){
                cout <<  out << endl;
            }
            level++;
        }
    }

    string InorderTraversal(list* t){
        int level = 1;
        string out = "";
        Node* p = t->root; 
        for(int i=0; i<degree; i++){
            if(t->kids[i] != nullptr){
                out += InorderTraversal(t->kids[i]);
            }
            if(p != nullptr){
                out += to_string(p->num)+ " ";
                p = p->next;
            }
        }
        return out;

    }

    string displayLevel(int level){
        string output = traverse(level, 1, root);
        if(output == ""){
            return "empty";
        }else{
            return output;
        }
    }

    string traverse(int level,int cur, list* list){
        if(level == cur){
            //cout << "correct level: " << cur << endl;
            string out = "";
            Node* p = list->root;
            for(int i=0; i<list->size; i++){
                out += " " + to_string(p->num);
                //cout << "adding " << p->num << " to out" << endl;
                
                p = p->next;
            }
            out = out.substr(1);
            return out; // substr so we take out a leading space
        }else{
            string out = "";
            for(int i=0; i<=degree; i++){
                if(list->kids[i] != nullptr){
                    string tmp = traverse(level, cur+1, list->kids[i]);
                    //cout << "----------" << tmp << "----" << endl;
                    if(tmp != ""){
                        out += " " + tmp;
                    }
                }
            }
            //cout << "_-_-_-_-_-" << out << endl;    
            if(out == "" || out == " "){
                return "";
            }else{
                return out.substr(1); // substr so we take out a leading space
            }
        }
    }
};

int main(int argc, char *argv[]){
    ArgumentManager am(argc,argv);
    string inputfilename = am.get("input");
    string outputfilename = am.get("output");
    string cmdfilename = am.get("command");

    ifstream inf;
    inf.open(inputfilename);
    ifstream cmdf;
    cmdf.open(cmdfilename);
    ofstream outf;
    outf.open(outputfilename);
    if(!inf.is_open()){
        cout << "FAILED TO OPEN THE FREEKING FILE WHATT"<< endl;
    }
    //string Clueless;
    //getline(cmdf, Clueless);
    //cout << "finished setup" << endl;

    //grab degree
    string degreeString;
    cmdf >> degreeString;
    //cout << Clueless << "AGHGHGHGHGHHHGHGHGH" << endl;;
    int degree = stoi(degreeString.substr(7));
    //cout << "degree is : " << degree << endl;
    //create bTree and insert all those numbers
    BTree btree(degree);
    string newNum = "";
    while(inf >> newNum){
        //cout << "inserting _" << newNum << "_" << endl;
        btree.insert(stoi(newNum));
        //cout << "inserted " << newNum << endl;
        //cout << "curent btree" << endl;
        //btree.display();
        //cout << "nut" << endl;
    }
    //cout << "nut" << endl << endl;
    //btree.display();
    //cout << btree.displayLevel(2) << endl;
    string cmd = "";
    int leveltoPrint = 0;
    while(cmdf >> cmd){
        if(cmd == "Level"){
            cmdf >> leveltoPrint;
            outf << btree.displayLevel(leveltoPrint) << endl;
            //cout << btree.displayLevel(leveltoPrint);
        }else if(cmd == "Inorder"){
            cmdf >> cmd;
            outf << btree.InorderTraversal(btree.root) << endl;;
            //cout << btree.InorderTraversal(btree.root);
        }
    }
    inf.close();
    cmdf.close();
    outf.close();
}