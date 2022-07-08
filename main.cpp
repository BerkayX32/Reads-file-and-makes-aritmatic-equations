#include <iostream>
#include <fstream>
#include <stack>
#include <string>
using namespace std;

struct node{
    node * next;
    node * prev;
    int data;
};

int abs(int x, int y){
    if((x-y)>=0)
        return x-y;
    return y-x;
}

void swap(node ** x, node **y){
    node * temp = *y;
    *y = *x;
    *x = temp;
}

node * push(node * head, int newVal){
    node * newHead = new node;
    newHead->data=newVal;
    newHead->prev=nullptr;
    head->prev=newHead;
    newHead->next=head;
    newHead->data=newVal;
    return newHead;
}

node * addNode(node * head, int data){
    if(!head){
        head = new node;
        head->prev=nullptr;
        head->data=data;
        head->next=nullptr;
    }
    else{
        node * temp = head;
        while(temp->next)
            temp=temp->next;
        temp->next=new node;
        temp->next->prev=temp;
        temp->next->next=nullptr;
        temp->next->data=data;
    }
    return head;
}

node * listOfZeros(int size){
    node * head =nullptr;
    for(int i=0; i<size; i++)
        head=addNode(head, 0);
    return head;
}

int getSize(node * head){
    node * tmp=head;
    int size=0;
    while(tmp){
        size++;
        tmp=tmp->next;
    }
    return size;
}

void printBigInt(node * head){
    node * temp=head;
    while(temp){
        cout << temp->data << " ";
        temp=temp->next;
    }
    cout << endl;
}



void reverse(node **head)
{
    node *temp = nullptr;
    node *curr = *head;
    while (curr != nullptr)
    {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev;
    }
    if(temp != nullptr )
        *head = temp->prev;
}

void freeBigInt(node * & head){
    node * temp = head;
    while(temp){
        node * t2 = temp;
        temp = temp->next;
        delete t2;
    }
    head=nullptr;
}

node * copyAll(node * lhs, node * rhs){
    freeBigInt(lhs);
    lhs=nullptr;
    node * temp=rhs;
    if(temp){
        while(temp){
            lhs=addNode(lhs, temp->data);
            temp=temp->next;
        }
    }
    
    return lhs;
}

node * removeLeadingZeros(node * temp){
    while(temp && temp->data==0){
        node * del = temp;
        temp=temp->next;
        temp->prev=nullptr;
        delete del;
    }
    if(temp)
        return temp;
    else{
        temp = new node;
        temp->data=0;
        temp->next=nullptr;
        temp->prev=nullptr;
        return temp;
    }
}

void carryInsertHelper(node * lhs, node * rhs, int & overhead, node *& result){
    if(lhs != rhs){
        carryInsertHelper(lhs->next, rhs, overhead, result);
        int newVal = (lhs->data + overhead)%10;
        overhead = (lhs->data + overhead)/10;
        result = push(result, newVal);
    }
}

node * sameSizeF(node * lhs, node * rhs, int & overhead){
    if(lhs){
        node * tmp = new node;
        tmp->next = sameSizeF(lhs->next, rhs->next, overhead);
        if(tmp->next)
            tmp->next->prev=tmp;
        tmp->data = (lhs->data+rhs->data+overhead)%10;
        overhead = (lhs->data+rhs->data+overhead)/10;
        return tmp;
    }
    return lhs;
}

node * addBigInt(node * lhs, node * rhs){
    bool onlyOneSide=false;
    bool sameSize=false;
    bool notSameSize=true;
    node * result = nullptr;
    if(!rhs){
        onlyOneSide = true;
        result = copyAll(result, lhs);
    }
    if(!lhs){
        onlyOneSide = true;
        result = copyAll(result, rhs);
    }
    if(!onlyOneSide){
        if(getSize(lhs) == getSize(rhs)){
            sameSize=true;
            int overhead = 0;
            result = sameSizeF(rhs, lhs, overhead);
            if(overhead>0)
            result=push(result, overhead);
        }
    }
    if(!onlyOneSide && !sameSize){
        int changeOfTwo = abs(getSize(lhs), getSize(rhs));
        if(getSize(lhs)<getSize(rhs))
            swap(&lhs, &rhs);
        node * cur = lhs;
        while(changeOfTwo>0){
            cur=cur->next;
            changeOfTwo-=1;
        }
        int overhead=0;
        result = sameSizeF(cur, rhs, overhead);
        carryInsertHelper(lhs, cur, overhead, result);
        if(overhead>0)
            result=push(result, overhead);
    }
    
    return result;
}


node * multiplyBigInt(node * lhs, node * rhs){
    swap(&lhs, &rhs);
    int size1=getSize(lhs);
    int size2=getSize(rhs);
    reverse(&lhs);
    
    reverse(&rhs);
    
    node * rhs_temp = rhs;
    
    node * result = listOfZeros(size1+size2+1);
    node * result_temp = result;
    
    node * result_temp_2;
    node * lhs_temp;
    
    while(rhs_temp){
        int overhead = 0;
        result_temp_2 = result_temp;
        lhs_temp = lhs;
        
        while(lhs_temp){
            
            int res = lhs_temp->data * rhs_temp->data + overhead;
            int newData = (result_temp_2->data + res) % 10;
            result_temp_2->data+=res%10;
            overhead=res/10 + result_temp_2->data/10;
            result_temp_2->data = newData;
            
            lhs_temp=lhs_temp->next;
            result_temp_2=result_temp_2->next;
        }
        if(overhead){
            result_temp_2->data+=overhead;
        }
        
        result_temp=result_temp->next;
        rhs_temp=rhs_temp->next;
    }
    
    
    freeBigInt(lhs);
    freeBigInt(rhs);
    reverse(&result);
    return removeLeadingZeros(result);
}


node * updateBigInt(string s){
    node * head = nullptr;
    for(int i=0; i<s.size(); i++){
        head=addNode(head, stoi(s.substr(i, 1)));
    }
    return head;
}


node * createBigInt(string fileName){
    ifstream input(fileName.c_str());
    stack<node *> myStack;
    string inp;
    while(input>>inp){
        if(inp!="*"){
            if(inp!="+")
                myStack.push(updateBigInt(inp));
        }
        else{
            node * left = nullptr;
            left = myStack.top();
            myStack.pop();
            string rightStr;
            node * right;
            input>> rightStr;
            node * rhs = updateBigInt(rightStr);
            node * result = multiplyBigInt(left, rhs);
            myStack.push(result);
        }
    }
    stack<node *> toBeDeleted=myStack;
    node * sum = nullptr;
    while(!myStack.empty()){
        node * rhs = nullptr;
        rhs = myStack.top();
        myStack.pop();
        sum = addBigInt(sum, rhs);
    }
    node * result = nullptr;
    result = copyAll(result, sum);
    freeBigInt(sum);
    while(!toBeDeleted.empty()){
        freeBigInt(toBeDeleted.top());
        toBeDeleted.pop();
    }
    return result;
}

int main(){
    
    node * result = createBigInt("input.txt");
    ofstream output("output.txt");
    node * temp = result;
    while(temp){
        output << temp->data;
        temp=temp->next;
    }
    return 0;
}

