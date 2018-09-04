class TestCoverage:
  coverage = [False] * 10000
class ConstructorError(Exception):
    def __init__(self, a,b,c,d):
        super(Exception, self).__init__(a + "/" + b + "/" + c + "/" + d)

class SequenceNode:
    start_mark = "test_start_mark"
    id = "test_id"

    value = []

class MappingNode():
    start_mark = "m_test_start_mark"
    id = "m_test_id"
    value = []

    

construct_object_calls = 0

test_key = 95374
test_val = 15420

class Sample10:
    def construct_object(self, obj):
        global construct_object_calls
        if construct_object_calls % 2 == 0:
            assert obj == test_key
        else:
            assert obj == test_val

        construct_object_calls += 1
        return obj

#START OF SAMPLE
    def construct_yaml_pairs(self, node):
        pairs = []
        yield pairs
        if not isinstance(node, SequenceNode):
            raise ConstructorError("while constructing pairs", node.start_mark,
                                   "expected a sequence, but found %s" % node.id, node.start_mark)
        for subnode in node.value:
            if not isinstance(subnode, MappingNode):
                raise ConstructorError("while constructing pairs", node.start_mark,
                                       "expected a mapping of length 1, but found %s" % subnode.id,
                                       subnode.start_mark)
            if len(subnode.value) != 1:
                raise ConstructorError("while constructing pairs", node.start_mark,
                                       "expected a single mapping item, but found %d items" % len(subnode.value),
                                       subnode.start_mark)
            key_node, value_node = subnode.value[0]
            key = self.construct_object(key_node)
            value = self.construct_object(value_node)
            pairs.append((key, value))
#END OF SAMPLE


s = Sample10()

def get_pairs(node):
    for i in s.construct_yaml_pairs(node):
        x = i

    return x

assert next(s.construct_yaml_pairs(MappingNode())) == []

try:
    get_pairs(MappingNode())
    assert False
except ConstructorError as e:
    assert e.message == "while constructing pairs/m_test_start_mark/expected a sequence, but found m_test_id/m_test_start_mark"

assert get_pairs(SequenceNode()) == []

m = SequenceNode()
m.value = [SequenceNode()]
m.value[0].start_mark = "s_test_start_mark"
m.value[0].id = "s_test_id"

try:
    get_pairs(m)
    assert False
except ConstructorError as e:
    assert e.message == "while constructing pairs/test_start_mark/expected a mapping of length 1, but found s_test_id/s_test_start_mark"

m.value = [MappingNode()]
m.value[0].value = [1,2,3,4,5,6]

try:
    get_pairs(m)
    assert False
except ConstructorError as e:
    assert e.message == "while constructing pairs/test_start_mark/expected a single mapping item, but found 6 items/m_test_start_mark"

sn = MappingNode()
sn.value = [(test_key, test_val)]
m.value = [sn, sn]
assert construct_object_calls == 0
assert get_pairs(m) == [(test_key, test_val), (test_key, test_val)]
assert construct_object_calls == 4
