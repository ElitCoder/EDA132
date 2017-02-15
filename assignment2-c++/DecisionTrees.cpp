#include "DTL.h"

#include <iostream>

using namespace std;

int main() {
    string filename("data/restaurant.arff");
    
    DTL dtl(filename);
    Tree tree = dtl.run();
    
    cout << tree;
}