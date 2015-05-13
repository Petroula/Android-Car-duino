package pegasus.bluetootharduino;

//TODO: We might want to remove this class completely and just use something like Autodrive.SensorData instead
public class SensorData {
    public static int ultrasonicFront = 0,
            ultrasonicFrontRight = 0,
            ultrasonicRear = 0,
            infraredSideFront = 0,
            infraredSideRear = 0,
            infraredRear = 0,
            gyroHeading = 0,
            razorHeading = 0,
            encoderPulses = 0;

    public enum UltrasoundSensor {
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
        SIDE_FRONT(0),
        SIDE_REAR(1),
        REAR(2);

        private final int value;
        private InfraredSensor(int value) {
            this.value = value;
        }

        public int toInt() {
            return value;
        }
    }

    static boolean foundObstacle() {
        return Autodrive.hasFoundObstacle();
    }

    static int irFrontAutodrive() {
        return Autodrive.irFrontAutodrive();
    }

    static void setUltrasound(UltrasoundSensor sensor, int value){
        int sensorNr = sensor.toInt();
        Autodrive.setUltrasound(sensorNr, value);
        Autodrive.setUltrasound(sensor.toInt(), value);

        if (sensorNr == 0) {
            ultrasonicFront = value;
        } else if (sensorNr == 1) {
            ultrasonicFrontRight = value;
        } else if (sensorNr == 2) {
            ultrasonicRear = value;
        }
        CameraActivity.updateDebuggingConsole();
    }

    static void setInfrared(InfraredSensor sensor, int value){
        int sensorNr = sensor.toInt();
        Autodrive.setInfrared(sensorNr,value);
        Autodrive.setInfrared(sensor.toInt(),value);

        if (sensorNr == 0) {
            infraredSideFront = value;
        } else if (sensorNr == 1) {
            infraredSideRear = value;
        } else if (sensorNr == 2) {
            infraredRear = value;
        }
        CameraActivity.updateDebuggingConsole();
    }

    static void setEncoderPulses(int value){
        Autodrive.setEncoderPulses(value);

        encoderPulses = value;
        CameraActivity.updateDebuggingConsole();
    }

    static void setGyroHeading(int value){
        Autodrive.setGyroHeading(value);

        gyroHeading = value;
        CameraActivity.updateDebuggingConsole();
    }

    static void setRazorHeading(int value){
        Autodrive.setRazorHeading(value);

        razorHeading = value;
        CameraActivity.updateDebuggingConsole();
    }

    static void handleInput(String input){
        input = input.replaceAll("\\r|\\n", "");

        if (input.startsWith("EN")){
            setEncoderPulses(Integer.parseInt(input.substring(3)));
        }else if (input.startsWith("HE")){
            setGyroHeading(Integer.parseInt(input.substring(3)));
        }else if (input.startsWith("RZR")){
            setRazorHeading(Integer.parseInt(input.substring(4)));
        }else if (input.startsWith("US")){
            int sensorNum = Integer.parseInt(input.substring(2,3));
            UltrasoundSensor sensor = UltrasoundSensor.REAR;
            switch (sensorNum){
                case 0:
                    sensor = UltrasoundSensor.FRONT;
                    break;
                case 1:
                    sensor = UltrasoundSensor.FRONT_RIGHT;
                    break;
            }
            setUltrasound(sensor, Integer.parseInt(input.substring(4)));
        } else if (input.startsWith("IR")){
            int sensorNum = Integer.parseInt(input.substring(2,3));
            InfraredSensor sensor = InfraredSensor.REAR;
            switch (sensorNum){
                case 0:
                    sensor = InfraredSensor.SIDE_FRONT;
                    break;
                case 1:
                    sensor = InfraredSensor.SIDE_REAR;
                    break;
            }
            setInfrared(sensor,Integer.parseInt(input.substring(4)));
        }
    }
}
