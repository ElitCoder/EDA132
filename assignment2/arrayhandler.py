def get_attribute(attribute_list, attribute_index):
    return attribute_list[attribute_index]

def get_class_of_example(example):
    return example[len(example)-1]

def get_first_class(examples):
    return examples[0][-1]

def remove_class_attribute(attributes):
    del attributes[len(attributes)-1]

if __name__ == '__main__':
    a = [2,3,4,5,6,7,8,9,0]
    xyz = [0,12,4,6,242,7,9]
    for x in filter(lambda w: w in a, xyz):
      print(x)
