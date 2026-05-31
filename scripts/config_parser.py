import sys
from get_config_name import getConfigFile, filterOutComments


# without yaml library
def parseConfig(file_name: str):
    data = dict()
    with open(file_name, "r") as config_file:
        for line in config_file:
            no_comment = filterOutComments(line.split())
            if len(no_comment) != 0 and ':' in no_comment[0]:
                data[ no_comment[0].split(':')[0] ] = no_comment[1]
    return data

if __name__ == "__main__": # TODO error handling
    parseConfig(getConfigFile())
    assert len(sys.argv) == 2
    arg = sys.argv[1]
    print(parseConfig(getConfigFile())[arg])