#!/bin/python3
# -*- coding: utf-8 -*-

import yaml
from inc.configs_collector import getParentDirAsPathlibPathInstance
from inc.configs_collector import recursivelyMergeAllYmlFiles
from inc.create_translations_csv import generate_godot_localization_file
from inc.create_translations_csv import save_data

if __name__ == "__main__":

    #
    # Collect recursively all '*.yml' files starting from main project directory
    # and create one big config data structure.
    #
    START_DIR           = getParentDirAsPathlibPathInstance()
    SKIP_DIRS_AND_FILES = {".vscode", "charGfx", "bin_archives", "merged_config_dump.yml"}
    database = recursivelyMergeAllYmlFiles( START_DIR, SKIP_DIRS_AND_FILES )

    #
    # Create godot localization file.
    #
    result = generate_godot_localization_file( database )
    save_data( result, "kielbasagd.csv" )









