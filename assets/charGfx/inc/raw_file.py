
from os import SEEK_END
from struct import unpack
from codecs import decode
from codecs import encode

class RawFile:

    def __init__( self ):
        self.fsock    = None
        self.filesize = None

    def load_file( self, path ):
        try:
            self.fsock = open( path, 'rb', 0)
            self.fsock.seek( 0, SEEK_END)
            self.filesize = self.fsock.tell()
            self.fsock.seek( 0, 0 )
        except IOError:
            return False
        return True

    def close( self ):
        self.fsock.close()

    def get_bytes( self, index, length ):
        self.fsock.seek( index, 0 )
        array = []
        for pos in range( length ):
            array.append( self.get_unsigned_byte( index + pos ) )
        return array

    def get_unsigned_word( self, index ):
        self.fsock.seek( index, 0 )
        return unpack( '<H', self.fsock.read( 2 ) )[ 0 ]

    def get_unsigned_byte( self, index ):
        self.fsock.seek( index, 0 )
        return unpack( '<B', self.fsock.read( 1 ) )[ 0 ]

    def get_file_size( self ):
        return self.filesize

