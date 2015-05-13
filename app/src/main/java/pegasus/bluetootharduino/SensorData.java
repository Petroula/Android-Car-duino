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

    static boolean foundObstacle() {
        return Autodrive.hasFoundObstacle();
    }

    static int irFrontAutodrive() {
        return Autodrive.irFrontAutodrive();
    }

    static int usFrontAutodrive() {
        return Autodrive.usFrontAutodrive();
    }

    static void setUltrasound(int sensor, int value){
        Autodrive.setUltrasound(0, value);

        if (sensor == 0) {
            ultrasonicFront = value;
        } else if (sensor == 1) {
            ultrasonicFrontRight = value;
        } else if (sensor == 2) {
            ultrasonicRear = value;
        }
        CameraActivity.updateDebuggingConsole();
    }

    static void setInfrared(int sensor, int value){
        Autodrive.setInfrared(sensor,5);

        if (sensor == 0) {
            infraredSideFront = value;
        } else if (sensor == 1) {
            infraredSideRear = value;
        } else if (sensor == 2) {
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
            setUltrasound(sensorNum - 1, Integer.parseInt(input.substring(4)));
        } else if (input.startsWith("IR")){
            int sensorNum = Integer.parseInt(input.substring(2,3));
            setInfrared(sensorNum - 1,Integer.parseInt(input.substring(4)));
        }
    }
}
