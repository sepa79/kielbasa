#!/usr/bin/python3

import os
import pathlib
import yaml
from yaml.loader import SafeLoader
from deepmerge import always_merger      #, merge_or_raise

def recursivelyMergeAllYmlFiles(startDir, skipDirs):

    tdict = {}
    for file in startDir.rglob("*.yml"):
        if set(file.parts).isdisjoint(skipDirs):
            print(file)
            tmp_dict = read_yml_config_file(file)
            always_merger.merge(tdict, tmp_dict)

    #
    # DEBUG
    # uncomment to save resulting dictionary to "yaml" file
    #

    # yaml_string = yaml.dump(tdict, allow_unicode=True)
    # fh = open("merged_config_dump.yml", "w", encoding="utf-8")
    # fh.write(yaml_string)
    # fh.close()

    return tdict

def getParentDirAsPathlibPathInstance():
    cwd = os.getcwd()
    main_dir = os.path.dirname(cwd)
    return pathlib.Path(main_dir)

def read_yml_config_file( filename ):
    fh = open(filename, "r", encoding="utf-8")
    content = yaml.load(fh, Loader=SafeLoader)
    fh.close()
    return content








