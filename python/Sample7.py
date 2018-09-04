class TestCoverage:
  coverage = [False] * 10000
test_read_delimiter = [240,24040,661910,790101]

check_max_bytes_outer_called = False
check_max_bytes_inner_called = False

cur_loc_list = []
cur_loc = None

class BufferItem:
    def find(self, delim):
        global cur_loc
        assert delim == test_read_delimiter
        x = cur_loc_list.pop()
        if x == -1 and len(cur_loc_list) == 0:
            s._loop_done = True

        cur_loc = x
        return x

    def __len__(self):
        return 42012096

class ReadBuffer:

    def __getitem__(self, item):
        assert item == 0
        return BufferItem()

    def __len__(self):
        return len(cur_loc_list) + 1

test_read_buffer = ReadBuffer()

def _double_prefix(buf):
    assert buf == s._read_buffer
    s._double_prefix_calls += 1

class ReResItem:
    _end = -2

    def __init__(self, m):
        self._end = m

    def end(self):
        return self._end

class Regex:
    def search(self, buf):#
        assert buf.__class__ == BufferItem
        global cur_loc
        x = cur_loc_list.pop()
        cur_loc = x
        if x == -1 and len(cur_loc_list) == 0:
            s._loop_done = True

        if x == -1:
            return None
        else:
            return ReResItem(x)

class Sample7:
    _read_bytes = None
    _read_buffer_size = 0
    _read_partial = 0

    _read_delimiter = None
    _read_regex = None
    _read_buffer = False

    _loop_done = True

    _double_prefix_calls = 0

    def _check_max_bytes(self, delimiter, n):
        if self._loop_done:
            global check_max_bytes_outer_called
            if self._read_delimiter is not None:
                assert delimiter == self._read_delimiter
            else:
                assert delimiter == self._read_regex
            assert n == len(self._read_buffer[0])
            check_max_bytes_outer_called = True
        else:
            global check_max_bytes_inner_called
            assert check_max_bytes_inner_called == False
            if self._read_delimiter is not None:
                assert delimiter == self._read_delimiter
                assert n == cur_loc + len(delimiter)
                check_max_bytes_inner_called = True
            else:
                assert delimiter == self._read_regex
                assert n == cur_loc
                check_max_bytes_inner_called = True

#START OF SAMPLE
    def _find_read_pos(self):
        if (self._read_bytes is not None and
                (self._read_buffer_size >= self._read_bytes or
                     (self._read_partial and self._read_buffer_size > 0))):
            num_bytes = min(self._read_bytes, self._read_buffer_size)
            return num_bytes
        elif self._read_delimiter is not None:
            if self._read_buffer:
                while True:
                    loc = self._read_buffer[0].find(self._read_delimiter)
                    if loc != -1:
                        delimiter_len = len(self._read_delimiter)
                        self._check_max_bytes(self._read_delimiter,
                                              loc + delimiter_len)
                        return loc + delimiter_len
                    if len(self._read_buffer) == 1:
                        break
                    _double_prefix(self._read_buffer)
                self._check_max_bytes(self._read_delimiter,
                                      len(self._read_buffer[0]))
        elif self._read_regex is not None:
            if self._read_buffer:
                while True:
                    m = self._read_regex.search(self._read_buffer[0])
                    if m is not None:
                        self._check_max_bytes(self._read_regex, m.end())
                        return m.end()
                    if len(self._read_buffer) == 1:
                        break
                    _double_prefix(self._read_buffer)
                self._check_max_bytes(self._read_regex,
                                      len(self._read_buffer[0]))
        return None
#END OF SAMPLE


s = Sample7()

s._read_bytes = 53
s._read_buffer_size = 65
s._read_partial = False

assert s._find_read_pos() == 53

s._read_buffer_size = 42
s._read_partial = True
assert s._find_read_pos() == 42

s._read_delimiter = None
s._read_regex = None
s._read_partial = False
assert s._find_read_pos() == None

s._read_partial = True
s._read_buffer_size = -1
assert s._find_read_pos() == None

s._read_bytes = -2
assert s._find_read_pos() == -2

s._read_buffer_size = -1
s._read_bytes = 0

assert check_max_bytes_outer_called == False

s._read_delimiter = test_read_delimiter
s._read_buffer = None
assert s._find_read_pos() == None

s._read_delimiter = None
s._read_regex = Regex()
assert s._find_read_pos() == None

s._read_delimiter = test_read_delimiter
s._read_buffer = test_read_buffer

cur_loc_list = [17]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == len(test_read_delimiter) + 17
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False


cur_loc_list = [-1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == None
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called == False
assert check_max_bytes_outer_called == True

cur_loc_list = [-1, 17]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == len(test_read_delimiter) + 17
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False

cur_loc_list = [17, -1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == len(test_read_delimiter) + 17
assert s._double_prefix_calls == 1
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False

cur_loc_list = [-1, -1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == None
assert s._double_prefix_calls == 1
assert check_max_bytes_inner_called == False
assert check_max_bytes_outer_called == True

s._read_delimiter = None
s._read_regex = Regex()



cur_loc_list = [17]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == 17
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False


cur_loc_list = [-1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == None
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called == False
assert check_max_bytes_outer_called == True

cur_loc_list = [-1, 17]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == 17
assert s._double_prefix_calls == 0
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False

cur_loc_list = [17, -1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == 17
assert s._double_prefix_calls == 1
assert check_max_bytes_inner_called
assert check_max_bytes_outer_called == False

cur_loc_list = [-1, -1]
s._loop_done = False
check_max_bytes_outer_called = False
check_max_bytes_inner_called = False
s._double_prefix_calls = 0
assert s._find_read_pos() == None
assert s._double_prefix_calls == 1
assert check_max_bytes_inner_called == False
assert check_max_bytes_outer_called == True
