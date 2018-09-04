import java.util.*;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample6 {
    private static final int TEST_DOC_COUNT = 10;

    private static int lineNo = 0;

    private static final Object testPrepareCreateRes = new Object();

    private static void assertAcked(Object o) {
        assert o == testPrepareCreateRes;
        assert lineNo == 0;
        lineNo++;
    }

    private static Object prepareCreate(String s, int n, Object putRes) {
        assert "cb-test".equals(s);
        assert n == 1;
        assert putRes == testPutRes;
        return testPrepareCreateRes;
    }

    private static final Object testPutRes = new Object();

    private static class Settings {
        public static Builder builder() { return new Builder(); }
    }

    private static final int SETTING_NUMBER_OF_REPLICAS = 531753;

    private static class Builder {
        public Builder() {}

        private static Object put(int c, Object o) {
            assert c == SETTING_NUMBER_OF_REPLICAS;
            assert o == testBetweenRes;
            return testPutRes;
        }
    }

    private static final Object testBetweenRes = new Object();
    private static Object between(int a, int b) {
        assert a == 0;
        assert b == 1;
        return testBetweenRes;
    }

    private static class Client {
        public SourceSetter prepareIndex(String a, String b, String id) {
            assert this == testClient;
            assert "cb-test".equals(a);
            assert "type".equals(b);
            Integer.parseInt(id);
            return new SourceSetter();
        }

        public Client prepareSearch(String a) {
            assert this == testClient;
            assert lineNo == 4;
            lineNo++;
            assert "cb-test".equals(a);
            return this;
        }
        public Client setQuery(Object query) {
            assert this == testClient;
            assert query == testMatchAllQuery;
            assert lineNo == 5;
            lineNo++;
            return this;
        }

        public Client addSort(String a, int order) {
            assert this == testClient;
            assert "test".equals(a);
            assert order == SortOrder.DESC;
            assert lineNo == 6;
            lineNo++;
            return this;
        }

        public void get() {
            assert lineNo == 7;
            lineNo++;
        }
    }

    private static final Object testMatchAllQuery = new Object();
    private static Object matchAllQuery() { return testMatchAllQuery; }


    private static class SortOrder {
        private static final int DESC = 73753;
    }

    private static Client testClient = new Client();
    private static Client client() {
        assert lineNo == 1;
        lineNo++;
        return testClient;
    }

    private int scaledRandomIntBetween(int a, int b) {
        assert a == 300;
        assert b == 1000;
        assert lineNo == 2;
        lineNo++;
        return TEST_DOC_COUNT;
    }

    private static class IndexRequestBuilder {
        long id;

        public IndexRequestBuilder(long id) {
            this.id = id;
        }
    }

    private static void indexRandom(boolean b, List<IndexRequestBuilder> reqs) {
        assert lineNo == 3;
        lineNo++;
        assert b == true;
        assert reqs.size() == TEST_DOC_COUNT;
        for (int i = 0; i < reqs.size(); i++) {
            assert reqs.get(i).id == i;
        }
    }

    private static class SourceSetter {
        public IndexRequestBuilder setSource(String s, long id) {
            assert "test".equals(s);
            return new IndexRequestBuilder(id);
        }
    }

#START OF SAMPLE    
    public void testNoopFielddataBreaker() throws Exception {
        assertAcked(prepareCreate("cb-test", 1, Settings.builder().put(SETTING_NUMBER_OF_REPLICAS, between(0, 1))));
        Client client = client();

        // index some different terms so we have some field data for loading
        int docCount = scaledRandomIntBetween(300, 1000);
        List<IndexRequestBuilder> reqs = new ArrayList<>();
        for (long id = 0; id < docCount; id++) {
            reqs.add(client.prepareIndex("cb-test", "type", Long.toString(id)).setSource("test", id));
        }
        indexRandom(true, reqs);

        // Sorting using fielddata and thus the FIELDDATA breaker
        client.prepareSearch("cb-test").setQuery(matchAllQuery()).addSort("test", SortOrder.DESC).get();
        // no exception because the breaker is a noop
    }
#END OF SAMPLE    

    public static void main(String[] args) throws Exception {
        new Sample6().testNoopFielddataBreaker();
        assert lineNo == 8;
    }
}