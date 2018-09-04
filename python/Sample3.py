class TestCoverage:
  coverage = [False] * 10000
lineNo = 0

class ScrapyCommand:
    @classmethod
    def add_options(cls, s3, parser):
        global test_parser, lineNo, test_sample3
        assert s3 == test_sample3
        assert parser == test_parser
        assert lineNo == 0
        lineNo += 1

class Parser:
    contents = []

    def add_option(self, switch, metavar, help, dest):
        global lineNo
        lineNo += 1
        self.contents.append((switch, metavar, help, dest))

class Sample3:
#START OF SAMPLE
    def add_options(self, parser):
        ScrapyCommand.add_options(self, parser)
        parser.add_option("--get", dest="get", metavar="SETTING",
                          help="print raw setting value")
        parser.add_option("--getbool", dest="getbool", metavar="SETTING",
                          help="print setting value, interpreted as a boolean")
        parser.add_option("--getint", dest="getint", metavar="SETTING",
                          help="print setting value, interpreted as an integer")
        parser.add_option("--getfloat", dest="getfloat", metavar="SETTING",
                          help="print setting value, interpreted as a float")
        parser.add_option("--getlist", dest="getlist", metavar="SETTING",
                          help="print setting value, interpreted as a list")
#END OF SAMPLE


test_sample3 = Sample3()
test_parser = Parser()

test_sample3.add_options(test_parser)
assert lineNo == 6

assert test_parser.contents == [ ("--get", "SETTING", "print raw setting value", "get"),
                                 ("--getbool", "SETTING", "print setting value, interpreted as a boolean", "getbool"),
                                 ("--getint", "SETTING", "print setting value, interpreted as an integer", "getint"),
                                 ("--getfloat", "SETTING", "print setting value, interpreted as a float", "getfloat"),
                                 ("--getlist", "SETTING", "print setting value, interpreted as a list", "getlist")
                            ]
