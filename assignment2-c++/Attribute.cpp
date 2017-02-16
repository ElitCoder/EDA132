#include "Attribute.h"

using namespace std;

Attribute::Attribute(const string &name, const vector<string> &values) : m_name(name), m_values(values) {
    m_real = values.empty();
}

const string& Attribute::getName() const {
    return m_name;
}

const vector<string>& Attribute::getValues() const {
    return m_values;
}

bool Attribute::isReal() const {
    return m_real;
}