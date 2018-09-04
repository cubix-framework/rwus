import java.io.*;
import java.util.*;
import java.util.function.*;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}


public class Sample8 {

    private static int lineNo;

    private static class XContentParser {}

    private static final XContentParser testParser = new XContentParser();

    private static class XContentType {
        private static class JSON {
            private static XContent xContent() {
                assert lineNo == 0;
                lineNo++;
                return new XContent();
            }
        }
    }

    private static class XContent {
        private XContentParser createParser(String s) {
            assert lineNo == 1;
            lineNo++;
            assert s.equals( "{\n"+ "  \"test\" : \"foo\",\n"+ "  \"junk\" : 2\n"+ "}");
            return testParser;
        }
    }

    private static class ConstructingObjectParser<A, B> {

        private Function<String[], A> f;

        public ConstructingObjectParser(String x, boolean b, Function<String[], A> f) {
            assert lineNo == 2;
            lineNo++;
            assert "foo".equals(x);
            assert b == true;
            this.f = f;
        }

        public void declareString(Object a, ParseField p) {
            assert lineNo == 3;
            lineNo++;
            assert a == testConstructorArg;
            assert p.s.equals("test");
        }

        public A apply(XContentParser p, B matcher) {
            assert lineNo == 4;
            lineNo++;
            assert matcher == MATCHER;
            assert p == testParser;
            return f.apply(new String[]{"foo"});
        }
    }

    private static class ParseField {
        private String s;

        public ParseField(String s) {
            this.s = s;
        }
    }

    private static Object testConstructorArg = new Object();
    private static Object constructorArg() { return testConstructorArg; }

    private static class ParseFieldMatcherSupplier {}
    private final static ParseFieldMatcherSupplier MATCHER = new ParseFieldMatcherSupplier();

    private static void assertEquals(String a, String b) {
        assert lineNo == 5;
        lineNo++;
        assert "foo".equals(a);
        assert "foo".equals(b);
    }

#START OF SAMPLE    
    public void testIgnoreUnknownFields() throws IOException {
        XContentParser parser = XContentType.JSON.xContent().createParser(
                "{\n"
                        + "  \"test\" : \"foo\",\n"
                        + "  \"junk\" : 2\n"
                        + "}");
        class TestStruct {
            public final String test;
            public TestStruct(String test) {
                this.test = test;
            }
        }
        ConstructingObjectParser<TestStruct, ParseFieldMatcherSupplier> objectParser = new ConstructingObjectParser<>("foo", true, a ->
                new TestStruct((String) a[0]));
        objectParser.declareString(constructorArg(), new ParseField("test"));
        TestStruct s = objectParser.apply(parser, MATCHER);
        assertEquals(s.test, "foo");
    }
#END OF SAMPLE    

    public static void main(String[] args) throws Exception {
        new Sample8().testIgnoreUnknownFields();
        assert lineNo == 6;
    }
}