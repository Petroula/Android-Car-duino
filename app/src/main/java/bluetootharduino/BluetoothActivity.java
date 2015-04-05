/** 
 * The bluetooth-arduino connection part uses code from the following site
 * https://bellcode.wordpress.com/2012/01/02/android-and-arduino-bluetooth-communication/
 */

package bluetootharduino;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.GestureDetector.*;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.bluetooth.*;
import android.content.Intent;

import com.example.bluetootharduino.R;

public class BluetoothActivity extends Activity implements OnClickListener, OnGestureListener {

	BluetoothAdapter adapter;
    BluetoothDevice MiDevice;
    BluetoothSocket socket;
    InputStream in;
    OutputStream out;
    EditText input;
    TextView data;
	Button send;
	Button connect;
	Button disconnect;
	
	Thread BlueToothThread;
    boolean stop = false;
    int position;
    byte read[];
    Netstrings nt = new Netstrings();

    GestureDetector detector;
	
    @SuppressWarnings("deprecation")
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.bluetooth_activity);
        
        input = (EditText)findViewById(R.id.input);
        
        send = (Button)findViewById(R.id.send);
        send.setOnClickListener(this);
        
        connect = (Button)findViewById(R.id.connect);
        connect.setOnClickListener(this);
        
        disconnect = (Button)findViewById(R.id.disconnect);
        disconnect.setOnClickListener(this);
        
        data = (TextView)findViewById(R.id.data);
        
        detector = new GestureDetector(this);
        
        Toast.makeText(getApplicationContext(), "Swipe to Camera Mode", Toast.LENGTH_SHORT).show();
        
       
    }

    public void runBT() throws IOException {
    	
    	//pairs bluetooth
    	adapter = BluetoothAdapter.getDefaultAdapter();
        
        if(!adapter.isEnabled()) {
            Intent enable = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enable, 0);
        }
        
        Set<BluetoothDevice> pairedDevices = adapter.getBondedDevices();
        if(pairedDevices.size() > 0) {
            for(BluetoothDevice device : pairedDevices) {
                if(device.getName().equals("HC-06")) {
                    MiDevice = device;
                    data.setText("device paired");
                    break;
                }
            }
        }
        
        //opens connection
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //Standard SerialPortService ID
        
        socket = MiDevice.createRfcommSocketToServiceRecord(uuid);
     
        socket.connect();
        out = socket.getOutputStream();
        in = socket.getInputStream();
        
        data.setText("connection established");
       
        
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
                                            data.setText(nt.decodedNetstring(result));
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
    
	@Override
	public void onClick(View v) {
		
		
		if(v.getId() == R.id.connect) {
			try {
				runBT();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
		} else if(v.getId() == R.id.send) {
			
			try {
			String text = nt.encodedNetstring(input.getText().toString());
			out.write(text.getBytes());
			data.setText("message sent");			
			} catch (IOException e) {
				e.printStackTrace();
			}			
			
		} else if(v.getId() == R.id.disconnect) {
			
	        try {
	        	stop = true;
				out.close();
				in.close();
		        socket.close();
		        data.setText("Connection Closed");
			} catch (IOException e) {
				e.printStackTrace();
			}	        
		}	
	} 
	
	
	
/** Uses swipe to change to the camera activity*/
	
	@Override
	public boolean onDown(MotionEvent e) {
		return false;
	}

	@Override
	public void onShowPress(MotionEvent e) {}

	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		return false;
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		return false;
	}

	@Override
	public void onLongPress(MotionEvent e) {}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,	float velocityY) {
		
		if(e1.getX()>e2.getX()) {		
			/** Changes to Camera screen */  	
       	    Intent changeToCamera= new Intent(getApplicationContext(), CameraActivity.class);
			startActivity(changeToCamera);
			return true;
		}
		
		return false;
	}
	
	public boolean onTouchEvent(MotionEvent ev) {
		return detector.onTouchEvent(ev);	
	} 
}
