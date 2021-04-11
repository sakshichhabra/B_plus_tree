#include<bits/stdc++.h>
#include<stdio.h>

using namespace std;

int order;          //ORDER OF B+TREE
int const MAX =50;  //MAXIMUM SIZE OF THE ARRAY
static int searchTrigger =1;    //IT WILL SET ITS VALUE TO 1 WHEN VARIABLE NOT FOUND
ofstream outputFile;    //TO GENERATE OUTPUT FILE

struct Node{

    Node *parentNode;       //PARENT NODE
    Node *childNode[MAX];   //CHILD NODE
    int key[MAX];           //DEFINING KEYS
    double values[MAX];      //DEFINING VALUES
    int totalNodes;         //TOTAL NUMBER OF ACTIVE NODES AT ANY INSTANCE

    //CONSTRUCTOR FOR NODE INITIALIZATION
    Node(){
        parentNode = NULL;
        totalNodes = 0;
        for(int i=0; i<MAX; i++){
            childNode[i] = NULL;
            key[i] = INT_MAX;
            values[i]=INT_MAX;
        }
    }
};
Node *rootNode = new Node();//ROOT NODE

struct DList{			//Doubly linked list for the leaf node
int key;				//key of DList
double value;			//value of DList
DList *next;			//next pointer to point next node
DList *prev;			//previous pointer to point to the previous node
}*START=NULL,*Temp1=NULL,*q=NULL;   //some supporting pointer like START NODE


