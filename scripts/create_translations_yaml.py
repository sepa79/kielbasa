#!/bin/python3
# -*- coding: utf-8 -*-

import yaml
from inc.configs_collector import getParentDirAsPathlibPathInstance
from inc.configs_collector import recursivelyMergeAllYmlFiles
from inc.create_translations_yaml import create_common_h_file
from inc.create_translations_yaml import create_lang_files

if __name__ == "__main__":

    #
    # Collect recursively all '*.yml' files starting from main project directory
    # and create one big config data structure from them.
    #
    START_DIR           = getParentDirAsPathlibPathInstance()
    SKIP_DIRS_AND_FILES = {".vscode", "charGfx", "ble.yml", "bin_archives"}
    config = recursivelyMergeAllYmlFiles( START_DIR, SKIP_DIRS_AND_FILES )

    #
    # Create 'common.h' file baseing on "config" data structure
    #
    create_common_h_file( config, "templates/common_h.template", "../translation/common.h" )

    #
    # Create Polish translation files.
    #
    # Polish translation is used as base, so output files are unique.
    #
    dot_c_template  = "templates/texts_c_pl.template"
    dot_h_template  = "templates/texts_h_pl.template"
    create_lang_files( config, "pl", dot_c_template, dot_h_template )

    #
    # Set templates for other languages.
    #
    dot_c_template  = "templates/texts_c.template"
    dot_h_template  = "templates/texts_h.template"

    #
    # Create given language translation files.
    #
    create_lang_files( config, "en", dot_c_template, dot_h_template )









