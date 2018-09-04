class TestCoverage:
  coverage = [False] * 10000

class MissingArgumentError(Exception):
    def __init__(self, name):
        self._name = name

cur_arguments = None
test_name = "ddagdaou"
test_source = "hoauxbntgdao"
cur_strip = True

class Sample9:

    _ARG_DEFAULT = 90102

    def _get_arguments(self, name, source, dummy_kw=123, strip=None):
        assert name == test_name
        assert source == test_source
        assert dummy_kw == 123
        assert strip == cur_strip
        return cur_arguments

#START OF SAMPLE
    def _get_argument(self, name, default, source, strip=True):
        args = self._get_arguments(name, source, strip=strip)
        if not args:
            if default is self._ARG_DEFAULT:
                raise MissingArgumentError(name)
            return default
        return args[-1]
#END OF SAMPLE

s = Sample9()

cur_strip = True
cur_arguments = [202,20,1029]
assert s._get_argument(test_name, None, test_source) == 1029
assert s._get_argument(test_name, None, test_source, cur_strip) == 1029
cur_strip = False
assert s._get_argument(test_name, None, test_source, cur_strip) == 1029

cur_arguments = None

try:
    s._get_argument(test_name, s._ARG_DEFAULT, test_source, cur_strip)
    assert False
except MissingArgumentError as e:
    assert e._name == test_name

assert s._get_argument(test_name, 204, test_source, cur_strip) == 204
