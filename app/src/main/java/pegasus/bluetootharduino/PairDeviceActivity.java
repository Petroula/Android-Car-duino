package pegasus.bluetootharduino;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.app.ProgressDialog;


public class PairDeviceActivity extends Activity implements AdapterView.OnItemClickListener, GestureDetector.OnGestureListener {

    BluetoothPairing blue = new BluetoothPairing();
    ListView pairedList;
    static String pair;
    ProgressDialog progress;
    GestureDetector detector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.paired_devices);

        detector = new GestureDetector(this);

        /* Show a progress dialog before presenting the results */
        progress = new ProgressDialog(PairDeviceActivity.this);
        progress.setMessage("Fetching paired devices...");
        progress.show();

        pairedList = (ListView)findViewById(R.id.pairedList);

        blue.BTsearch();
        if(!blue.btEnabled) {
            final AlertDialog alert = new AlertDialog.Builder(this).create();
            alert.setTitle("Ooops...");
            alert.setMessage("No bluetooth available...");
            alert.setButton("Ok", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    alert.dismiss();
                }
            });
            alert.setCanceledOnTouchOutside(false);
            alert.show();
        }
        ArrayAdapter<String> listAdapter = new ArrayAdapter<>(PairDeviceActivity.this, R.layout.custom_listview_item, android.R.id.text1, blue.listDevices);

        if(blue.listDevices.size() > 0) {
            progress.dismiss();
            pairedList.setAdapter(listAdapter);
        } else {
            final AlertDialog alert = new AlertDialog.Builder(this).create();
            alert.setTitle("Paired Devices:");
            alert.setMessage("No paired devices found...");
            alert.setButton("Ok", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    alert.dismiss();
                }
            });
            alert.setCanceledOnTouchOutside(false);
            alert.show();
        }
        pairedList.setOnItemClickListener(this);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        final AlertDialog alert = new AlertDialog.Builder(this).create();
        alert.setTitle("You chose to pair with...");
        pair = parent.getItemAtPosition(position).toString();
        pair = pair.split("\n")[pair.split("\n").length - 1];
        blue.BTpair();
        alert.setMessage(parent.getItemAtPosition(position).toString().replaceAll("\n.*", ""));
        alert.setButton("Ok", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                alert.dismiss();
            }
        });
        alert.setCanceledOnTouchOutside(false);
        alert.show();
    }

    /** Uses swipe to change to the main activity*/

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
        if(e1.getX()<e2.getX()) {
            /** Changes to Main screen */
            Intent changeToMain = new Intent(getApplicationContext(), MainActivity.class);
            startActivity(changeToMain);
            return true;
        }
        return false;
    }

    public boolean onTouchEvent(MotionEvent ev) {
        return detector.onTouchEvent(ev);
    }

    /** Changes the behaviour of the back button */
    public void onBackPressed() {
        Intent changeToMain= new Intent(getApplicationContext(), MainActivity.class);
        startActivity(changeToMain);
    }
}