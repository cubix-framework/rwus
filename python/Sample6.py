from __future__ import print_function

class TestCoverage:
  coverage = [False] * 10000


import sys

class difflib:
    def unified_diff(self, a, b, tofile, fromfile):
        assert a[0] == ['9lst\n', '10lst\n', '11lst\n'][0]
        assert b == ['10lst\n', '11lst\n', '12lst\n']
        assert tofile == "<Event id=420> '10' 20[0]"
        assert fromfile == "<Event id=92> '10' 20[0]"
        return ["myline1\n", "myline2\n"]

get_grouping_behavior_calls = 0
class event_manager(object):
    def get_grouping_behavior(cls,t):
        global get_grouping_behavior_calls
        get_grouping_behavior_calls += 1
        if get_grouping_behavior_calls == 1:
            assert t == cur_left
            return cur_left_grouping
        else:
            assert t == cur_right
            return cur_right_grouping


class sentry:
    pass


sys.modules["difflib"] = difflib()
sys.modules["sentry"] = object()
sys.modules["sentry.event_manager"] = event_manager()

class item:
    def __init__(self, id):
        self.id = id
        self._value = []

    def __getitem__(self, item):
        return self._value[item]

cur_left = None
cur_right = None
cur_left_grouping = None
cur_right_grouping = None

old_print = print

output_buf = ""
def print(s):
    global output_buf
    output_buf += str(s)

write_output_buf = ""
class MyStdout:
    def write(self, s):
        global write_output_buf
        write_output_buf += s

sys.stdout = MyStdout()

class six:
    @classmethod
    def text_type(cls, h):
        return str(h) + "lst"

#START OF SAMPLE
def print_unified_diff(left, right):
    from difflib import unified_diff
    from sentry.event_manager import (
        get_grouping_behavior,
    )

    left_id = left.id
    right_id = right.id

    left = get_grouping_behavior(left)
    right = get_grouping_behavior(right)

    if left == right:
        return

    if left[0] != right[0]:
        print('! Grouping behavior differs: %r vs %r' % (left[0], right[0]))
        return

    print('> Same grouping behavior: %r' % left[0])

    # These should only be fingerprints at this point

    left = left[1]
    right = right[1]

    left_fingerprint = [k[0] for k in left]
    right_fingerprint = [k[0] for k in right]
    if left_fingerprint != right_fingerprint:
        print('!! Different fingerprint algorithms: %r vs %r' % (left_fingerprint, right_fingerprint))
        return

    bits = left_fingerprint
    print('> Same fingerprint algorithm: %r' % bits)

    left = [k[1] for k in left]
    right = [k[1] for k in right]
    for idx, (a, b) in enumerate(zip(left, right)):
        bit = bits[idx]
        for ((a_key, a_hashes), (b_key, b_hashes)) in zip(a, b):
            if a_key != b_key:
                print('>> Different interfaces for %r: %r vs %r' % (bit, a_key, b_key))
                continue
            for idx, (a_hash, b_hash) in enumerate(zip(a_hashes, b_hashes)):
                a_hash = [six.text_type(h) + '\n' for h in a_hash]
                b_hash = [six.text_type(h) + '\n' for h in b_hash]
                a_file = '<Event id=%d> %r %r[%d]' % (left_id, bit, a_key, idx)
                b_file = '<Event id=%d> %r %r[%d]' % (right_id, bit, b_key, idx)
                for line in unified_diff(a_hash, b_hash, fromfile=a_file, tofile=b_file):
                    sys.stdout.write(line)
#END OF SAMPLE

cur_left, cur_right = item(92), item(420)
cur_left_grouping = item(6426)
cur_right_grouping = cur_left_grouping

print_unified_diff(cur_left, cur_right)
assert output_buf == ""

get_grouping_behavior_calls = 0
cur_right_grouping = item(42046)
cur_left_grouping._value = ["\"ttt\""]
cur_right_grouping._value = ["\"tttt\""]
print_unified_diff(cur_left, cur_right)
assert output_buf == "! Grouping behavior differs: '\"ttt\"' vs '\"tttt\"'"

get_grouping_behavior_calls = 0
output_buf = ""
cur_left_grouping._value = ["\"ttt\"", [("10", 11), ("10", 11)]]
cur_right_grouping._value = ["\"ttt\"", [("10", 11), ("11", 11)]]
print_unified_diff(cur_left, cur_right)
assert output_buf == "> Same grouping behavior: '\"ttt\"'!! Different fingerprint algorithms: ['10', '10'] vs ['10', '11']"

get_grouping_behavior_calls = 0
output_buf = ""
cur_left_grouping._value = ["deg", [("10", [(20,21)])]]
cur_right_grouping._value = ["deg", [("10", [(21,21)])]]
print_unified_diff(cur_left, cur_right)
assert output_buf == "> Same grouping behavior: 'deg'> Same fingerprint algorithm: ['10']>> Different interfaces for '10': 20 vs 21"

get_grouping_behavior_calls = 0
output_buf = ""
cur_left_grouping._value = ["deg", [("10", [(20,[[9,10,11]])])]]
cur_right_grouping._value = ["deg", [("10", [(20,[[10,11,12]])])]]
print_unified_diff(cur_left, cur_right)
assert output_buf == "> Same grouping behavior: 'deg'> Same fingerprint algorithm: ['10']"
assert write_output_buf == "myline1\nmyline2\n"

# Ideally, I should test on structures of length more than 1, but.....that sounds really painful
# I hate whoever wrote this code and designed this triply-nested data structure
