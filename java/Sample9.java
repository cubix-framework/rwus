class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample9 {

    private static int lineNo = 0;

    private static class View {
        public ViewHolder getTag() {
            return tag;
        }

        public void setTag(ViewHolder tag) {
            this.tag = tag;
        }

        public View findViewById(int x) {
            assert x == R.id.chart;
            return testLineChart;
        }

        private ViewHolder tag;
    }


    private final static LineChart testLineChart = new LineChart();


    private static class LineChart extends View {
        public Description getDescription() {
            return description;
        }

        private Description description = new Description();

        public boolean isDrawGridBackground() {
            return drawGridBackground;
        }

        public void setDrawGridBackground(boolean drawGridBackground) {
            assert lineNo == 1;
            lineNo++;
            this.drawGridBackground = drawGridBackground;
        }

        private boolean drawGridBackground = true;

        public XAxis getXAxis() {
            return xAxis;
        }

        private XAxis xAxis = new XAxis();

        public YAxis getAxisRight() {
            return axisRight;
        }

        private YAxis axisRight = new YAxis();

        private int animateX;
        private void animateX(int x) {
            assert lineNo == 11;
            lineNo++;
            animateX = x;
        }

        private LineData lineData;
        private void setData(LineData lineData) {
            assert lineNo == 10;
            lineNo++;
            this.lineData = lineData;
        }
    }

    private static class XAxis {
        private int position;

        public int getTypeface() {
            return typeface;
        }

        public void setTypeface(int typeface) {
            assert lineNo == 3;
            lineNo++;
            this.typeface = typeface;
        }

        public int getPosition() {
            return position;
        }

        public void setPosition(int position) {
            assert lineNo == 2;
            lineNo++;
            this.position = position;
        }

        private int typeface;
    }

    private static class YAxis {
        private int typeface;
        private boolean drawGridLines = true;
        private float axisMinimum = 1f;

        public void setTypeface(int typeface) {
            assert lineNo == 6;
            lineNo++;
            this.typeface = typeface;
        }

        public void setDrawGridLines(boolean drawGridLines) {
            assert lineNo == 8;
            lineNo++;
            this.drawGridLines = drawGridLines;
        }

        public void setAxisMinimum(float axisMinimum) {
            assert lineNo == 9;
            lineNo++;
            this.axisMinimum = axisMinimum;
        }

        private void setLabelCount(int a, boolean b) {
            assert lineNo == 7;
            lineNo++;
            assert a == 5;
            assert b == false;
        }
    }

    private static class Description {
        public boolean isEnabled() {
            return enabled;
        }

        public void setEnabled(boolean enabled) {
            assert lineNo == 0;
            lineNo++;
            this.enabled = enabled;
        }

        private boolean enabled = true;
    }

    private final static View testInflatedView = new View();

    private static class ViewHolder {
        private LineChart chart;
    }

    private static class Context {}

    private final static Context testContext = new Context();

    private static class LayoutInflater {
        public static LayoutInflater from(Context c) {
            assert testContext == c;
            return new LayoutInflater();
        }

        public View inflate(int x, Object o) {
            assert x == R.layout.list_item_linechart;
            assert o == null;
            return testInflatedView;
        }
    }

    private static class R {
        private static class layout {
            private static int list_item_linechart = 75375337;
        }

        private static class id {
            private static int chart = 7317537;
        }
    }

    private static class LeftAxis {
        private int a;
        private boolean b = true;
        private float c = 1f;

        public void setLabelCount(int a, boolean b) {
            assert lineNo == 4;
            lineNo++;
            this.a = a;
            this.b = b;
        }

        public void setAxisMinimum(float c) {
            assert lineNo == 5;
            lineNo++;
            this.c = c;
        }

    }

    private final static LeftAxis leftAxis = new LeftAxis();

    private static final class LineData {}
    private static final LineData mChartData = new LineData();


    private final static int mTf = 75113917;
    private static final class XAxisPosition {
        private static final int BOTTOM = 3517371;
    }

#START OF SAMPLE
    public View getView(int position, View convertView, Context c) {

        ViewHolder holder = null;

        if (convertView == null) {

            holder = new ViewHolder();

            convertView = LayoutInflater.from(c).inflate(
                    R.layout.list_item_linechart, null);
            holder.chart = (LineChart) convertView.findViewById(R.id.chart);

            convertView.setTag(holder);

        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        // apply styling
        // holder.chart.setValueTypeface(mTf);
        holder.chart.getDescription().setEnabled(false);
        holder.chart.setDrawGridBackground(false);

        XAxis xAxis = holder.chart.getXAxis();
        xAxis.setPosition(XAxisPosition.BOTTOM);
        xAxis.setTypeface(mTf);

        leftAxis.setLabelCount(5, false);
        leftAxis.setAxisMinimum(0f); // this replaces setStartAtZero(true)

        YAxis rightAxis = holder.chart.getAxisRight();
        rightAxis.setTypeface(mTf);
        rightAxis.setLabelCount(5, false);
        rightAxis.setDrawGridLines(false);
        rightAxis.setAxisMinimum(0f); // this replaces setStartAtZero(true)

        // set data
        holder.chart.setData((LineData) mChartData);

        // do not forget to refresh the chart
        // holder.chart.invalidate();
        holder.chart.animateX(750);

        return convertView;
    }
#END OF SAMPLE
    
    public static void main(String[] args) {
        Sample9 s = new Sample9();

        View testView = new View();
        ViewHolder testHolder = new ViewHolder();
        LineChart chart = new LineChart();
        testView.tag = testHolder;
        testHolder.chart = chart;

        lineNo = 0;
        View res1 = s.getView(-1, testView, testContext);
        assert  lineNo == 12;

        assert res1 == testView;
        assert res1.tag == testHolder;
        assert res1.tag.chart == chart;

        checkLeftAxis();
        checkChart(res1.tag.chart);


        leftAxis.a = 4;
        leftAxis.b = true;
        leftAxis.c = 1f;

        lineNo = 0;

        View res2 = s.getView(-1, null, testContext);
        assert lineNo == 12;

        assert res2 == testInflatedView;
        assert res2.tag.chart == testLineChart;

        checkLeftAxis();
        checkChart(res2.tag.chart);

    }

    private static void checkChart(LineChart chart) {

        assert chart.getDescription().enabled == false;
        assert chart.drawGridBackground == false;

        assert chart.xAxis.position == XAxisPosition.BOTTOM;
        assert chart.xAxis.typeface == mTf;

        assert chart.axisRight.typeface == mTf;
        assert chart.axisRight.drawGridLines == false;
        assert chart.axisRight.axisMinimum == 0f;

        assert chart.lineData == mChartData;
        assert chart.animateX == 750;
    }

    private static void checkLeftAxis() {

        assert leftAxis.a == 5;
        assert leftAxis.b == false;
        assert leftAxis.c == 0f;
    }
}