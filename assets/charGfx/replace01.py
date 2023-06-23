#!/usr/bin/python3

import sys
from inc.charpad_map_loader import CharpadMapLoader
import random

TILE_TO_REPLACE = 0x31

def print_info(data):
    print("data :  ", data)

def save(fileName, data):
    with open(fileName, "wb") as fh:
        fh.write( bytearray(data) )
        fh.close()
    return

def replace(dataArray, tileCount):
    startValue = TILE_TO_REPLACE
    endValue   = TILE_TO_REPLACE + tileCount - 1
    for index, value in enumerate(dataArray):
        newValue = random.randint(startValue,endValue)
        if value == TILE_TO_REPLACE:
            dataArray[index] = newValue
    # return dataArray

if __name__ == "__main__":

    # verify arguments count
    args = sys.argv
    if len( args ) != 3:
        print()
        print( "application takes 2 arguments, example:" )
        print()
        print( "> %s filename.bin 5" % (args[0]) )
        print()
        exit()

    # take file name from arguments
    fileName = args[1]
    grassTileCount = int(args[2])

    # init charpad map loader
    loader = CharpadMapLoader()

    # show error and exit if loader can't load file
    if not loader.load_file(fileName):
        print("error loading file...   ", fileName)
        exit()

    # get data from loader
    data = loader.get_data()

    # close loader
    loader.close()

    # show loaded data
    # print_info(data)

    # replace with range
    replace(data, grassTileCount)
    print(data)

    # save results to file
    newFileName = fileName.replace(".bin", " - grass %s.bin" % (grassTileCount))
    save(newFileName, data)







