#!/usr/bin/python3

import sys
from inc.charpad_map_loader import CharpadMapLoader
import random

TILE_TO_REPLACE = 0x82
TILE_COUNT = 10
MAP_HORIZONTAL_SIZE = 64

def print_info(data):
    print("data :  ", data)

def save(fileName, data):
    with open(fileName, "wb") as fh:
        fh.write( bytearray(data) )
        fh.close()
    return

def replace_remove_repeats(dataArray, tileCount):
    startValue = TILE_TO_REPLACE
    endValue   = TILE_TO_REPLACE + tileCount - 1
    for index, value in enumerate(dataArray):
        newValue = random.randint(startValue,endValue)
        if value == TILE_TO_REPLACE:
            gatheredTiles = [
                    dataArray[index - 1],
                    dataArray[index - MAP_HORIZONTAL_SIZE - 1],
                    dataArray[index - MAP_HORIZONTAL_SIZE + 0],
                    dataArray[index - MAP_HORIZONTAL_SIZE + 1]
                    ]
            while newValue in gatheredTiles:
                newValue = random.randint(startValue,endValue)
            dataArray[index] = newValue
    # return dataArray

if __name__ == "__main__":

    # verify arguments count
    args = sys.argv
    if len( args ) != 2:
        print()
        print( "application takes 1 argument, example:" )
        print()
        print( "> %s filename.bin" % (args[0]) )
        print()
        exit()

    # take file name from arguments
    fileName = args[1]
    grassTileCount = TILE_COUNT

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

    # replace with removed repeats
    replace_remove_repeats(data, grassTileCount)
    # print(data)

    # save results to file
    # newFileName = fileName.replace(".bin", " - grass %s.bin" % (grassTileCount))
    save(fileName, data)







