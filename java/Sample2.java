import java.util.*;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample2 {

    private static class InternalAggregation {}
    
    private static class InternalGeoCentroid extends InternalAggregation {
        private String name;
        private GeoPoint centroid;
        private int count;
        private int pipelineAggregators;
        private int metaData;

        public InternalGeoCentroid(String name, GeoPoint centroid, int count, int pipelineAggregators, int metaData) {
            this.name = name;
            this.centroid = centroid;
            this.count = count;
            this.pipelineAggregators = pipelineAggregators;
            this.metaData = metaData;
        }

        public boolean equals(Object object) {
            if (this == object) return true;
            if (object == null || getClass() != object.getClass()) return false;

            InternalGeoCentroid that = (InternalGeoCentroid) object;

            if (count != that.count) return false;
            if (centroid != null ? !centroid.equals(that.centroid) : that.centroid != null) return false;

            return true;
        }
    }
    
    private static class GeoPoint {
        double lon,lat;
        public GeoPoint(double a, double b) {
            this.lon = a;
            this.lat = b;
        }

        public double getLon() { return lon; }
        public double getLat() { return lat; }

        public boolean equals(Object object) {
            if (this == object) return true;
            if (object == null || getClass() != object.getClass()) return false;

            GeoPoint geoPoint = (GeoPoint) object;

            if (Double.compare(geoPoint.lon, lon) != 0) return false;
            if (Double.compare(geoPoint.lat, lat) != 0) return false;

            return true;
        }
    }

    private static class ReduceContext {}

    private static final int TEST_METADATA = 420240;
    private static int getMetaData() { return TEST_METADATA; }
    private static final int TEST_PIPELINE_AGGREGATORS = 647535;
    private static int pipelineAggregators() { return TEST_PIPELINE_AGGREGATORS; }

    private static final String name = "EUOIOODQXA>PYI";


    public InternalGeoCentroid doReduce_original(List<InternalAggregation> aggregations, ReduceContext reduceContext) {
        double lonSum = Double.NaN;
        double latSum = Double.NaN;
        int totalCount = 0;
        for (InternalAggregation aggregation : aggregations) {
            InternalGeoCentroid centroidAgg = (InternalGeoCentroid) aggregation;
            if (centroidAgg.count > 0) {
                totalCount += centroidAgg.count;
                if (Double.isNaN(lonSum)) {
                    lonSum = centroidAgg.count * centroidAgg.centroid.getLon();
                    latSum = centroidAgg.count * centroidAgg.centroid.getLat();
                } else {
                    lonSum += (centroidAgg.count * centroidAgg.centroid.getLon());
                    latSum += (centroidAgg.count * centroidAgg.centroid.getLat());
                }
            }
        }
        final GeoPoint result = (Double.isNaN(lonSum)) ? null : new GeoPoint(latSum/totalCount, lonSum/totalCount);
        return new InternalGeoCentroid(name, result, totalCount, pipelineAggregators(), getMetaData());
    }

#START OF SAMPLE
    public InternalGeoCentroid doReduce(List<InternalAggregation> aggregations, ReduceContext reduceContext) {
        double lonSum = Double.NaN;
        double latSum = Double.NaN;
        int totalCount = 0;
        for (InternalAggregation aggregation : aggregations) {
            InternalGeoCentroid centroidAgg = (InternalGeoCentroid) aggregation;
            if (centroidAgg.count > 0) {
                totalCount += centroidAgg.count;
                if (Double.isNaN(lonSum)) {
                    lonSum = centroidAgg.count * centroidAgg.centroid.getLon();
                    latSum = centroidAgg.count * centroidAgg.centroid.getLat();
                } else {
                    lonSum += (centroidAgg.count * centroidAgg.centroid.getLon());
                    latSum += (centroidAgg.count * centroidAgg.centroid.getLat());
                }
            }
        }
        final GeoPoint result = (Double.isNaN(lonSum)) ? null : new GeoPoint(latSum/totalCount, lonSum/totalCount);
        return new InternalGeoCentroid(name, result, totalCount, pipelineAggregators(), getMetaData());
    }
#END OF SAMPLE

    public static void main(String[] args) {
        Random rand = new Random(0);
        Sample2 s = new Sample2();

        for (int i = 0 ; i < 100; i++) {
            List<InternalAggregation> testList = makeRandomTestList(rand);
            InternalGeoCentroid res  = s.doReduce(testList, null);
            assert res.name == name;
            assert res.pipelineAggregators == TEST_PIPELINE_AGGREGATORS;
            assert res.metaData == TEST_METADATA;
            InternalGeoCentroid test_res = s.doReduce_original(testList, null);
            if (res.centroid == null) {
                assert(test_res.centroid == null);
            } else {
                assert(res.centroid.equals(test_res.centroid));
            }
            assert(res.count == test_res.count);
        }
    }

    private static List<InternalAggregation> makeRandomTestList(Random rand) {
        int len = rand.nextInt(10);
        List<InternalAggregation> result = new ArrayList<InternalAggregation>();
        for (int i = 0; i < len; i++) {
            InternalGeoCentroid x = new InternalGeoCentroid(null, new GeoPoint(rand.nextDouble(), rand.nextDouble()), rand.nextInt(10) + 1, 0, 0);
            result.add(x);
        }
        return result;
    }

}
