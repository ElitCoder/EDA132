#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>

class Attribute {
public:
    Attribute(const std::string &name, const std::vector<std::string> &values);
    
    const std::vector<std::string>& getValues() const;
    const std::string& getName() const;
    bool isReal() const;
private:
    std::string m_name;
    bool m_real;

    std::vector<std::string> m_values;
};

#endif