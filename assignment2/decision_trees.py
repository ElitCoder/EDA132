import re
import math
import copy

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
                attribute_values[i] = create_dict(attributes[i])
                i += 1
            elif(line[:1] != "@" and line != "\n"):
                value_list = re.split(',', line[:len(line)-1])
                data.append(value_list)
                insert_values(attribute_values, value_list, attributes)
    return attributes, data, attribute_values

def insert_values(attribute_values, value_list, attributes):
    truth = value_list[len(value_list)-1]

    for x in range(0,len(value_list)-1):
        temp = value_list[x]
        attribute_values[x][attributes[x][0]][truth] += 1

        if not is_int(temp):
            attribute_values[x][temp][truth] +=1
        else:
            attribute_values[x]['real'][truth] += 1

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def create_dict(attribute_list):
    temp = {}
    for attribute in attribute_list:
        temp[attribute] = {'yes': 0, 'no': 0}
    return temp

def retrieve_values(attribute):
    values = []
    for x in range(0,len(attribute)):
        values.append(re.search('\w+', attribute[x]).group())
    return values

def decision_tree_learning(attributes, examples, attribute_values, parent_examples = None):
    if len(examples) == 0:
        return plurality_value(parent_examples)
    elif all_same_classification(examples):
        return classification(examples)
    elif len(attributes) == 0:
        return plurality_value(examples)
    else:
        #Value
        A = -1
        #Index
        A_index = -1
        
        for attribute in attributes:
            importance_val = importance(attributes, attribute, attribute_values)
            if importance_val > A:
                A_index = attribute
                A = importance_val
        temp = attributes[A_index]
        
        tree = {}
        tree[temp[0]] = {}
        #exs = []
        for k in range(1,len(temp)):
            exs = get_examples(examples, A_index, temp[k])
            attr = get_attributes(attributes, A_index)
            subtree = decision_tree_learning(attr, exs, attribute_values, examples)
            tree[temp[0]][temp[k]] = subtree
        return tree

def get_examples(examples, A_index, attribute):
    new_examples = []
    for example in examples:
        if example[A_index] == attribute:
            new_examples.append(example)
    return new_examples

def get_np(attribute, attributes_index, attribute_values):
    temp = attribute_values[attributes_index][attributes[attributes_index][0]]
    return temp['yes'], temp['no']

def importance(attribute, attributes_index, attribute_values):
    p, n = get_np(attribute, attributes_index, attribute_values)

    if p == 0 or n == 0:
        b = 0
    else:
        b = B_func(float(p)/(p+n))
    return b - remainder(attribute, attributes_index, attribute_values)

def get_attributes(attributes, A_index):
    new_attr = copy.deepcopy(attributes)
    del new_attr[A_index]
    return new_attr

def remainder(attributes, attributes_index, attribute_values):
    toreturn = 0
    temp = attribute_values[attributes_index][attributes[attributes_index][0]]
    p = temp['yes']
    n = temp['no']

    for k in range(1,len(attributes[attributes_index])):
        temp = attribute_values[attributes_index][attributes[attributes_index][k]]
        pk = temp['yes']
        nk = temp['no']
        if nk == 0 or pk == 0:
            continue
        div = float(pk)/(nk+pk)
        toreturn += ((pk+nk)*B_func(div))

    toreturn = toreturn/(p+n)
    return toreturn

def B_func(q):
    p = 1-q
    return -((q*math.log(q,2))+(p*math.log(p,2)))

def plurality_value(examples):
    return False

def classification(examples):
    return examples[0][-1]

def all_same_classification(examples):
    classification = examples[0][-1]
    for x in range(1, len(examples)):
        if not examples[x][-1] == classification:
            return False
    return True

def print_tree(tree):
    return None

if __name__ == '__main__':
    attributes, examples, attribute_values = parser(filename)
    del attributes[len(attributes)-1]
    tree = decision_tree_learning(attributes, examples, attribute_values)
    
    print("\nAttributes: \n%s" % attributes)
    print('----')
    print("Attribute-values: \n%s" % attribute_values)
    print('----')
    print("Examples: \n%s" % examples)