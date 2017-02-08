import re

filename = "data/weather.arff"


def parser(filename_input):
    attributes = {}
    data = []
    #{'outlook':['sunny','overcast','rainy']}
    with open(filename_input, 'r') as outfile:
        for line in outfile:
            if(line[:2] == "@a"):
                attribute = re.split(' ', line[11:])
                attributes[attribute[0]] = retrieve_values(attribute)
            elif(line[:1] != "@" and line != "\n"):
                data.append(re.split(',', line[:len(line)-1]))
    return attributes, data

def retrieve_values(attribute):
    values = []
    for x in range(1,len(attribute)):
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

def importance_test(attribute, examples):
    return None

def plurality_value(examples):
    return False

def classification(examples):
    return False

def all_same_class(examples):
    return False

if __name__ == '__main__':
    attributes, examples = parser(filename)
    decision_tree_learning(examples, attributes)
