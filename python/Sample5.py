class TestCoverage:
  coverage = [False] * 10000
lineNo = 0

test_model = 64262462

cur_start = 0
cur_end = 0

cur_safe_start = 0

test_kw_key = 'auohs'
test_kw_value = 464208
test_kwargs = {test_kw_key : test_kw_value}

cur_start_key = 0
cur_end_key = 0

class Key:
    parent__ = 0
    app__ = 0
    namespace__ = 0
    kind__ = 0

    def parent(self):
        return self.parent__

    def app(self):
        return self.app__

    def namespace(self):
        return self.namespace__

    def kind(self):
        return self.kind__

    @classmethod
    def from_path(cls, kind, num, namespace, _app, parent):
        global lineNo
        assert kind == test_key.kind__
        assert namespace == test_key.namespace__
        assert _app == test_key.app__
        assert parent == test_key.parent__

        if lineNo == 2:
            lineNo += 1
            assert  num == cur_start
            return cur_start_key
        else:
            assert lineNo == 3
            lineNo += 1
            assert num == cur_end
            return cur_end_key

test_key = Key()
test_key.parent__ = 642052
test_key.app__ = 2427536
test_key.namespace__ = 67232
test_key.kind__ = 42356

def _coerce_to_key(model):
    global lineNo
    assert model == test_model
    assert lineNo == 0
    lineNo += 1
    return test_key

class Datastore:
    def NormalizeAndTypeCheck(self, startEndTuple, ab_tuple):
        global lineNo
        assert type(startEndTuple) == tuple
        assert len(startEndTuple) == 2
        start = startEndTuple[0]
        end = startEndTuple[1]

        assert type(ab_tuple) == tuple
        assert len(ab_tuple) == 2
        a = ab_tuple[0]
        b = ab_tuple[1]

        assert start == cur_start
        assert end == cur_end
        assert a == int
        assert b == long
        assert lineNo == 1
        lineNo += 1

    def AllocateIds(self, key, **kwargs):
        assert key == test_key
        assert kwargs[test_kw_key] == test_kw_value
        assert kwargs['max'] == cur_end
        return cur_safe_start, None

datastore = Datastore()

cur_collision = False

class Query:
    def __init__(self, namespace, keys_only, _app):
        assert keys_only == True
        assert namespace == test_key.namespace()
        assert _app == test_key.app__

    def filter(self, str, k):
        global lineNo
        if lineNo == 4:
            assert str == '__key__ >='
            assert k == cur_start_key
            lineNo += 1
        else:
            assert lineNo == 5
            lineNo += 1
            assert str == '__key__ <='
            assert k == cur_end_key

        return self

    def fetch(self, n):
        assert n == 1
        return cur_collision

class BadArgumentError(Exception):
    def __init__(self, msg):
        self.msg = msg

KEY_RANGE_COLLISION = 17357364
KEY_RANGE_CONTENTION = 64202053
KEY_RANGE_EMPTY = 20372401

#START OF SAMPLE
def allocate_id_range(model, start, end, **kwargs):
    key = _coerce_to_key(model)
    datastore.NormalizeAndTypeCheck((start, end), (int, long))
    if start < 1 or end < 1:
        raise BadArgumentError('Start %d and end %d must both be > 0.' %
                               (start, end))
    if start > end:
        raise BadArgumentError('Range end %d cannot be less than start %d.' %
                               (end, start))

    safe_start, _ = datastore.AllocateIds(key, max=end, **kwargs)


    race_condition = safe_start > start

    start_key = Key.from_path(key.kind(), start, parent=key.parent(),
                              _app=key.app(), namespace=key.namespace())
    end_key = Key.from_path(key.kind(), end, parent=key.parent(),
                            _app=key.app(), namespace=key.namespace())
    collision = (Query(keys_only=True, namespace=key.namespace(), _app=key.app())
                 .filter('__key__ >=', start_key)
                 .filter('__key__ <=', end_key).fetch(1))

    if collision:
        return KEY_RANGE_COLLISION
    elif race_condition:
        return KEY_RANGE_CONTENTION
    else:
        return KEY_RANGE_EMPTY
#END OF SAMPLE


try:
    lineNo = 0
    cur_start, cur_end = -5,17
    allocate_id_range(test_model, cur_start, cur_end, **test_kwargs)
    assert False
except BadArgumentError as e:
    assert e.msg == "Start -5 and end 17 must both be > 0."
    assert lineNo == 2


try:
    lineNo = 0
    cur_start, cur_end = 5,-17
    allocate_id_range(test_model, cur_start, cur_end, **test_kwargs)
    assert False
except BadArgumentError as e:
    assert e.msg == "Start 5 and end -17 must both be > 0."
    assert lineNo == 2


try:
    lineNo = 0
    cur_start, cur_end = 42,18
    allocate_id_range(test_model, cur_start, cur_end, **test_kwargs)
    assert False
except BadArgumentError as e:
    assert e.msg == "Range end 18 cannot be less than start 42."
    assert lineNo == 2


cur_start_key = 2420736
cur_end_key = 54273
cur_start, cur_end = 1, 2

lineNo = 0
cur_collision = True
assert allocate_id_range(test_model, cur_start, cur_end, **test_kwargs) == KEY_RANGE_COLLISION
assert lineNo == 6

lineNo = 0
cur_safe_start = 2
cur_collision = True
assert allocate_id_range(test_model, cur_start, cur_end, **test_kwargs) == KEY_RANGE_COLLISION
assert lineNo == 6

lineNo = 0
cur_safe_start = 2
cur_collision = False
assert allocate_id_range(test_model, cur_start, cur_end, **test_kwargs) == KEY_RANGE_CONTENTION
assert lineNo == 6

lineNo = 0
cur_safe_start = 0
cur_collision = False
assert allocate_id_range(test_model, cur_start, cur_end, **test_kwargs) == KEY_RANGE_EMPTY
assert lineNo == 6
