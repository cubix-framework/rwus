class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample7 {
    private static final String testField = "ououiau";

    private static class Query {
        private double l;
        private double u;

        public Query(double l, double u) {
            this.l = l;
            this.u = u;
        }
    }

    private static class DoublePoint {
        private static Query newRangeQuery(String field, double l, double u) {
            assert testField.equals(field);
            return new Query(l, u);
        }
    }

    private static final double testNextUpRes = 37535.573753;
    private static final double testNextDownRes = 35313177.753;

    private static class Math {
        public static double nextUp(double a) {
            assert a == testLowerTermRes;
            return testNextUpRes;
        }

        public static double nextDown(double a) {
            assert a == testUpperTermRes;
            return testNextDownRes;
        }
    }

    private final static Object testLowerTerm = new Object();
    private final static Object testUpperTerm = new Object();

    private final static double testLowerTermRes = 75339.3;
    private final static double testUpperTermRes = 539105.642;

    public Double parse(Object o) {
        if (o == testLowerTerm) return testLowerTermRes;
        if (o == testUpperTerm) return testUpperTermRes;

        return null;
    }

#START OF SAMPLE    
    Query rangeQuery(String field, Object lowerTerm, Object upperTerm,
                     boolean includeLower, boolean includeUpper) {
        double l = Double.NEGATIVE_INFINITY;
        double u = Double.POSITIVE_INFINITY;
        if (lowerTerm != null) {
            l = parse(lowerTerm);
            if (includeLower == false) {
                l = Math.nextUp(l);
            }
        }
        if (upperTerm != null) {
            u = parse(upperTerm);
            if (includeUpper == false) {
                u = Math.nextDown(u);
            }
        }
        return DoublePoint.newRangeQuery(field, l, u);
    }
#END OF SAMPLE    

    public static void main(String[] args) {
        Sample7 s = new Sample7();

        Query x;

        x = s.rangeQuery(testField, null, null, true, true);
        assert x.l == Double.NEGATIVE_INFINITY;
        assert x.u == Double.POSITIVE_INFINITY;

        x = s.rangeQuery(testField, testLowerTerm, null, true, true);
        assert x.l == testLowerTermRes;
        assert x.u == Double.POSITIVE_INFINITY;

        x = s.rangeQuery(testField, testLowerTerm, null, false, true);
        assert x.l == testNextUpRes;
        assert x.u == Double.POSITIVE_INFINITY;

        x = s.rangeQuery(testField, null, testUpperTerm, true, true);
        assert x.l == Double.NEGATIVE_INFINITY;
        assert x.u == testUpperTermRes;

        x = s.rangeQuery(testField, testLowerTerm, testUpperTerm, true, true);
        assert x.l == testLowerTermRes;
        assert x.u == testUpperTermRes;

        x = s.rangeQuery(testField, testLowerTerm, testUpperTerm, false, true);
        assert x.l == testNextUpRes;
        assert x.u == testUpperTermRes;

        x = s.rangeQuery(testField, null, testUpperTerm, true, false);
        assert x.l == Double.NEGATIVE_INFINITY;
        assert x.u == testNextDownRes;

        x = s.rangeQuery(testField, testLowerTerm, testUpperTerm, true, false);
        assert x.l == testLowerTermRes;
        assert x.u == testNextDownRes;

        x = s.rangeQuery(testField, testLowerTerm, testUpperTerm, false, false);
        assert x.l == testNextUpRes;
        assert x.u == testNextDownRes;

    }
}