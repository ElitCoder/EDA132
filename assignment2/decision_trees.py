import re
import math
import copy
import arrayhandler as ah

filename = "data/weather.arff"
CONST_TAB = "    "
CONST_YES = 'yes'
CONST_NO = 'no'
number_list = ['real', 'integer', 'double']
#TODO
#classes, not only yes/no - Think this is fixed!
#? - how to use it
#plurality_value
#handle real_values
#ArrayHandler for clarification

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
                example = re.split(',', line[:len(line)-1])
                data.append(example)
                insert_values(attribute_values, example, attributes)

    return attributes, data, attribute_values


    #Goes through the example and picks out classification for each. 
    #Add this to the correct attribute & the attributes value in the example
    #(e.g. 'sunny')
    #TODO
    #Fix "else" (only real now, wtf?)
def insert_values(attribute_values, example, attributes):
    classification = example[len(example)-1]
    
    for x in range(0,len(example)-1):
        attribute_value_example = example[x]
        attribute_list = attribute_values[x]
        attribute = attribute_list[attributes[x][0]]
        
        attribute[classification] += 1

        if not is_int(attribute_value_example):
            attribute_list[attribute_value_example][classification] +=1
        else:
            attribute_list['real'][classification] += 1     #'real' contains value rather than one out of set strings

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def create_dict(attribute_list):
    temp = {}
    for attribute in attribute_list:
        temp[attribute] = {CONST_YES: 0, CONST_NO: 0}
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
        A_value = -1
        A_index = -1

        for attribute in attributes:
            importance_val = importance(attributes, attribute, attribute_values)
            if importance_val > A_value:
                A_index = attribute
                A_value = importance_val
        attribute_value_list = attributes[A_index]
        #print(temp)
        
        tree = create_tree_root(attribute_value_list)
        
        for k in range(1,len(attribute_value_list)):
            exs = get_examples(examples, A_index, attribute_value_list[k])
            attr = get_attributes(attributes, A_index)
            subtree = decision_tree_learning(attr, exs, attribute_values, examples)
            tree[attribute_value_list[0]][attribute_value_list[k]] = subtree
        return tree

def create_tree_root(root):
    tree = {}
    tree[root[0]] = {}
    return tree

def get_examples(examples, A_index, attribute):
    new_examples = []
    for example in examples:
        if example[A_index] == attribute:
            new_examples.append(example)
    return new_examples

def get_np(attribute, attributes_index, attribute_values):
    temp = attribute_values[attributes_index][attributes[attributes_index][0]]
    return temp[CONST_YES], temp[CONST_NO]

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
    p = temp[CONST_YES]
    n = temp[CONST_NO]

    for k in range(1,len(attributes[attributes_index])):
        temp = attribute_values[attributes_index][attributes[attributes_index][k]]
        pk = temp[CONST_YES]
        nk = temp[CONST_NO]
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
    #According to http://stackoverflow.com/questions/15643055/what-is-plurality-classification-in-decision-trees
    number_yes = 0
    for example in examples:
        number_yes += example[-1].count(CONST_YES)
    return CONST_YES if (number_yes >= (len(examples)/float(2))) else CONST_NO

def classification(examples):
    return examples[0][-1]

def all_same_classification(examples):

    classification = examples[0][-1]
    for x in range(1, len(examples)):
        if not examples[x][-1] == classification:
            return False
    return True

#Builds a string
def print_tree(tree, depth = ''):
    tree_string = ""
    if(tree == CONST_NO or tree == CONST_YES):
        return ' : %s\n' % tree
    for attribute in tree:
        tree_string = tree_string + "\n"
        for value in tree[attribute]:
            tree_string = tree_string + depth +  attribute + " = " + value
            tree_string = tree_string + print_tree(tree[attribute][value], depth + CONST_TAB)
    return tree_string

def debug_print(attributes, attribute_values, examples):
    print("\nAttributes: \n%s" % attributes)
    print('----')
    print("Attribute-values: \n%s" % attribute_values)
    print('----')
    print("Examples: \n%s" % examples)

if __name__ == '__main__':
    attributes, examples, attribute_values = parser(filename)
    ah.remove_class_attribute(attributes)

    #debug_print(attributes, attribute_values, examples)
    tree = decision_tree_learning(attributes, examples, attribute_values)
    #print(tree)
    print(print_tree(tree))