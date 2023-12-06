#include <iostream>
#include <fstream>
#include "Node.cpp"
#include "dict.cpp"  

using namespace std;

struct pairs{
    public:
    int book_code;
    int page;
    int paragraph;
    float score;
};

class minheap{
    private:
        vector<pairs> heap;
        int size;
    public:
        minheap();
        void swap(int i, int j);
        void heapify();
        void heapify(int i);
        void insert(pairs val);
        pairs get_top();
        void pop();
};


struct bcode_pg_para{
    int bcode;
    int pg;
    int para;
};

class QNA_tool {

private:
    // Dict* main_dict;
    minheap* miheap;
    // int mhsize;
    // Node* root;
    // Node* tail;
    // You are free to change the implementation of this function
    bool is_gfilled;
    vector<string> splitsentence(string line);
    void query_llm(string filename, Node* root, int k, string API_KEY);

    // You can add attributes/helper functions here

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    int tparanum;  //total number of paragraphs for second part
    int last_boco;
    int last_pega;
    int last_para;
    void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res);
    Node* get_top_k_para_for_p2(string question, int k);
    void write_content(int book_code, int page, int paragraph);
    para_content_dict* content_dict;
    Dict* main_dict;
};