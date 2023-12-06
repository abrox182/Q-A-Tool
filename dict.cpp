// #pragma once
#include "dict.h"

const int ALPHABET_SIZE= 128;

int binary_search1(vector<int>& arr, int val)
{
    int low =0; 
    int high = arr.size()-1;

    while(low<=high)
    {
        int mid = (high+low)/2;
        if(arr[mid]==val){return mid;}
        else if(arr[mid]<val)
        {low = mid +1;}
        else{high = mid -1;}
    }
    return -1;
}

vector<string> Dict::splitsentence(string line)
{
    vector<string> ans;
    string spch = " .,-:!()?[];@\'\"";
    //corresponding ascii values are 
    //32,46,44,45,58,33,40,41,63,91,93,59,64,39,34
    // vector<int> spchascii = {32, 33, 34, 39, 40, 41, 44, 45, 46, 58, 59, 63, 64, 91, 93};
    vector<int> spchascii;
    spchascii.push_back(32);
    spchascii.push_back(33);
    spchascii.push_back(34);
    spchascii.push_back(39);
    spchascii.push_back(40);
    spchascii.push_back(41);
    spchascii.push_back(44);
    spchascii.push_back(45);
    spchascii.push_back(46);
    spchascii.push_back(58);
    spchascii.push_back(59);
    spchascii.push_back(63);
    spchascii.push_back(64);
    spchascii.push_back(91);
    spchascii.push_back(93);
    
    string s="";
    int n = line.size();
    for(char ch: line)
    {
        int c = int(ch);
        if(binary_search1(spchascii, c)==-1)
        {
            s+= ch;
        }

        else
        {
            if(s!="")
            {ans.push_back(s);}
            s="";
        }
    }
    if(s!="")
    {ans.push_back(s);}
    return ans;
}

Dict:: Dict()
{
    rootque= new trynode;
}

Dict::~Dict() {
    // vector<TrieNode*> nodeStk;
    // nodeStk.push_back(root);

    // while (nodeStk.size() != 0) {
    //     TrieNode* curr = nodeStk.back();
    //     nodeStk.pop_back();

    //     for (int i=0; i < 128; i++) {
    //         if (curr->children[i] != nullptr) {
    //             nodeStk.push_back(curr->children[i]);
    //         }
    //     }

    //     delete curr;
    // }
}


trynode* Dict::node_of_word_p2(string word)           //change name to something better
{
    trynode* current = rootque;
    for(char c : word)
    {
        // cout<<"char : "<<c<<endl;
        //assuming the word already exists in the dictionary
        int idx = tolower(c);
        // cout<<"idx "<<idx<<endl;
        if (current->children2[idx]!=NULL){
            current = current->children2[idx];
        }
        else{
            return NULL;
        } 
    }
    if (current->wend==false){   //not sure
        return NULL;
    }
    return current;
    // float score = current->corpus_count+1;
    // score/= (current->generalcount+1);
    // infovec=current->paracount //remember to make it vector of infonode;
    // return score;
}


trynode* Dict::get_root_p2(){
    return rootque;
}

void Dict::insert_word_for_2(string word, int para, int bkcode, int pgno){
    trynode* current = rootque;
    for(char c: word)
    {
        int idx = tolower(c); ///doubt 
        // cout<<"count "<<count<<endl;
        // count++;
        if(!current->children2[idx])
        {
            current->children2[idx]= new trynode();
        }
        current = current->children2[idx];
        // cout<<"reached1"<<endl;
    }
    // cout<<"reached2"<<endl;
    current->wend= true;
    current->corcount++;
    // cout<<"reached2"<<endl;
    // cout<<para<<" "<<bkcode<<" "<<pgno<<endl;
    

    //check
    // if (current->paracount2.size()==0){
    //     current->npara++;
    // }
    if(current->paracount2.size()>0 && current->paracount2.back()->para_no==para && current->paracount2.back()->page_no==pgno && current->paracount2.back()->bookcode==bkcode)
    {
        // cout<<word<<endl;
        // cout<<current->paracount.back()->para_no<<" "<<current->paracount.back()->bookcode<<" "<<current->paracount.back()->page_no<<endl;
        // cout<<para<<" "<<bkcode<<" "<<pgno<<endl;
        current->paracount2.back()->freq++;
    }
    else 
    {
        current->npara++;
        // cout<<"word is "<<word<<" ";
        infonode* newpara = new infonode();
        newpara->para_no=para;
        newpara->bookcode= bkcode;
        newpara->page_no= pgno;
        // cout<<newpara->para_no<<" "<<newpara->bookcode<<" "<<newpara->page_no<<endl;
        newpara->freq=1;
        current->paracount2.push_back(newpara);
    }
}

void Dict::insert_sen_for_2(int book_code, int page, int paragraph, int sentence_no, string sentence){
    vector<string> words =  splitsentence(sentence);
    for(string s : words)
    {
        insert_word_for_2(s,paragraph, book_code, page);
    }

}


//FOR PARAGRAPH CONTENT

para_content_dict::para_content_dict(){
    content_root=new boco_root;
}

para_content_dict::~para_content_dict(){
    //make this destructor
}

void para_content_dict::insert_content(int book_code,int page,int para_no,string str){
    // boco_root* curr=content_root;
    // cout<<"insert_content"<<endl;
    if (content_root->children3.size()<book_code+1){
        // cout<<"insert_content"<<endl;
        content_root->children3.resize(book_code+1);        //optimize this resize step
        content_root->children3[book_code]=new pega_node();
    }
    // content_root->children3[book_code]=new pega_node();
    if (content_root->children3[book_code]->children4.size()<page+1){
        content_root->children3[book_code]->children4.resize(page+1);
        content_root->children3[book_code]->children4[page]=new para_node();
    }
    if (content_root->children3[book_code]->children4[page]->children5.size()<para_no+1){
        content_root->children3[book_code]->children4[page]->children5.resize(para_no+1);
        content_root->children3[book_code]->children4[page]->children5[para_no]=new content_node();
    }
    // content_root->children3[book_code]->children4[page]->children5[para_no]=new content_node();
    // cout<<"insert_content"<<endl;
    content_root->children3[book_code]->children4[page]->children5[para_no]->content+=str+"\n";
    // cout<<str<<endl;
}

string para_content_dict::getcontent(int book_code,int page,int para_no){
    return content_root->children3[book_code]->children4[page]->children5[para_no]->content;
}