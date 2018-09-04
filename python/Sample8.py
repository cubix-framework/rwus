class TestCoverage:
  coverage = [False] * 10000
import sys
import os

class Sample8:

    modulefilename = None

#START OF SAMPLE
    def get_module_name(self):
        basename = os.path.basename(self.modulefilename)
        basename = basename.split('.', 1)[0]
        if basename.endswith('_d') and hasattr(sys, 'gettotalrefcount'):
            basename = basename[:-2]
        return basename
#END OF SAMPLE


s = Sample8()

def test(val, res):
    s.modulefilename = val
    assert s.get_module_name() == res

test("bcd", "bcd")
test("/aou/uaou/bcd", "bcd")
test("/ouo/auuou/bcd.sthsht.st", "bcd")
test("bcd_d", "bcd_d")

sys.gettotalrefcount = 1
test("bcd_d", "bcd")
