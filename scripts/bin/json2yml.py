#!/bin/python3
# -*- coding: utf-8 -*-
import json
import yaml

# no error checking, be carefull
def read_json_config_file( filename ):
    fh = open(filename, "r", encoding="utf-8")
    content = json.load(fh)
    fh.close()
    return content

if __name__ == "__main__":

    # read json config file
    config = read_json_config_file( "data.json" )

    # convert to yaml
    yaml_string = yaml.dump(config, allow_unicode=True)

    # write to file
    fh = open("data.yml", "w", encoding="utf-8")
    fh.write(yaml_string)
    fh.close()





