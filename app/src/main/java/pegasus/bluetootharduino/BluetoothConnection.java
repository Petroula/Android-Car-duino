/**
 * The bluetooth-arduino connection part uses code from the following site
 * https://bellcode.wordpress.com/2012/01/02/android-and-arduino-bluetooth-communication/
 */

package pegasus.bluetootharduino;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;
import android.bluetooth.*;
import android.os.Handler;
import android.util.Log;

public class BluetoothConnection {

    static BluetoothSocket socket;
    BluetoothAdapter adapt;
    InputStream in;
    static OutputStream out;
    String returnResult;
    String carduino = "98:D3:31:70:22:71";

    Thread BlueToothThread;
    boolean stop = false;
    int position;
    byte read[];
    static Netstrings nt = new Netstrings();


    public void runBT() throws IOException, NullPointerException {

        //opens connection
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //Standard SerialPortService ID

        if(BluetoothPairing.MiDevice == null) {
            adapt = BluetoothAdapter.getDefaultAdapter();
            socket = adapt.getRemoteDevice(carduino).createRfcommSocketToServiceRecord(uuid);
        } else {
            socket = BluetoothPairing.MiDevice.createRfcommSocketToServiceRecord(uuid);
        }

        socket.connect();
        out = socket.getOutputStream();
        in = socket.getInputStream();
//      data.setText("connection established");

        //gets data
        final Handler handler = new Handler();
        final byte delimiter = 10;

        stop = false;
        position = 0;
        read = new byte[1024];
        BlueToothThread = new Thread(new Runnable() {

            public void run() {

                while(!Thread.currentThread().isInterrupted() && !stop) {

                    try {

                        int bytesAvailable = in.available();
                        if(bytesAvailable > 0) {
                            byte[] packetBytes = new byte[bytesAvailable];
                            in.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++) {
                                byte b = packetBytes[i];
                                if(b == delimiter) {
                                    byte[] encodedBytes = new byte[position];
                                    System.arraycopy(read, 0, encodedBytes, 0, encodedBytes.length);
                                    final String result = new String(encodedBytes, "US-ASCII");
                                    position = 0;

                                    handler.post(new Runnable() {
                                        public void run() {
                                            returnResult = nt.decodedNetstring(result);
                                            Log.i("result", returnResult);
                                            SensorData.handleInput(returnResult);
                                        }
                                    });

                                } else {
                                    read[position++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex) {
                        stop = true;
                    }
                }
            }
        });

        BlueToothThread.start();

    }


    public static void send() {
        try {
                String text = "";
                if(Autodrive.speedChanged())
                    text +=nt.encodedNetstring("m" + String.valueOf(Autodrive.getConvertedSpeed()));
                if(Autodrive.angleChanged())
                    text += nt.encodedNetstring("t" + String.valueOf(Autodrive.getConvertedAngle()));

                if(!text.isEmpty()) {
                    if (socket.isConnected()) {
                        out.write(text.getBytes());
                    }
                }
            } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendToManualMode(String command) {
        try {
            String text ="";
            if(command.equals("front")) {
                text = nt.encodedNetstring("m80");
                text += nt.encodedNetstring("t0");
            } else if(command.equals("back")) {
                text = nt.encodedNetstring("m-250");
                text += nt.encodedNetstring("t0");
            } else if(command.equals("right")) {
                text = nt.encodedNetstring("t20");
            } else if(command.equals("left")) {
                text = nt.encodedNetstring("t-20");
            } else if(command.equals("stop")) {
                text = nt.encodedNetstring("m0");
                text += nt.encodedNetstring("t0");
            }

            if(!text.isEmpty()) {
                if (socket.isConnected()) {
                    out.write(text.getBytes());
                }
            }
        } catch (IOException e) {
            e.printStackTrace();

        }

    }

    public void disconnect() {

        try {
            stop = true;
            out.close();
            in.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }



}
