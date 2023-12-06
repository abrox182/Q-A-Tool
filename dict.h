// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class infonode{
    public:
    int bookcode;
    int para_no;
    int page_no;
    int freq;

    infonode(int bkcode, int parano, int pageno, int count)
    {
        bookcode = bkcode; para_no=parano; page_no= pageno; freq= count;
    }

    infonode()
    {
        bookcode=0; para_no=0; page_no=0; freq=0;
    }
};

class trynode{
    public:
    trynode* children2[128];  //doubt 
    long corcount;
    vector<infonode*> paracount2;       //I'll put infonode in this
    int npara; //number of unique paragraphs in which it is coming
    bool wend;

    trynode(){
        for (int i = 0; i < 128; i++) {
            children2[i] = nullptr;
        }
        corcount=0;
        npara=0;
        wend= false;
    }
};

class Dict {
private:
    // You can add attributes/helper functions here
    trynode* rootque;

public: 
    
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();


    
    vector<string>  splitsentence(string line);

    trynode* get_root_p2();

    void insert_sen_for_2(int book_code, int page, int paragraph, int sentence_no, string sentence);

    void insert_word_for_2(string word, int para, int bkcode, int pgno);

    trynode* node_of_word_p2(string word);

    //int get_word_count(string word);
    //long countCorpusWords(string word);

    //long countCSVWords(string word);

    //TrieNode* scoreWords(string word);

    /* -----------------------------------------*/
};

//FOR SECOND PART
// To store the content of paragraphs in a trie

// that node which will be stored in para_content_dict dictionary
class content_node{
    public:
    string content;
};

class para_node{
    public:
    vector<content_node*> children5;
};

class pega_node{
    public:
    vector<para_node*> children4;
    // children4.resize(400);
};

class boco_root{
    public:
    // pega_node* children3[100];       //abhi 98 isliye kyuki i am assuming mahatma gandhi corpus denge;
    vector<pega_node*> children3;
    // children3.resize(100);
};

class para_content_dict {
private:
    // You can add attributes/helper functions here
    boco_root* content_root;

public: 
    
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    para_content_dict();

    ~para_content_dict();

    void insert_content(int book_code,int page,int para_no,string str);

    string getcontent(int book_code,int page,int para_no);

    /* -----------------------------------------*/
};

