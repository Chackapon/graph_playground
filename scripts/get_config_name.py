import sys

def filterOutComments(line_list: list):
    new_line = []
    for element in line_list:
        if '#' in element: break
        else: new_line.append(element)
    return new_line

def getConfigFile( makefile_path = "Makefile", default_config = "config.yaml" ):
    with open(makefile_path, "r") as makefile:
        for line in makefile:
            if "CONFIG_FILE" in line:
                return filterOutComments( line.split('=') )[-1].strip()
        return default_config


if __name__ == "__main__":
    print( getConfigFile( sys.argv[1] ) )