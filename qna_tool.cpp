// #pragma once
#include <assert.h>
#include <sstream>
#include <cmath>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"
// #define CONTENT_FILE "content.txt"
#define CSVFILENAME "unigram_freq.csv"
#define API_KEY "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"
#define API_FILENAME "api_call_fastchat.py"

minheap::minheap(){
    size=0;
}

int get_left(int i) {   
    return 2 * i + 1;
}

int get_right(int i) {
    return 2 * i + 2;
}

int get_parent(int i) {
    return (i - 1) / 2;
}

void minheap::swap(int i, int j) {
    pairs temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void minheap::heapify(int i) {
   int smallest = i;
   int left = get_left(i);
   int right = get_right(i);

   if (left < size && heap[left].score < heap[smallest].score) {
       smallest = left;
   }

   if (right < size && heap[right].score < heap[smallest].score) {
       smallest = right;
   }

   if (smallest != i) {
       swap(i, smallest);
       heapify(smallest);
   }
}


void minheap::insert(pairs val) {
   heap.push_back(val);
   size++;
   int i = size - 1;
   while (i != 0 && heap[get_parent(i)].score > heap[i].score) {
       swap(i, get_parent(i));
       i = get_parent(i);
   }
}

pairs minheap::get_top() {
    if (size == 0) {
        pairs val = {-1, -1, -1, -1.0};
        return val;
    }
    return heap[0];
}

void minheap::pop() {
    if (size == 0) {
        return;
    }
    heap[0] = heap[size - 1];
    heap.pop_back();
    size--;
    heapify(0);
}

void minheap::heapify() {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(i);
    }
}

QNA_tool::QNA_tool(){
    main_dict=new Dict();
    is_gfilled=false;
    miheap=new minheap();
    tparanum=0;
    last_boco=-1;
    last_pega=-1;
    last_para=-1;
    content_dict=new para_content_dict();
    // Implement your function here  
}


//MAKE THE DESTRUCTORS
QNA_tool::~QNA_tool(){
    
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    main_dict->insert_sen_for_2(book_code,page,paragraph,sentence_no,sentence);
    content_dict->insert_content(book_code,page,paragraph,sentence);
    if (last_boco==book_code && last_pega==page && last_para==paragraph){
        return;
    }
    tparanum++;
}

int binary_search(vector<int>& arr, int val)
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

vector<string> QNA_tool::splitsentence(string line)
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
        if(binary_search(spchascii, c)==-1)
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

Node* QNA_tool::get_top_k_para_for_p2(string question, int k){
    

    vector<vector<vector<float> > > parascore;      // stores book_code then page then paragraph_no.
    vector<bcode_pg_para> keep_track;
    int mhsize=0;
    vector<string> que_words=splitsentence(question);
    float k1=0.5;   //tuning parameter;
    for (int i=0;i<que_words.size();i++){
        trynode* tnode=main_dict->node_of_word_p2(que_words[i]);
        if (tnode==NULL){
            // cout<<"hi"<<endl;
            continue;
        }
        if (que_words[i].length()<=3 && tnode->corcount>80000){
            continue;
        }
        // int df=tnode->npara;
        // float idf=std::log(tparanum/(1+tnode->npara));      // +1 is added to deal with division by zero

        //second formula of idf:  try it out
        float idf=std::log((tparanum-tnode->npara + 0.5)/(tnode->npara+0.5));

        for (int j=0;j<tnode->paracount2.size();j++){
            int boco= tnode->paracount2[j]->bookcode; //book code
            int pega = tnode->paracount2[j]->page_no;
            int paran=tnode->paracount2[j]->para_no;
            int freq=tnode->paracount2[j]->freq;
            if (parascore.size()<boco+1){   //not sure if +1 aayega yaa nahi
                parascore.resize(boco+1);
            }
            if (parascore[boco].size()<pega+1){
                parascore[boco].resize(pega+1);
            }
            if (parascore[boco][pega].size()<paran+1){
                parascore[boco][pega].resize(paran+1);
            }
            parascore[boco][pega][paran]+=((idf*(freq*(k1+1)))/(freq+k1));       // if you change the value of k1 then change the value of k1+1 too ie not 2.5 anymore 
            bcode_pg_para new_ele={boco,pega,paran};
            keep_track.push_back(new_ele);
        }
    }
    // Now we have filled parascore and keep_track vectors
    // cout<<"size "<<keep_track.size()<<endl;
    // cout<<"size "<<mhsize<<endl;
    for (int i=0;i<keep_track.size();i++){
        int boco=keep_track[i].bcode;
        int pega=keep_track[i].pg;
        int paran=keep_track[i].para;
        if (parascore[boco][pega][paran]==-1){
            continue;
        }
        if (mhsize<k){
            pairs mh_ele={boco,pega,paran,parascore[boco][pega][paran]};
            parascore[boco][pega][paran]=-1;
            miheap->insert(mh_ele);
            mhsize++;
        }
        else{
            pairs top_ele=miheap->get_top();
            if (top_ele.score<parascore[boco][pega][paran]){
                miheap->pop();
                pairs mh_ele={boco,pega,paran,parascore[boco][pega][paran]};
                parascore[boco][pega][paran]=-1;
                miheap->insert(mh_ele);
            }
        }
    }
    // cout<<"size "<<mhsize<<endl;
    Node* root_hehe=NULL;
    Node* tail_hehe=NULL;
   
    while (mhsize>0){
        // cout<<"hi"<<endl;
        pairs top_ele=miheap->get_top();
        miheap->pop();
        mhsize--;
        if (root_hehe==NULL){
            // cout<<top_ele.book_code<<" "<<top_ele.page<<" "<<top_ele.paragraph<<" "<<top_ele.score<<endl;
            root_hehe=new Node(top_ele.book_code,top_ele.page,top_ele.paragraph,-1,-1);
            // root_hehe->right=NULL;
            tail_hehe=root_hehe;
            continue;
        }
        // if ((top_ele.book_code==83 && top_ele.page==355) || (top_ele.book_code==29 && top_ele.page==58)){
        // cout<<top_ele.book_code<<" "<<top_ele.page<<" "<<top_ele.paragraph<<" "<<top_ele.score<<endl;
        // }
        Node* new_node=new Node(top_ele.book_code,top_ele.page,top_ele.paragraph,-1,-1);
        root_hehe->left=new_node;
        new_node->right=root_hehe;
        root_hehe=new_node;
    }
    // }
    return root_hehe;
}

