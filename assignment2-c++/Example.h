#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "Attribute.h"

#include <vector>
#include <string>
#include <unordered_map>

class Example {
public:
    Example(const std::vector<Attribute> &attributes, const std::vector<std::string> &values, const std::string &classification);
    
    const std::string& getClassification() const;
    bool matches(const Attribute &attribute, const std::string &value) const;
    
private:
    std::unordered_map<std::string, std::string> m_values;
    std::string m_classification;
};

#endif