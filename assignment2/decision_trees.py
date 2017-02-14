import re
import math
import copy
import arrayhandler as ah

filename = "data/weather.arff"
CONST_TAB = "    "
CONST_YES = 'yes'
CONST_NO = 'no'
number_list = ['real', 'integer', 'double']

def parser(filename_input):
    attributes = {}
    data = []
    attribute_values = {}
    with open(filename_input, 'r') as outfile:
        i = 0
        for line in outfile:
            if(line[:2] == "@a"):
                attribute = re.split(' ', line[11:])
                attributes[i] = retrieve_values(attribute)
                i += 1

            elif(line == "@data\n"):
                create_attr_values(attribute_values, attributes)
            elif(line[:1] != "\n" and line[:1] != "@"):
                example = re.split(',', line.strip())
                data.append(example)
                insert_values(attribute_values, example, attributes)
    
    return attributes, data, attribute_values

def create_attr_values(attribute_values, attributes):
    for index in attributes:
        attribute_values[index] = create_dict(attributes[index], attributes)

def insert_values(attribute_values, example, attributes):
    classification = example[len(example)-1]
    
    for x in range(0,len(example)-1):
        if not attributes.get(x):
            continue
            
        attribute_value_example = example[x]
        attribute_list = attribute_values[x]
        
        attribute = attributes[x][0]
        attribute_classifications = attribute_list[attribute]
        
        attribute_classifications[classification] += 1
        
        if not is_int(attribute_value_example):
            attribute_list[attribute_value_example][classification] +=1
        else:
            attribute_list['real'][classification] += 1

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def create_dict(attribute_list, attributes):
    temp = {}
    for attribute in attribute_list:
        temp[attribute] = {CONST_NO:0, CONST_YES:0}

    return temp

def retrieve_values(attribute):
    values = []
    for x in range(0,len(attribute)):
        temp_attr = ""
        temp_attr += attribute[x].strip().strip(',').strip('{').strip('}')
        values.append(temp_attr)
    return values

def only_has_leaf(tree):
    #print(tree)
    
    for index in tree:
        if(index == 'posneg'):
            continue
        
        if tree[index] == CONST_NO or tree[index] == CONST_YES:
            print("")#LEAF")
            
        else:
            return False
    return True
    
def chi_pruning(tree):
    if only_has_leaf(tree):
        if hypothesis(tree):
            print("")
            
    else:
        for index in tree:
            if(index == 'posneg'):
                continue
            
            if tree[index] == CONST_NO or tree[index] == CONST_YES:
                continue
                
            chi_pruning(tree[index])

def hypothesis(tree):
    print("hypothesis")
    #print(tree)
    tree_posneg = tree['posneg']
    v = sum(tree_posneg[x] for x in tree_posneg)
    
    p = tree_posneg[CONST_YES]
    n = tree_posneg[CONST_NO]
    
    p_k = tree[index]['posneg'][CONST_YES]
    n_k = tree[index]['posneg'][CONST_NO]
    
    p_hat1 = p * (p_k / float(p + n))
    p_hat2 = p * (n_k / float(p + n))
    
    n_hat1 = n * (p_k / float(p + n))
    n_hat2 = n * (n_k / float(p + n))
    
    delta = ((p_k - p_hat1)**2 / p_hat1) + ((n_hat1)**2 / n_hat1) + ((p_hat2)**2 / p_hat2) + ((n_k - n_hat2)**2 / n_hat2)
    
    
    
    return True
    
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
            importance_val = importance(attributes, attribute, attribute_values, examples)
            if importance_val > A_value:
                A_index = attribute
                A_value = importance_val
        attribute_value_list = attributes[A_index]
        
        tree = create_tree_root(attribute_value_list)
        
        for k in range(1,len(attribute_value_list)):
            exs = get_examples(examples, A_index, attribute_value_list[k])
            attr = get_attributes(attributes, A_index)
            attribute_values_temp = {}
            create_attr_values(attribute_values_temp, attr)
            #print(attribute_values)
            for example in exs:
                insert_values(attribute_values_temp, example, attr)

            #print(attribute_values_temp)
            subtree = decision_tree_learning(attr, exs, attribute_values_temp, examples)
            tree[attribute_value_list[0]][attribute_value_list[k]] = subtree
            tree[attribute_value_list[0]]['posneg'] = attribute_values[A_index][attributes[A_index][0]]
            #tree[attribute_value_list[0]]['posneg'] = attribute_values_temp[A_index]
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

def importance(attributes, attributes_index, attribute_values, examples):
    p, n = get_np(attributes, attributes_index, attribute_values)

    if p == 0 or n == 0:
        b = 0
    else:
        b = B_func(float(p)/(p+n))
    return b - remainder(attributes, attributes_index, attribute_values)

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
    number_yes = 0
    for example in examples:
        number_yes += example[-1].count(CONST_YES)
    value = CONST_YES if (number_yes >= (len(examples)/float(2))) else CONST_NO
    result = {value:value, 'posneg':{'yes':number_yes, 'no':len(examples)-number_yes}}
    print(result)
    return result

def classification(examples):
    return examples[0][-1]

def all_same_classification(examples):
    classification = examples[0][-1]
    for x in range(1, len(examples)):
        if not examples[x][-1] == classification:
            return False
    return True

def print_tree(tree, depth = ''):
    tree_string = ""
    if(tree == CONST_NO or tree == CONST_YES):
        return ' : %s\n' % tree
    for attribute in tree:
        tree_string = tree_string + "\n"
        for value in tree[attribute]:
            if(value == 'posneg'):
                continue
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
    ah.remove_class_attribute(attribute_values)

    #debug_print(attributes, attribute_values, examples)
    tree = decision_tree_learning(attributes, examples, attribute_values)
    
    #chi_pruning(tree)
    #chi_pruning(tree, attributes, examples)
    print(tree)
    #print(print_tree(tree))