//SPLIT FUNCTION
void splitLeaf(Node *currentNode){
    int i,j,x;
    //DEFINING VARIBLE X TO SPLIT VALUESIN TWO HALFS BY ODD AND EVEN
    if(order%2){
        x = (order+1)/2;
        }
    else {x = order/2;
    }

    Node *rightNode = new Node();//NEW RIGHT NODE

    currentNode->totalNodes = x;    //UPDATING NUMBER OF ACTIVE NODES VALUES -> VALUE
    rightNode->totalNodes = order-x;//DEFINING TOTAL NUMBER OF ACTIVE NODE NUMBER

    rightNode->parentNode = currentNode->parentNode;//ASSIGNING COMMON PARENT TO CURRENTNODE TO RIGHTNODE

    for(i=x, j=0; i<order; i++, j++){
        rightNode->key[j] = currentNode->key[i];        //COPYING THE KEY VALUES
        rightNode->values[j] = currentNode->values[i];  //COPYING ALL VALUES

        currentNode->key[i] = INT_MAX;      //SETTING TO DEFAULT VALUE
        currentNode->values[i] = INT_MAX;   //SETTING TO DEFAULT VALUE
    }

    int val = rightNode->key[0];        //COPYING RIGHTMOST KEY VALUE TO VAL
    double rightval=rightNode->values[0];//COPYING RIGHTMOST VALUE TO RIGHTVAL

    //IF THE LEAF NODE IS PARENT NODE
    if(currentNode->parentNode==NULL){

        //GIVING LEAF NODE A PARENT
        Node *parentNode = new Node();
        parentNode->parentNode = NULL;
        parentNode->totalNodes=1;

        parentNode->key[0] = val;       //RIGHTMOST PARENT KEY
        parentNode->values[0]=rightval; //RIGHTMOST PARENT VALUE

        //ADDING THE CHILD NODES
        parentNode->childNode[0] = currentNode;
        parentNode->childNode[1] = rightNode;

        currentNode->parentNode = rightNode->parentNode = parentNode;   //SETTING THE NEW PARENTNODE
        rootNode = parentNode;                                          //UPDATING ROOTNODE
        return;//TASK COMPLETE;
    }
    else{   //IF THE NODE IS NOT PARENT
        currentNode = currentNode->parentNode;

        //SETTING THE NEW RIGHTCHILDNODE TO ITS CORRECT POSITION IN PARENT NODE
        Node *newChildNode = new Node();
        newChildNode = rightNode;

        for(i=0; i<=currentNode->totalNodes; i++){//SWAPING THE VALUES UNTIL SETS TO THE CORRECT POSITION IN PARENTNODE
            if(val < currentNode->key[i]){
                swap(currentNode->key[i], val);
                swap(currentNode->values[i],rightval);
            }
        }

        currentNode->totalNodes++;//INCRESING THE TOTAL VALUES BY ONE

        //SETTING THE RIGHTNODE TO CORRECT POSITION
        for(i=0; i<currentNode->totalNodes; i++){
            if(newChildNode->key[0] < currentNode->childNode[i]->key[0]){
                swap(currentNode->childNode[i], newChildNode);
            }
        }
        currentNode->childNode[i] = newChildNode;

        for(i=0;currentNode->childNode[i]!=NULL;i++){//SETTING THE CHILDNODE PARENT TO CURRENTNODE
            currentNode->childNode[i]->parentNode = currentNode;
        }
    }
}
//FUNTION TO SPLIT NONLEAF NODES
void splitNonLeaf(Node *currentNode){
    int x, i, j;

    x = order/2;                        //DEFINING VARIBLE X TO SPLIT VALUESIN TWO HALFS BY ODD AND EVEN

    Node *rightNode = new Node();       //NEW RIGHT NODE

    currentNode->totalNodes = x;        //UPDATING NUMBER OF ACTIVE NODES VALUES -> VALUE
    rightNode->totalNodes = order-x-1;  //DEFINING TOTAL NUMBER OF ACTIVE NODE NUMBER

    rightNode->parentNode = currentNode->parentNode;//ASSIGNING COMMON PARENT TO CURRENTNODE TO RIGHTNODE

    //MOST OF THE COMMENTS ARE SAME AS ABOVE
    for(i=x, j=0; i<=order; i++, j++){

        rightNode->key[j] = currentNode->key[i];
        rightNode->values[j] = currentNode->values[i];
        rightNode->childNode[j] = currentNode->childNode[i];

        currentNode->key[i] = INT_MAX;
        currentNode->values[i] = INT_MAX;

        if(i!=x)currentNode->childNode[i] = NULL;
    }

    int val = rightNode->key[0];
    double rightval = rightNode->values[0];

    //MEMORY COPYING THE VALUES AS IT ISBY USING MEMORYCOPY FUNCTION
    memcpy(&rightNode->key, &rightNode->key[1], sizeof(int)*(rightNode->totalNodes+1));
    memcpy(&rightNode->values, &rightNode->values[1], sizeof(double)*(rightNode->totalNodes+1));
    memcpy(&rightNode->childNode, &rightNode->childNode[1], sizeof(rootNode)*(rightNode->totalNodes+1));

    for(i=0;currentNode->childNode[i]!=NULL;i++){               //UPDATING THE PARENTS VALUES
        currentNode->childNode[i]->parentNode = currentNode;}
    for(i=0;rightNode->childNode[i]!=NULL;i++){                 //UPDATING THE PARENTS VALUES
        rightNode->childNode[i]->parentNode = rightNode;}

    //IF THE LEAF NODE IS PARENT NODE
    if(currentNode->parentNode==NULL){

        //GIVING LEAF NODE A PARENT
        Node *parentNode = new Node();
        parentNode->parentNode = NULL;
        parentNode->totalNodes=1;

        parentNode->key[0] = val;           //COPYING THE KEY VALUE
        parentNode->values[0] = rightval;   //COPYING THE VALUE

        parentNode->childNode[0] = currentNode;
        parentNode->childNode[1] = rightNode;

        currentNode->parentNode = rightNode->parentNode = parentNode; //GIVING COMMON PARENT

        rootNode = parentNode;  //SETTING THE ROOTNODE
        return;
    }
    else{//IF THE SPLIT NODE IS NOT A PARENTNODE
        currentNode = currentNode->parentNode;

        Node *newChildNode = new Node();
        newChildNode = rightNode;

        for(i=0; i<=currentNode->totalNodes; i++){
            if(val < currentNode->key[i]){
                swap(currentNode->key[i], val);
                swap(currentNode->values[i],rightval);
            }
        }

        currentNode->totalNodes++;

        for(i=0; i<currentNode->totalNodes; i++){
            if(newChildNode->key[0] < currentNode->childNode[i]->key[0]){
                swap(currentNode->childNode[i], newChildNode);
            }
        }
        currentNode->childNode[i] = newChildNode;

         for(i=0;currentNode->childNode[i]!=NULL;i++){
            currentNode->childNode[i]->parentNode = currentNode;
        }
    }
}
//FUNCTION TO INSERT A VALUE AT SORTED POSITION
void insertNode(Node *currentNode, int keyVal,double value){
    //TAKING THE VALUE AT CORRECT SORTED POSITION
    for(int i=0; i<=currentNode->totalNodes; i++){
        if(keyVal < currentNode->key[i] && currentNode->childNode[i]!=NULL){
            insertNode(currentNode->childNode[i], keyVal,value);
            if(currentNode->totalNodes==order)//CALLING SPLIT FUNCTION WHEN OVERFLOWS
                splitNonLeaf(currentNode);
            return;
        }
        //ELSE SETTING THE VALUE AND UPDATING THE VALUES IF REQUIRED
        else if(keyVal < currentNode->key[i] && currentNode->childNode[i]==NULL){
            swap(currentNode->key[i], keyVal);
            swap(currentNode->values[i], value);

            if(i==currentNode->totalNodes){
                    currentNode->totalNodes++;
                    break;
            }
        }
    }

    if(currentNode->totalNodes==order){
            splitLeaf(currentNode);
    }
}
//FUNCTION TO RE-DISTRIBUTE THE NODES WHEN IMBALANCE
void redistributeNode(Node *leftNode, Node *rightNode, bool isLeaf, int posOfLeftNode, int whichOneiscurrentNode){
    //RE-DISTRIBUTION OF TREE TO BALANCE
    if(whichOneiscurrentNode==0){
        //IF THE NODE IS NOT A LEAF NODE
        if(!isLeaf){
            //COPYING THE PARENT NODE KEY AND VALUE
            leftNode->key[leftNode->totalNodes] = leftNode->parentNode->key[posOfLeftNode];
            leftNode->values[leftNode->totalNodes] = leftNode->parentNode->values[posOfLeftNode];

            //ADDING THE RIGHT NODE RIGHTMOST CHILD POINTER TO THE LEFT OF LEFTMOST CHILDPOINTER
            //AND INCREASING THE TOTAL NODE VALUE BY ONE
            leftNode->childNode[leftNode->totalNodes+1] = rightNode->childNode[0];
            leftNode->totalNodes++;

            //SETTING THE PARENT OF NEWLY COPIED NODE FROM THE RIGHT NODE POINTER
            leftNode->parentNode->key[posOfLeftNode] = rightNode->key[0];
            leftNode->parentNode->values[posOfLeftNode] = rightNode->values[0];

            //DELETING THE RIGHTMOST VALUE OF RIGHTNODE AND DECREASING THE TOTALNODE VALUE BY ONE
            memcpy(&rightNode->key[0], &rightNode->key[1], sizeof(int)*(rightNode->totalNodes+1));
            memcpy(&rightNode->values[0], &rightNode->values[1], sizeof(double)*(rightNode->totalNodes+1));
            memcpy(&rightNode->childNode[0], &rightNode->childNode[1], sizeof(rootNode)*(rightNode->totalNodes+1));
            rightNode->totalNodes--;

        }else{//IF IT IS A LEAF NODE
            leftNode->key[leftNode->totalNodes] = rightNode->key[0];
            leftNode->values[leftNode->totalNodes] = rightNode->values[0];
            leftNode->totalNodes++;

            memcpy(&rightNode->key[0], &rightNode->key[1], sizeof(int)*(rightNode->totalNodes+1));
            memcpy(&rightNode->values[0], &rightNode->values[1], sizeof(double)*(rightNode->totalNodes+1));
            rightNode->totalNodes--;

            leftNode->parentNode->key[posOfLeftNode] = rightNode->key[0];
            leftNode->parentNode->values[posOfLeftNode] = rightNode->values[0];
        }

    }else{
        if(!isLeaf){

            memcpy(&rightNode->key[1], &rightNode->key[0], sizeof(int)*(rightNode->totalNodes+1));
            memcpy(&rightNode->values[1], &rightNode->values[0], sizeof(double)*(rightNode->totalNodes+1));
            memcpy(&rightNode->childNode[1], &rightNode->childNode[0], sizeof(rootNode)*(rightNode->totalNodes+1));

            rightNode->key[0] = leftNode->parentNode->key[posOfLeftNode];
            rightNode->values[0] = leftNode->parentNode->values[posOfLeftNode];

            rightNode->childNode[0] = leftNode->childNode[leftNode->totalNodes];
            rightNode->totalNodes++;

            leftNode->parentNode->key[posOfLeftNode] = leftNode->key[leftNode->totalNodes-1];
            leftNode->parentNode->values[posOfLeftNode] = leftNode->values[leftNode->totalNodes-1];

            leftNode->key[leftNode->totalNodes-1] = INT_MAX;
            leftNode->values[leftNode->totalNodes-1] = INT_MAX;
            leftNode->childNode[leftNode->totalNodes] = NULL;
            leftNode->totalNodes--;

        }else{
            memcpy(&rightNode->key[1], &rightNode->key[0], sizeof(int)*(rightNode->totalNodes+1));
            memcpy(&rightNode->values[1], &rightNode->values[0], sizeof(double)*(rightNode->totalNodes+1));

            rightNode->key[0] = leftNode->key[leftNode->totalNodes-1];
            rightNode->values[0] = leftNode->values[leftNode->totalNodes-1];

            rightNode->totalNodes++;

            leftNode->key[leftNode->totalNodes-1] = INT_MAX;
            leftNode->values[leftNode->totalNodes-1] = INT_MAX;
            leftNode->totalNodes--;

            leftNode->parentNode->key[posOfLeftNode] = rightNode->key[0];
            leftNode->parentNode->values[posOfLeftNode] = rightNode->values[0];
        }
    }
}
//FUNTION TO MERGE THE NODES AFTER DELETION (IN CASE OF VALUE DEFICIENT)
void mergeNode(Node *leftNode, Node *rightNode, bool isLeaf, int posOfRightNode){

    if(!isLeaf){//SETTING THE TOTALNODE VALUES
        leftNode->key[leftNode->totalNodes] = leftNode->parentNode->key[posOfRightNode-1];
        leftNode->values[leftNode->totalNodes] = leftNode->parentNode->values[posOfRightNode-1];
        leftNode->totalNodes++;
    }
    //COPYING THE VALUES TO THE LEFTNODE FROM  RIGHTNODE
    memcpy(&leftNode->key[leftNode->totalNodes], &rightNode->key[0], sizeof(int)*(rightNode->totalNodes+1));
    memcpy(&leftNode->values[leftNode->totalNodes], &rightNode->values[0], sizeof(double)*(rightNode->totalNodes+1));
    memcpy(&leftNode->childNode[leftNode->totalNodes], &rightNode->childNode[0], sizeof(rootNode)*(rightNode->totalNodes+1));

    leftNode->totalNodes += rightNode->totalNodes;//SUMING THE LEFTNODE AND RIGHTNODE VALUES
    //UPDATING THE PARENTNODE OF NEWLY MERGED RIGHTNODE
    memcpy(&leftNode->parentNode->key[posOfRightNode-1], &leftNode->parentNode->key[posOfRightNode], sizeof(int)*(leftNode->parentNode->totalNodes+1));
    memcpy(&leftNode->parentNode->values[posOfRightNode-1], &leftNode->parentNode->values[posOfRightNode], sizeof(double)*(leftNode->parentNode->totalNodes+1));
    memcpy(&leftNode->parentNode->childNode[posOfRightNode], &leftNode->parentNode->childNode[posOfRightNode+1], sizeof(rootNode)*(leftNode->parentNode->totalNodes+1));

    leftNode->parentNode->totalNodes--;//REDUCING THE NUMBER OF CHILD BY ONE (DELETING THE RIGHTNODE COUNT)

     for(int i=0;leftNode->childNode[i]!=NULL;i++){
        leftNode->childNode[i]->parentNode = leftNode;
    }
}
bool dataFound;

