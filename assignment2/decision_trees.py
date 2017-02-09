import re
import math

filename = "data/weather.arff"


def parser(filename_input):
    attributes = {}
    data = []
    attribute_values = {}
    #{'outlook':['sunny','overcast','rainy']}
    with open(filename_input, 'r') as outfile:
        i = 0
        for line in outfile:
            if(line[:2] == "@a"):
                attribute = re.split(' ', line[11:])
                attributes[i] = retrieve_values(attribute)

                #attributes[attribute[0]] = retrieve_values(attribute)
                attribute_values[i] = [0]*((len(attributes[i])-1)*2)
                i += 1
            elif(line[:1] != "@" and line != "\n"):
                value_list = re.split(',', line[:len(line)-1])
                data.append(value_list)

                index_to_add = decision_index(value_list[len(value_list)-1])

                #connect_values(attribute_values, data, index_to_add)

    return attributes, data, attribute_values

def connect_values(attribute_values, data, index_to_add):
    for
        index = attribute_values.index()

def decision_index(input):
    if input == 'yes':
        return 0
    return 1

def retrieve_values(attribute):
    values = []
    for x in range(0,len(attribute)):
        values.append(re.search('\w+', attribute[x]).group())
    return values

def decision_tree_learning(examples, attributes, parent_examples = None):
    if len(examples) == 0:
        return plurality_value(parent_examples)
    elif all_same_class(examples):
        return classification(examples)
    elif len(attributes) == 0:
        return plurality_value(examples)
    else:
        return None

def importance(attribute, examples):

    return None

def get_attribute():
    return None

def remainder(attributes, p, n):
    remainder = 0

    for x in range(0,len(attributes)):
        attributes

    remainder = remainder/(p+n)
    return None

def B_func(q):
    p = 1-q
    return -((q*log(q,2))+(p*log(p,2)))

def plurality_value(examples):
    return False

def classification(examples):
    return False

def all_same_class(examples):
    return False

if __name__ == '__main__':
    attributes, examples, attribute_values = parser(filename)
    print("Attributes: \n%s" % attributes)
    print('----')
    print("Attribute-values: \n%s" % attribute_values)
    print('----')
    print("Examples: \n%s" % examples)
    decision_tree_learning(examples, attributes)
