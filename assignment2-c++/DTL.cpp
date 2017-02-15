#include "DTL.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <numeric>
#include <algorithm>

using namespace std;

void String::replace(string &str, string replace, string with) {
    while(str.find(replace) != string::npos) {
        str.replace(str.find(replace), replace.size(), with);
    }
}

DTL::DTL(const std::string &filename) {
    read(filename);
}

Tree DTL::run() {
    return dtl(m_examples, m_attributes, vector<Example>());
}

vector<int> DTL::NP(const vector<Example> &examples) const {
    vector<int> np(m_classifications.size());
    
    for(const Example &example : examples) {
        for(unsigned int i = 0; i < m_classifications.size(); i++) {
            const string &classification = m_classifications.at(i);
            
            if(example.getClassification() == classification) {
                np.at(i)++;
                
                break;
            }
        }
    }
    
    return np;
}

vector<int> DTL::NP(const vector<Example> &examples, const Attribute &attribute, const string &value) const {
    vector<int> np(m_classifications.size());
    
    for(const Example &example : examples) {
        if(!example.matches(attribute, value)) {
            continue;
        }
        
        for(unsigned int i = 0; i < m_classifications.size(); i++) {
            const string &classification = m_classifications.at(i);
            
            if(example.getClassification() == classification) {
                np.at(i)++;
                
                break;
            }
        }
    }
    
    return np;
}

Tree DTL::dtl(const vector<Example> &examples, const vector<Attribute> &attributes, const vector<Example> &parentExamples) {    
    if(examples.empty()) {
        return Tree(pluralityValue(parentExamples));
    }
    
    else if(sameClassification(examples).length() > 0) {
        return Tree(sameClassification(examples));
    }
    
    else if(attributes.empty()) {
        return Tree(pluralityValue(examples));
    }
    
    const Attribute *A = nullptr;
    double bestInformation(-1);
    
    for(const Attribute &attribute : attributes) {
        if(attribute.isReal()) {
            continue;
        }
        
        double information = informationGain(attribute, examples);
        
        if(information > bestInformation) {
            bestInformation = information;
            A = &attribute;
        }
    }
    
    if(A == nullptr) {
        return Tree(pluralityValue(examples));
    }
    
    Tree tree(A->getName());
    
    for(const string &leaf : A->getValues()) {  
        auto exs = getMatchingExamples(examples, *A, leaf);
        vector<Attribute> attributesCopy(attributes);
        
        for(unsigned int j = 0; j < attributesCopy.size(); j++) {
            if(attributesCopy.at(j).getName() == A->getName()) {
                attributesCopy.erase(attributesCopy.begin() + j);
            }
        }
                        
        Tree subtree = dtl(exs, attributesCopy, examples);
        tree.add(leaf, subtree);
    }
        
    return tree;
}

vector<Example> DTL::getMatchingExamples(const vector<Example> &examples, const Attribute &attribute, const string &example) const {
    vector<Example> matches;
    
    for(const Example &e : examples) {
        if(e.matches(attribute, example)) {
            matches.push_back(e);
        }
    }
    
    return matches;
}

double DTL::B(vector<double> &q) const {
    double sum = 0.0;

    for(const double value : q) {
        if(value == 0) {
            continue;
        }
    
        sum += value*log2(value);
    }

    sum = sum * -1;
    
    return sum;
}

double DTL::remain(const Attribute &attribute, const vector<Example> &examples, const vector<int> &np) const {
    double sum = 0.0;
    int npSum = accumulate(np.begin(), np.end(), 0);
    
    for(const string &value : attribute.getValues()) {
        auto np_k = NP(examples, attribute, value);
        int np_kSum = accumulate(np_k.begin(), np_k.end(), 0);
        
        auto foundSum = find(np_k.begin(), np_k.end(), np_kSum);
        
        if(foundSum != np_k.end()) {
            continue;
        }
        
        vector<double> divnp;
        
        for(const int integer : np_k) {        
            double b = static_cast<double>(integer) / np_kSum;
            
            divnp.push_back(b);
        }
        
        sum += (np_kSum) * B(divnp);
    }
    
    sum /= static_cast<double>(npSum);
    
    return sum;
}

double DTL::informationGain(const Attribute &attribute, const vector<Example> &examples) const {
    auto np = NP(examples);
    int sum = accumulate(np.begin(), np.end(), 0);
    
    vector<double> divnp;
    
    for(const int value : np) {
        if(value == sum) {
            divnp.clear();
            
            break;
        }
        
        double b = static_cast<double>(value) / sum;        
        divnp.push_back(b);
    }
    
    double b = 0.0;
    
    if(!divnp.empty()) {
        b = B(divnp);
    }
    
    return b - remain(attribute, examples, np);
}

string DTL::sameClassification(const vector<Example> &examples) const {
    string classification = examples.at(0).getClassification();
    
    for(const Example &example : examples) {
        if(classification != example.getClassification()) {
            return "";
        }
    }
    
    return classification;
}

string DTL::pluralityValue(const vector<Example> &examples) const {
    vector<int> classifications(m_classifications.size());
    
    for(const Example &example : examples) {
        for(unsigned int i = 0; i < m_classifications.size(); i++) {
            const string &classification = m_classifications.at(i);
            
            if(example.getClassification() == classification) {
                classifications.at(i)++;
            }
        }
    }
    
    auto indexMax = max_element(classifications.begin(), classifications.end());
    const string &maxValue = m_classifications.at(distance(classifications.begin(), indexMax));
    
    return maxValue;
}

void DTL::read(const std::string &filename) {
    ifstream file(filename);
    
    if(!file) {
        cout << "Error opening file.\n";
        
        return;
    }
    
    string temp;
    
    while(getline(file, temp)) {
        if(temp.length() == 0 || temp[0] == '%' || temp.find("@relation") != string::npos) {
            continue;
        }
        
        if(temp == "@data") {
            break;
        }
        
        temp.erase(0, temp.find(" ") + 1);
        
        string name = temp.substr(0, temp.find(" "));
        vector<string> values;
        
        temp.erase(0, temp.find(" ") + 1);
        
        if(temp.find("real") == string::npos) {
            temp.erase(0, 1);
            temp.erase(temp.length() - 1, 1);
            
            while(temp.length() > 0) {
                auto commaPosition = temp.find(",");
                
                if(commaPosition == string::npos) {
                    values.push_back(temp);
                                        
                    break;
                }
                
                string value = temp.substr(0, commaPosition);
                temp.erase(0, commaPosition + 2);
                                
                values.push_back(value);
            }
        }
        
        m_attributes.push_back(Attribute(name, values));
    }
    
    const vector<string> &classifications = m_attributes.back().getValues();
    
    m_classifications = classifications;
    
    m_attributes.pop_back();
    
    while(getline(file, temp)) {
        if(temp.length() == 0 || temp[0] == '%') {
            continue;
        }
        
        String::replace(temp, ",", " ");
        
        vector<string> values;
        istringstream ss(temp);
        
        for(unsigned int i = 0; i < m_attributes.size(); i++) {
            ss >> temp;
            values.push_back(temp);
        }
        
        ss >> temp;
        
        m_examples.push_back(Example(m_attributes, values, temp));
    }
    
    file.close();
}