//FUNTION TO DELETE A VALUE
void deleteNode(Node *currentNode, int val, int currentNodePosition){

     //TO CHECK THE VALUE IF VALUE IS LEAFNODE
     bool isLeaf;
     if(currentNode->childNode[0]==NULL)
        isLeaf = true;
     else isLeaf = false;

    int prevLeftMostVal = currentNode->key[0];

     for(int i=0;dataFound==false &&  i<=currentNode->totalNodes; i++){//USING AS A SEARCH FUNCTION
        // GOING AT CORRECT LEAF NODE POSITION BY USING RECURSION
        if(val < currentNode->key[i] && currentNode->childNode[i] != NULL){
            deleteNode(currentNode->childNode[i], val, i);
        }

        //CHECK WHETHER THE VALUE AT LEAF NODE IS EQUAL TO THE SEARCHED KEY
        else if(val == currentNode->key[i] && currentNode->childNode[i] == NULL){

            //DELETING THE NODES BY JUST SHIFTING THE MEMORY ADDRESS TO LEST BY ONE MEMORY UNIT
            memcpy(&currentNode->key[i], &currentNode->key[i+1], sizeof(int)*(currentNode->totalNodes+1));
            memcpy(&currentNode->values[i], &currentNode->values[i+1], sizeof(double)*(currentNode->totalNodes+1));

            //UPDATING THE TOTALNODES VALUE BY DECREASING BY ONE
            currentNode->totalNodes--;
            dataFound = true;   //MAKING THE FOR LOOP CONDITION FALSE BY SAYING THAT WE FOUND THE VALUE
            break;              //BREAKING THE FOR LOOP
        }
     }

    //RETURN IF ROOT NODE IS THE LEAF NODE
     if(currentNode->parentNode == NULL && currentNode->childNode[0] == NULL){
        return;
     }
  //IF THE CURRENT NODE IS THE ROOT NODE AND HAVE ONLY ONE CHILD POINTER
    if(currentNode->parentNode==NULL && currentNode->childNode[0] != NULL && currentNode->totalNodes == 0){
        rootNode = currentNode->childNode[0];   //MAKING CHILDNODE AS A ROOTNODE
        rootNode->parentNode = NULL;            //SETTING PARENT OF ROOT NODE AS NULL
        return;
    }

    //CHECK WHETHER THE CURRENT NODE HAVE LESS THAN HALF OF MAXIMUM NODE AND THE RIGHT NODE HAVE MORE THAN HALF THAN REDISTRIBUTE IT.
    //IF IT IS LESS THAN HALF THEN WE WILL RE DISTRIBUTE IT.
    if(isLeaf && currentNode->parentNode!=NULL){

        if(currentNodePosition==0){
            Node *rightNode = new Node();
            rightNode = currentNode->parentNode->childNode[1];

            //IF RIGHT NODE HAVE MORE THAN HALF OF THE MAXIMUM NODE CAPACITY THAN REDISTRIBUTE IT.
            if(rightNode!=NULL && rightNode->totalNodes > (order+1)/2){
                    redistributeNode(currentNode, rightNode, isLeaf, 0, 0); //CALLING REDISTRIBUTE FUNCTION
            }
            //IF THEY BOTH HAVE LESS THAN HALF OF THE MAXIMUM CAPACITY THAN WE CAN SAFELY MERGE THEM
            else if (rightNode!=NULL && currentNode->totalNodes+rightNode->totalNodes < order){
                    mergeNode(currentNode, rightNode, isLeaf, 1);           //CALLING MERGENODE FUNCTION
            }
        }
        //ELSE WE WILL CHECK THE LEFT AND RIGHT CAPACITY SEPERATELY AND REDISTRIBUTE AND MERGE THEM
        else{
            Node *leftNode = new Node();
            Node *rightNode = new Node();

            leftNode = currentNode->parentNode->childNode[currentNodePosition-1];
            rightNode = currentNode->parentNode->childNode[currentNodePosition+1];

            //IF THE LEFT NODE HAVE MORE THAN HALF OF THE MAXIMUM CAPACITY THAN REDISTRIBUTE IT AND MERGE IT
            if(leftNode!=NULL && leftNode->totalNodes > (order+1)/2){
                 redistributeNode(leftNode, currentNode, isLeaf, currentNodePosition-1, 1);
            }
            else if(rightNode!=NULL && rightNode->totalNodes > (order+1)/2){
                redistributeNode(currentNode, rightNode, isLeaf, currentNodePosition, 0);
            }
            else if (leftNode!=NULL && currentNode->totalNodes+leftNode->totalNodes < order){
                mergeNode(leftNode, currentNode, isLeaf, currentNodePosition);
            }
            else if (rightNode!=NULL && currentNode->totalNodes+rightNode->totalNodes < order){
               mergeNode(currentNode, rightNode, isLeaf, currentNodePosition+1);
            }
        }
    }
    else if(!isLeaf && currentNode->parentNode!=NULL){

        if(currentNodePosition==0){
            Node *rightNode = new Node();
            rightNode = currentNode->parentNode->childNode[1];

            //IF THE RIGHT NODE HAVE MORE THAN HALF OF THE MAXIMUM CAPACITY THAN REDISTRIBUTE IT AND MERGE IT
            if( rightNode!=NULL && rightNode->totalNodes-1 >= ceil((order-1)/2) ){
                redistributeNode(currentNode, rightNode, isLeaf, 0, 0);
            }
            else if (rightNode!=NULL && currentNode->totalNodes+rightNode->totalNodes < order - 1){
                mergeNode(currentNode, rightNode, isLeaf, 1);
            }
        }
        //FOR ANY OTHER CASE WE WILL SIMPLY TRY TO REDISTRIBUTE IT
        else{
            //INITIALIZATION OF LEFT AND RIGHT NODE
            Node *leftNode = new Node();
            Node *rightNode = new Node();

            //ASSIGNING THE LOCATION IN THE TREE
            leftNode = currentNode->parentNode->childNode[currentNodePosition-1];
            rightNode = currentNode->parentNode->childNode[currentNodePosition+1];

            //IF LEFT NODE HAVE MORE THAN HALF OF ORDER CAPACITY THE RE-DISTRIBUTE
            if( leftNode!=NULL && leftNode->totalNodes-1 >= ceil((order-1)/2)){
                 redistributeNode(leftNode, currentNode, isLeaf, currentNodePosition-1, 1);
            }
            else if(rightNode!=NULL && rightNode->totalNodes-1 >=  ceil((order-1)/2)){
                redistributeNode(currentNode, rightNode, isLeaf, currentNodePosition, 0);
            }
            //MERGING WHEN NOTHING TO RE-DISTRIBUTE

            else if ( leftNode!=NULL && currentNode->totalNodes+leftNode->totalNodes < order-1) {
                mergeNode(leftNode, currentNode, isLeaf, currentNodePosition);
            }
            else if ( rightNode!=NULL && currentNode->totalNodes+rightNode->totalNodes < order-1){
               mergeNode(currentNode, rightNode, isLeaf, currentNodePosition+1);
            }
        }

    }

    Node *tempNode = new Node();
    tempNode = currentNode->parentNode;
    while(tempNode!=NULL){
            for(int i=0; i<tempNode->totalNodes;i++){
                if(tempNode->key[i]==prevLeftMostVal){
                    tempNode->key[i] = currentNode->key[0];
                    tempNode->values[i] = currentNode->values[0];
                    break;
                }
        }
        tempNode = tempNode->parentNode;
    }

}

