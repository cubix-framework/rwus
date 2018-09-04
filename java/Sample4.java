class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample4 {

    private static boolean isFiltered = false;
    private static boolean isFiltered() { return isFiltered; }

    private static class Filter {
        public JsonGenerator getDelegate() {
            return testDelegate;
        }
    }
    private static Filter filter = new Filter();

    private static class JsonGenerator {}

    private static JsonGenerator testDelegateDelegate = new JsonGenerator();

    private static class JsonGeneratorDelegate extends  JsonGenerator {
        public JsonGenerator getDelegate() {
            return testDelegateDelegate;
        }
    }
    private static JsonGenerator generator = new JsonGenerator();
    private static JsonGenerator testDelegate;


#START OF SAMPLE    
    private JsonGenerator getLowLevelGenerator() {
        if (isFiltered()) {
            JsonGenerator delegate = filter.getDelegate();
            if (delegate instanceof JsonGeneratorDelegate) {
                // In case of combined inclusion and exclusion filters, we have one and only one another delegating level
                delegate = ((JsonGeneratorDelegate) delegate).getDelegate();
                assert delegate instanceof JsonGeneratorDelegate == false;
            }
            return delegate;
        }
        return generator;
    }
#END OF SAMPLE    

    public static void main(String[] args) {
        Sample4 s = new Sample4();

        isFiltered = false;
        assert s.getLowLevelGenerator() == generator;

        isFiltered = true;
        testDelegate = new JsonGenerator();
        assert s.getLowLevelGenerator() == testDelegate;
        testDelegate = new JsonGeneratorDelegate();
        assert s.getLowLevelGenerator() == testDelegateDelegate;
    }
}