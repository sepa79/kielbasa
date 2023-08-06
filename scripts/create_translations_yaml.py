#!/bin/python3
# -*- coding: utf-8 -*-

import yaml
from inc.configs_collector import getParentDirAsPathlibPathInstance
from inc.configs_collector import read_yml_config_file
from deepmerge import always_merger      #, merge_or_raise
from inc.create_translations_yaml import create_common_h_file
from inc.create_translations_yaml import create_pl_lang_files
from inc.create_translations_yaml import create_lang_files


def process_configs(SKIP_DIRS):

    kielbasa = getParentDirAsPathlibPathInstance()
    tdict = {}

    for file in kielbasa.rglob("*.yml"):
        if set(file.parts).isdisjoint(SKIP_DIRS):
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

if __name__ == "__main__":

    SKIP_DIRS = {".vscode", "charGfx", "ble.yml"}

    config = process_configs(SKIP_DIRS)

    # read config file
    # config = read_yml_config_file( "data.yml" )

    # create common.h file basing on "config" data structure
    dst_file_name   = "common.h"
    dot_h_template  = "templates/common_h.template"
    create_common_h_file( dst_file_name, dot_h_template, config )

    # create textsPL.c and textsPL.h files
    dot_c_template = "templates/texts_c_pl.template"
    dot_h_template = "templates/texts_h_pl.template"
    dst_dot_c_file = "textPL.c"
    dst_dot_h_file = "textPL.h"
    create_pl_lang_files( dot_c_template, dot_h_template, config )

    # create files from data.yml with given language 
    # if it exists in 'data.yml'
    create_lang_files( config, "en" )









