#include "Example.h"

#include <iostream>

using namespace std;

Example::Example(const vector<Attribute> &attributes, const vector<string> &values, const string &classification) : m_classification(classification) {
    for(unsigned int i = 0; i < values.size(); i++) {
        const string &name = attributes.at(i).getName();
        const string &value = values.at(i);
        
        m_values.insert({name, value});
    }
}

const string& Example::getClassification() const {
    return m_classification;
}

bool Example::matches(const Attribute &attribute, const string &value) const {
    auto search = m_values.find(attribute.getName());
    
    if(search != m_values.end()) {
        if(value == search->second) {
            return true;
        }
    }
    
    return false;
}