void Delete(int key){           //DELETE FUNCTION
dataFound = false;              //INITITALLY SET TO FALSE(DATA NOT FOUND)
deleteNode(rootNode, key, 0);   //CALLING DELETE FUNCTION WITH ROOTNODE
}

bool dataFound1;
//FUNCTION FOR SEARCHING THE KEY VALUE
void Search(Node *currentNode, int val, int temp){
    int calsy=0;
    double tempo;
    //TREVERSE THE B+TREE UPTO THE VALUE GREATER THAN REQUIRED VALUE
     for(int i=0;dataFound1==false &&  i<=currentNode->totalNodes; i++){
        if(val < currentNode->key[i] && currentNode->childNode[i] != NULL){
            Search(currentNode->childNode[i], val, i);
        }

        //MATCH THE REQUIRED VALUE TO THE LEAST NEXT POSSIBLE VALUE
        else if(val == currentNode->key[i] && currentNode->childNode[i] == NULL){
            tempo=currentNode->values[i];
            dataFound1 = true;
            calsy=1;
            break;
        }
     }
     //IF VALUE FOUND THEN PRINT IT ELSE NOTHING
     if(calsy==1){
        searchTrigger=0;
        outputFile<<tempo;return;
     }
}
void Search(int k){

    vector < Node* > Nodes;//FOR HOLDINGING RUNTIME NODES INSTANCE
    dataFound1 = false;
    Nodes.clear();
    Nodes.push_back(rootNode);  //GOING BACK TO ROOTNODE
    searchTrigger=1;            //SETTING VALUE TO NOT FOUND
    Search(rootNode , k, 0);    //CALLING SEARCH FUNCTION BY PASSING ROOTNODE REFERENCE
    if(searchTrigger==1){       //PRINTING NULL WHEN NOT FOUND
        outputFile<<"NULL";
    }
    outputFile<<"\n";
}
//SEARCH FOR THE RANGE OF VALUES
void Search(Node *currentNode, int val1,int val2, int temp){
     for(int i=0;i<=currentNode->totalNodes; i++){
        if(val1 < currentNode->key[i] && currentNode->childNode[i] != NULL){
            Search(currentNode->childNode[i], val1,val2, i);//GOES UPTO THE MIN GIVEN VALUE

        }
        //PRINTS THE RANGE VALUES
        else if(val2 >= currentNode->key[i] && val1 <= currentNode->key[i] && currentNode->childNode[i] == NULL){
                outputFile<<currentNode->values[i]<<" ,";
                searchTrigger=0; continue;
        }
     }
}
//CALLING THE MAIN SEARCH FUNCTION WITH 4 PARAMETERS
void Search(int k,int k2){
    vector < Node* > Nodes;     //FOR HOLDINGING RUNTIME NODES INSTANCE
    Nodes.clear();
    Nodes.push_back(rootNode);  //GOING BACK TO ROOTNODE
    searchTrigger=1;            //SETTING VALUE TO NOT FOUND
    Search(rootNode , k,k2, 0); //CALLING SEARCH FUNCTION BY PASSING ROOTNODE REFERENCE
    if(searchTrigger==1){
        outputFile<<"NULL";
    }
    outputFile<<"\n";
}