void QNA_tool::write_content(int book_code, int page, int paragraph){
    // cout<<"hi"<<endl;
    std::ofstream outputFile("paragraphs.txt", std::ios::app);
    string cont=content_dict->getcontent(book_code,page,paragraph);
    // cout<<"hi"<<endl;
    outputFile<<cont<<"\n";
    outputFile.close();
}

void make_file(string question){
    std::ofstream outputFile2("question.txt", std::ios::app);
    outputFile2<<question<<"\n";
    outputFile2.close();
}


// void QNA_tool::query(string question, string filename){
//     int k =50; //change it accordingly
//     Node* top_paras=get_top_k_para_for_p2(question,k);
//     std::ofstream outputFile("context.txt");
//     std::ofstream outputFile2("question.txt");
//     make_file(question);
//     while (top_paras!=NULL){
//         // cout<<"hi"<<endl;
//         write_content(top_paras->book_code,top_paras->page,top_paras->paragraph);
//         top_paras=top_paras->right;
//     }
//     std::cout << "Q: " << question << std::endl;
//     std::cout << "A: " << "Studying COL106 :)" << std::endl;
//     return;
// }

string get_keywords(string question) {

    std::ofstream outputFile("initial_query.txt", std::ios::app);
    outputFile<<question<<"\n";
    outputFile.close();
    
    string command = "python3";
    command += " ";
    command += "api_call_keywords.py";

    system(command.c_str());

    std::ifstream outputFile2("keywords.txt", std::ios::app);
    string key;
    getline(outputFile2, key);
    
    return key;
}

void QNA_tool::query(string question, string filename){

    int k = 5;
    if (API_FILENAME=="api_call_vertex.py"){
        k = 15;
    }

    // augment the question
    std::ofstream outputFile3("initial_query.txt");
    string aug_q = question + get_keywords(question);

    Node* top_paras=get_top_k_para_for_p2(aug_q,k);

    std::ofstream outputFile("paragraphs.txt");
    std::ofstream outputFile2("question.txt");
    make_file(question);
    while (top_paras!=NULL){
        // cout<<"hi"<<endl;
        write_content(top_paras->book_code,top_paras->page,top_paras->paragraph);
        top_paras=top_paras->right;
    }

    query_llm(filename,top_paras,k,API_KEY);
    return;
}

void QNA_tool::get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res){

    std::ifstream inputFile(FILENAME);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[4];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph) &&
            (metadata[3] == sentence_no)
        ){
            res = sentence;
            return;
        }

        res = "$I$N$V$A$L$I$D$";
        return;
    }

}

void QNA_tool::query_llm(string filename, Node* root, int k, string apik){

    string command = "python3 ";
    command += API_FILENAME;
    command += " ";
    command += filename;

    system(command.c_str());
    return;
}
