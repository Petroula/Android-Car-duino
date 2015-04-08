package pegasus.bluetootharduino;

import java.io.IOException;

public class AutomaticCarDriver extends Thread /* ALLWAYS RAN IN ITS OWN THREAD*/ {

    private boolean running = true;

    public void close(){
        running = false;
    }

    @Override
    public void run() {
        try {
            while (running) {
                int a = Natives.Tick();
            }
        } catch (Exception e) {
            running = false;
        }
    }
}
