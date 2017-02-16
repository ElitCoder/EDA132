#include "DTL.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "Please specify input file\n";
        
        return -1;
    }
    
    string filename(argv[1]);
    
    DTL dtl(filename);
    Tree tree = dtl.run();
    
    cout << "Decision tree:\n\n";
    cout << tree;
}