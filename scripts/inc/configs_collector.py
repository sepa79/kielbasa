#!/usr/bin/python3

import os
import pathlib
import yaml
from yaml.loader import SafeLoader

def getParentDirAsPathlibPathInstance():
    cwd = os.getcwd()
    main_dir = os.path.dirname(cwd)
    return pathlib.Path(main_dir)

def read_yml_config_file( filename ):
    fh = open(filename, "r", encoding="utf-8")
    content = yaml.load(fh, Loader=SafeLoader)
    fh.close()
    return content








