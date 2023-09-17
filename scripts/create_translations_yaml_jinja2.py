#!/bin/python3
# -*- coding: utf-8 -*-

import yaml
from inc.configs_collector import getParentDirAsPathlibPathInstance
from inc.configs_collector import recursivelyMergeAllYmlFiles
from inc.create_translations_yaml import create_common_h_file_jinja2
from inc.create_translations_yaml import create_lang_files_jinja2

if __name__ == "__main__":

    #
    # Collect recursively all '*.yml' files starting from main project directory
    # and create one big config data structure.
    #
    START_DIR           = getParentDirAsPathlibPathInstance()
    SKIP_DIRS_AND_FILES = {".vscode", "charGfx", "bin_archives", "merged_config_dump.yml"}
    config = recursivelyMergeAllYmlFiles( START_DIR, SKIP_DIRS_AND_FILES )

    #
    # Create 'common.h' file from "config" data structure
    #
    create_common_h_file_jinja2( config, "common_h.j2", "../translation/common.h" )

    #
    # Create given language translation '*.c' and '*.h' files.
    #
    # create_lang_files_jinja2 (
    #       data_structure, language_tag, template_c_file, template_h_file, output_filename_prefix
    # )
    #
    create_lang_files_jinja2( config, "pl", "texts_c.j2", "texts_h.j2", "../translation/textsPL" )
    create_lang_files_jinja2( config, "en", "texts_c.j2", "texts_h.j2", "../translation/textsEN" )