void Initialize(int x){
    if(x <=50){
        order=x;        //SETTING THE ORDER OF BPLUSTREE
    }
    else{//IF THE ORDER IS GREATER THAN 50
            order=3;    //DEFAULT VALUE
        outputFile<<" not valid initialization\n";
    }
}
 void Insert(int x,double y){
    insertNode(rootNode,x,y);   //CALLING THE INSERT NODE FUNCTION
 }

int main(){

    outputFile.open("output_file.txt"); //CREATING OUTPUT FILE
    cout<<"this is working";
    //DEFINING THE STRING DETECTING VARIABLES
    string INI="Initialize";
    string DEL = "Delete";
    string SEA = "Search";
    string INS = "Insert";
    string openbraces = "(";
    string closebraces = ")";
    string comma = ",";

    string text;
    //OPENING THE INPUT FILE
    ifstream ifs("input.txt");

    int keyValue;
    double value;

    while(!ifs.eof())       //CHECKING THE EOF CONDITION IN A WHILE LOOP
    {   cout<<"stuck in loop";
        getline(ifs,text);  //GETTING A LINE FROM INPUT FILE

        if(text.find(INS)!=string:: npos){ //FINDING THE STRING MATCHES WITH INSERT

            size_t pos1 = text.find(openbraces);    //FINDING THE INDEX OF OPENBRACES
            size_t pos2 = text.find(comma);         //FINDING THE INDEX OF COMMA
            size_t pos3 = text.find(closebraces);   //FINDING THE INDEX OF CLOSING BRACES
            size_t difference1 = pos2-pos1;         //FINDING THE DIFFERENCE BETWEEN INDEXES OF OPEN AND COMMA
            size_t difference2 = pos3-pos2;         //FINDING THE DIFFERENCE BETWEEN INDEXES OF COMMA AND CLOSING BRACES

            string s1 = text.substr(pos1+1, difference1);
            string s2 =text.substr(pos2+1, difference2);

            keyValue = atoi(s1.c_str());    //CONVERTING STRING TO INTEGER
            value = atof(s2.c_str());       //CONVERTING STRING TO INTEGER

            Insert(keyValue, value);    // CALLING INSERT FUNCTION
        }
        else if(text.find(INI)!=string:: npos){      //FINDING THE STRING MATCHES WITH INITIALIZATION

            size_t pos1 = text.find(openbraces);    //FINDING THE INDEX OF OPENBRACES
            size_t pos2 = text.find(closebraces);   //FINDING THE INDEX OF CLOSEING BRACES
            size_t difference = pos2-pos1;          //FINDING THE DIFFERENCE BETWEEN INDEXES OF OPEN AND CLOSING BRACES

            string s = text.substr(pos1+1, difference); //GETTING THE NUMERICAL STRING VALUE FROM LINE
            keyValue = atoi(s.c_str());     //CONVERTING STRING TO INTEGER
            Initialize(keyValue);           // CALLING INITIALIZE FUNCTION

        }
        else if(text.find(DEL)!=string:: npos){ //FINDING THE STRING MATCHES WITH DELETE

            size_t pos1 = text.find(openbraces);    //FINDING THE INDEX OF OPENBRACES
            size_t pos2 = text.find(closebraces);   //FINDING THE INDEX OF CLOSEING BRACES
            size_t difference = pos2-pos1;          //FINDING THE DIFFERENCE BETWEEN INDEXES OF OPEN AND CLOSING BRACES

            string s = text.substr(pos1+1, difference);
            keyValue = atoi(s.c_str());    //CONVERTING STRING TO INTEGER
            Delete(keyValue);   // CALLING DELETE FUNCTION
        }
        else if(text.find(SEA)!=string:: npos){ //FINDING THE STRING MATCHES WITH SEARCH

            size_t pos1 = text.find(openbraces);
            size_t pos2 = text.find(comma);
            size_t pos3 = text.find(closebraces);
            size_t difference1 = pos2-pos1;
            size_t difference2 = pos3-pos2;

            string s1 = text.substr(pos1+1, difference1);
            string s2 =text.substr(pos2+1, difference2);

            keyValue = atoi(s1.c_str());   //CONVERTING STRING TO INTEGER
            value = atoi(s2.c_str());  //CONVERTING STRING TO INTEGER

            if(value!=0){
                Search(keyValue, value);// CALLING SEARCH FUNCTION
            }
            else{
                Search(keyValue);       // CALLING SEARCH FUNCTION
            }

        }
    }
    outputFile.close(); //CLOSING THE OUTPUTFILE
    cout<<"ending softly";
    return 0;
}
