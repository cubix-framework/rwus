class TestCoverage:
  coverage = [False] * 10000
import os, inspect

test_obj = 7375173

cur_ofile = ""

enter_called = False
exit_called = False

test_readline = 5025677
test_encoding = 716754041

class openpy:
    @classmethod
    def detect_encoding(cls, r):
        global test_readLine, test_encoding
        assert r == test_readline
        return (test_encoding, None)

class Buf:
    readline = 0

    def __enter__(self):
        global enter_called
        assert enter_called == False
        enter_called = True
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        global exit_called
        assert exit_called == False
        exit_called = True

test_buf = Buf()
test_buf.readline = test_readline


class stdlib_io:
    @classmethod
    def open(cls, fil, mode):
        global cur_ofile, test_buf
        assert mode == 'rb'
        assert fil == cur_ofile
        t = test_buf
        test_buf = None
        return t

def find_file(obj):
    assert obj == test_obj
    return cur_ofile

#START OF SAMPLE
def get_encoding(obj):
    ofile = find_file(obj)
    if ofile is None:
        return None
    elif ofile.endswith(('.so', '.dll', '.pyd')):
        return None
    elif not os.path.isfile(ofile):
        return None
    else:
        with stdlib_io.open(ofile, 'rb') as buffer:
            encoding, lines = openpy.detect_encoding(buffer.readline)
        return encoding
#END OF SAMPLE

cur_ofile = None
assert get_encoding(test_obj) == None

cur_ofile = "ooeua.so"
assert get_encoding(test_obj) == None
cur_ofile = "a.dll"
assert get_encoding(test_obj) == None
cur_ofile = "b.pyd"
assert get_encoding(test_obj) == None

cur_ofile = "/"
assert get_encoding(test_obj) == None
cur_ofile = "oauauaou/uao/"
assert get_encoding(test_obj) == None
cur_ofile = "ThisDoesNotExist.py"
assert get_encoding(test_obj) == None

an_existing_file = inspect.getfile(inspect.currentframe())
cur_ofile = an_existing_file
assert get_encoding(test_obj) == test_encoding
assert enter_called
assert exit_called

