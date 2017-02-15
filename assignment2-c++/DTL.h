#ifndef DTL_H
#define DTL_H

#include "Attribute.h"
#include "Example.h"
#include "Tree.h"

#include <string>
#include <vector>

namespace String {
    void replace(std::string &str, std::string replace, std::string with);
}

class DTL {
public:
    DTL(const std::string &filename);
    
    Tree run();
    
private:
    void read(const std::string &filename);
    Tree dtl(const std::vector<Example> &examples, const std::vector<Attribute> &attributes, const std::vector<Example> &parentExamples);
    std::string pluralityValue(const std::vector<Example> &examples) const;
    std::string sameClassification(const std::vector<Example> &examples) const;
    double informationGain(const Attribute &attribute, const std::vector<Example> &examples) const;
    std::vector<Example> getMatchingExamples(const std::vector<Example> &examples, const Attribute &attribute, const std::string &example) const;
    std::vector<int> NP(const std::vector<Example> &examples) const;
    std::vector<int> NP(const std::vector<Example> &examples, const Attribute &attribute, const std::string &value) const;
    double B(std::vector<double> &q) const;
    double remain(const Attribute &attribute, const std::vector<Example> &examples, const std::vector<int> &np) const;

    std::vector<Attribute> m_attributes;
    std::vector<Example> m_examples;
    std::vector<std::string> m_classifications;
};

#endif