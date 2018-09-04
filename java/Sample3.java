import java.util.*;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample3 {
    private static java.util.Random testRandom = new Random(0);

    private static class Bucket {
        private long docCount;
        private InternalAggregations aggs;

        public InternalAggregations getAggregations() {
            return aggs;
        }


        public Bucket(long docCount, InternalAggregations aggs) {
            this.aggs = aggs;
            this.docCount = docCount;
        }

        public boolean equals(Object object) {
            if (this == object) return true;
            if (object == null || getClass() != object.getClass()) return false;

            Bucket bucket = (Bucket) object;

            if (docCount != bucket.docCount) return false;
            if (aggs != null ? !aggs.equals(bucket.aggs) : bucket.aggs != null) return false;

            return true;
        }
    }

    private static class ReduceContext {}
    private static final ReduceContext TEST_REDUCE_CONTEXT = new ReduceContext();

    private static class InternalAggregations {
        private int num;

        public InternalAggregations(int num) {
            this.num = num;
        }

        public static InternalAggregations reduce(List<InternalAggregations> aggregations, ReduceContext context) {
            assert context == TEST_REDUCE_CONTEXT;
            assert aggregations.size() == curInput.size();
            for (int i = 0; i < curInput.size(); i++) {
                assert curInput.get(i).aggs.equals(aggregations.get(i));
            }
            int sum = 0;
            for (InternalAggregations agg : aggregations) sum += agg.num;
            return new InternalAggregations(sum);
        }

        public boolean equals(Object object) {
            if (this == object) return true;
            if (object == null || getClass() != object.getClass()) return false;

            InternalAggregations that = (InternalAggregations) object;

            if (num != that.num) return false;

            return true;
        }

        public int hashCode() {
            int result = super.hashCode();
            result = 31 * result + num;
            return result;
        }
    }

    private static class InternalDateHistogram {
        private static class Bucket extends Sample3.Bucket {
            public Bucket(int key, long docCount, int keyed, int format, InternalAggregations aggs) {
                super(docCount, aggs);
                assert key == Sample3.key;
                assert keyed == Sample3.keyed;
                assert format == Sample3.format;
            }
        }
    }

    private static final int key = 9130410;
    private static final int keyed = 204239153;
    private static final int format = 6403209;



    Bucket reduce_orig(List<Bucket> buckets, ReduceContext context) {
        List<InternalAggregations> aggregations = new ArrayList<>(buckets.size());
        long docCount = 0;
        for (Bucket bucket : buckets) {
            docCount += bucket.docCount;
            aggregations.add((InternalAggregations) bucket.getAggregations());
        }
        InternalAggregations aggs = InternalAggregations.reduce(aggregations, context);
        return new InternalDateHistogram.Bucket(key, docCount, keyed, format, aggs);
    }

#START OF SAMPLE    
    Bucket reduce(List<Bucket> buckets, ReduceContext context) {
        List<InternalAggregations> aggregations = new ArrayList<>(buckets.size());
        long docCount = 0;
        for (Bucket bucket : buckets) {
            docCount += bucket.docCount;
            aggregations.add((InternalAggregations) bucket.getAggregations());
        }
        InternalAggregations aggs = InternalAggregations.reduce(aggregations, context);
        return new InternalDateHistogram.Bucket(key, docCount, keyed, format, aggs);
    }
#END OF SAMPLE    

    private static List<Bucket> curInput = null;

    public static void main(String[] args) {
        Sample3 s = new Sample3();

        for (int i = 0; i < 100; i++) {
            curInput = makeTestInput();
            Bucket result  = s.reduce(curInput, TEST_REDUCE_CONTEXT);
            Bucket comp = s.reduce_orig(curInput, TEST_REDUCE_CONTEXT);
            assert(result.equals(comp));
        }
    }

    private static List<Bucket> makeTestInput() {
        int len = testRandom.nextInt(10);
        List<Bucket> result = new ArrayList<Bucket>();

        for (int i = 0; i < len; i++) {
            result.add(new Bucket(testRandom.nextInt(100), new InternalAggregations(testRandom.nextInt(100))));
        }

        return result;
    }
}