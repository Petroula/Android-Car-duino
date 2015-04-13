package pegasus.bluetootharduino;

public class SensorData {
    private static final double PULSES_PER_CM = 0.5; //to be determined experimentally
    private static int[] ultrasound = {0,0,0};
    private static int[] infrared = {0,0};
    private static long encoderPulses = 0;

    public enum UltrasoundSensor{
        FRONT(0),
        FRONT_RIGHT(1),
        REAR(2);

        private final int value;
        private UltrasoundSensor(int value) {
            this.value = value;
        }

        public int toInt() {
            return value;
        }
    };

    public enum InfraredSensor {
        FRONT(0),
        REAR(1);

        private final int value;
        private InfraredSensor(int value) {
            this.value = value;
        }

        public int toInt() {
            return value;
        }
    }
    static void setUltrasound(UltrasoundSensor sensor, int value){ ultrasound[sensor.toInt()] = value; };

    static int getUltrasoundSensor(UltrasoundSensor sensor){ return ultrasound[sensor.toInt()]; }

    static void setInfrared(InfraredSensor sensor, int value){
        infrared[sensor.toInt()] = value;
    }

    static int getInfraredSensor(InfraredSensor sensor){
        return infrared[sensor.toInt()];
    }

    static void setEncoderPulses(int value){
        encoderPulses = value;
    }

    static double getEncoderDistance(){
        return encoderPulses/PULSES_PER_CM;
    }


}
