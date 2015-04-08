package test;

import junit.framework.TestCase;

public class NativesTest extends TestCase {

    public void testTick() throws Exception {
        pegasus.bluetootharduino.Natives.Tick();
    }
}