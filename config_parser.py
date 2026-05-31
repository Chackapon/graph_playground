import sys


def getConfigFile():
    with open("Makefile", "r") as makefile:
        return makefile.readline().split()[-1] # TODO make dynamic


# using yaml library
def getConfigData():
    from yaml import SafeLoader, load
    with open(getConfigFile(), "r") as config_file:
        return load(config_file, Loader=SafeLoader)

# TO not use yaml library
def filterOutComments(line_list: list):
    new_line = []
    for element in line_list:
        if '#' in element: break
        else: new_line.append(element)
    return new_line
def parseYAML(file_name: str):
    data = dict()
    with open(file_name, "r") as config_file:
        for line in config_file:
            no_comment = filterOutComments(line.split())
            if len(no_comment) != 0 and ':' in no_comment[0]:
                data[ no_comment[0].split(':')[0] ] = no_comment[1]
    return data

if __name__ == "__main__": # TODO error handling
    parseYAML(getConfigFile())
    assert len(sys.argv) == 2
    arg = sys.argv[1]
    # print( getConfigData()[arg] )
    print( parseYAML(getConfigFile())[arg] )