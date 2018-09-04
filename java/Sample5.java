import java.util.Arrays;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample5 {
#START OF SAMPLE    
    public int[] getFrameTimeStampsFromDurations(int[] frameDurationsMs) {
        int[] frameTimestampsMs = new int[frameDurationsMs.length];
        int accumulatedDurationMs = 0;
        for (int i = 0; i < frameDurationsMs.length; i++) {
            frameTimestampsMs[i] = accumulatedDurationMs;
            accumulatedDurationMs += frameDurationsMs[i];
        }
        return frameTimestampsMs;
    }
#END OF SAMPLE
    
    public static void main(String[] args) {
        Sample5 s = new Sample5();

        int[] testInput1 = {};
        int[] testOutput1 = {};

        assert Arrays.equals(testOutput1, s.getFrameTimeStampsFromDurations(testInput1));

        int[] testInput2 = {1000, 2000, 3000};
        int[] testOutput2 = {0, 1000, 3000};
        assert Arrays.equals(testOutput2, s.getFrameTimeStampsFromDurations(testInput2));
    }
}