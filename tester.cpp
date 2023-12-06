#include<bits/stdc++.h>
#include "Node.h"
// #include "qna_tool.h" 
#include "qna_tool.cpp"

using namespace std;

int main(){

    QNA_tool qna_tool;

    int num_books = 98;

    for(int i = 1; i <= num_books; i++){

        std::cout << "Inserting book " << i << std::endl;

        std::string filename = "gandhi/mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();

    }

    string question = "Who was Mahatma?";
    // string question = "What were Gandhi's views on the manner of how one should eat?";
    // string question2 = "hshgdjahsgus";
    // string question3 = "what were the views of mahatma on partition?";

    /*
        Part-1 : Get the top-k paragraphs
        Checking Correctness
    */

    // Let's try to ask a simple question to the qna_tool
    // Node* head = qna_tool.get_top_k_para_for_p2(question, 10);
    // // Node* head2 = qna_tool.get_top_k_para(question2, 5);
    // // Node* head3 = qna_tool.get_top_k_para(question3, 5);
    // vector<Node*> result;
    // result.push_back(head);
    // // result.push_back(head2);
    // // result.push_back(head3);
    // for (int i=0;i<result.size();i++){
    //     if (result[i]!=NULL){
    //         cout << "Result of Query "<<i+1<<" :" << endl;
    //         Node* curr=result[i];
    //         while (curr != NULL) {
    //             cout <<" Book Code: " << curr->book_code << ", Page: " << curr->page
    //             << ", Paragraph: " << curr->paragraph << endl;
    //             curr = curr->right;
    //         }
    //     }
    //     else{
    //         cout << "No Result of query "<<i+1<< endl;
    //     }
    // }
    // vector<string> paras;
    // while(head != nullptr){
    //     string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //     paras.push_back(res);
    //     head = head->right;
    // }

    // for(int i = 0; i < (int)paras.size(); i++){
    //     cout << paras[i] << endl << endl << endl;
    // }

    /*
        Part-2 : Query the LLM
    */
    qna_tool.query(question, "out.txt");
    
    return 0;
}