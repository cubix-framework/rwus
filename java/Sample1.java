class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample1 {

    private @interface Test {}

    private static int lineNo = 0;

    private static void subscribe() {
        assert lineNo == 0;
        lineNo++;
    }

    private static class Value {}
    private static Value testValue = new Value();
    private static Throwable testThrowable = new Throwable();

    private static <E> E mock(Class<E> klass) {
        if (lineNo == 1) {
            assert klass.equals(Value.class);
            lineNo++;
            return klass.cast(testValue);
        } else {
            assert lineNo == 5;
            lineNo++;
            return klass.cast(testThrowable);
        }
    }

    private static class DataSource {
        public void setResult(Value v, int c) {
            assert lineNo == 2;
            assert v == testValue;
            assert c == INTERMEDIATE;
            lineNo++;
        }

        public void setFailure(Throwable t) {
            assert lineNo == 6;
            assert t == testThrowable;
            lineNo++;
        }

        public void close() {
            assert lineNo == 9;
            lineNo++;
        }
    }

    private static DataSource mDataSource = new DataSource();

    private static void verifySubscribers(int c) {
        assert (  lineNo == 3 && c == ON_NEW_RESULT
               || lineNo == 7 && c == ON_FAILURE
                  || lineNo == 10 && c == NO_INTERACTIONS);
        lineNo++;
    }

    private static void verifyState(DataSource dataSource, int c1, int c2, int c3, Value v, int c4, Throwable t) {
        if (lineNo == 4) {
            assert dataSource == mDataSource;
            assert c1 == NOT_CLOSED;
            assert c2 == NOT_FINISHED;
            assert c3 == WITH_RESULT;
            assert v == testValue;
            assert c4 == NOT_FAILED;
            assert t == null;
        } else if (lineNo == 8) {
            assert dataSource == mDataSource;
            assert c1 == NOT_CLOSED;
            assert c2 == FINISHED;
            assert c3 == WITH_RESULT;
            assert v == testValue;
            assert c4 == FAILED;
            assert t == testThrowable;
        } else {
            assert lineNo == 11;
            assert dataSource == mDataSource;
            assert c1 == CLOSED;
            assert c2 == FINISHED;
            assert c3 == WITHOUT_RESULT;
            assert v == null;
            assert c4 == FAILED;
            assert t == testThrowable;
        }

        lineNo++;
    }


    private static final int INTERMEDIATE = 101, NOT_CLOSED = 102, NOT_FINISHED = 103, WITH_RESULT = 104, NOT_FAILED = 105, ON_FAILURE = 106, FAILED = 107, CLOSED = 108, ON_NEW_RESULT = 109, FINISHED = 110, WITHOUT_RESULT = 111, NO_INTERACTIONS = 112;



#START OF SAMPLE
    public void testLifeCycle_IntermediateResult_Failure_Close() {
        subscribe();
        // intermediate result
        Value value1 = mock(Value.class);
        mDataSource.setResult(value1, INTERMEDIATE);
        verifySubscribers(ON_NEW_RESULT);
        verifyState(mDataSource, NOT_CLOSED, NOT_FINISHED, WITH_RESULT, value1, NOT_FAILED, null);
        // failure
        Throwable throwable = mock(Throwable.class);
        mDataSource.setFailure(throwable);
        verifySubscribers(ON_FAILURE);
        verifyState(mDataSource, NOT_CLOSED, FINISHED, WITH_RESULT, value1, FAILED, throwable);
        // close
        mDataSource.close();
        verifySubscribers(NO_INTERACTIONS);
        verifyState(mDataSource, CLOSED, FINISHED, WITHOUT_RESULT, null, FAILED, throwable);
    }
#END OF SAMPLE

    public static void main(String[] args) {
        new Sample1().testLifeCycle_IntermediateResult_Failure_Close();
        assert(lineNo == 12);
    }

}
