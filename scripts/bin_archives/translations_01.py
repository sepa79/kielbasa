#!/usr/bin/python3

import yaml
from inc.configs_collector import *
from deepmerge import always_merger      #, merge_or_raise

if __name__ == "__main__":

    SKIP_DIRS = {".vscode", "charGfx", "ble.yml"}

    kielbasa = getParentDirAsPathlibPathInstance()
    tdict = {}

    for file in kielbasa.rglob("*.yml"):
        if set(file.parts).isdisjoint(SKIP_DIRS):
            print(file)
            tmp_dict = read_yml_config_file(file)
            always_merger.merge(tdict, tmp_dict)

    yaml_string = yaml.dump(tdict, allow_unicode=True)
    fh = open("ble.yml", "w", encoding="utf-8")
    fh.write(yaml_string)
    fh.close()









