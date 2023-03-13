
from inc.raw_file import RawFile

class CharpadMapLoader:

    def __init__(self):
        self.raw_file = RawFile()

    def load_file(self, path):
        if self.raw_file.load_file(path):
            return True
        return False

    def close(self):
        self.raw_file.close()

    def get_data(self):
        return self.raw_file.get_bytes(0, self.raw_file.get_file_size())

