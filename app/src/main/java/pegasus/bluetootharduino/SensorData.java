package pegasus.bluetootharduino;

//TODO: We might want to remove this class completely and just use something like Autodrive.SensorData instead
public class SensorData {
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
    static void setUltrasound(UltrasoundSensor sensor, int value){
        Autodrive.setUltrasound(sensor.toInt(), value);
    };

    static int getUltrasoundSensor(UltrasoundSensor sensor){
        return Autodrive.getUltrasoundSensor(sensor.toInt());
    }

    static void setInfrared(InfraredSensor sensor, int value){
        Autodrive.setInfrared(sensor.toInt(),value);
    }

    static int getInfraredSensor(InfraredSensor sensor){
        return Autodrive.getInfraredSensor(sensor.toInt());
    }

    static void setEncoderPulses(int value){
        Autodrive.setEncoderPulses(value);
    }

    static double getEncoderDistance(){
        return Autodrive.getEncoderDistance();
    }


}
