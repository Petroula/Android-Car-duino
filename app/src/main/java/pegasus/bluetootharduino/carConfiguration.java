package pegasus.bluetootharduino;

public enum carConfiguration {
    INSTANCE;

    static int maxAngle = 25;


    static int maxSpeed = 200;
    static int minSpeed = -300;

    static int infraRedError = 0;
    static int infraRedMin = 4;
    static int infraRedMax = 25;

    static int ultrasonicError = 0;
    static int ultrasonicRedMax = 70;

    static int minGyro = 0;
    static int maxGyro = 360;

    static int minRazor = -180;
    static int maxRazor = 180;
}

