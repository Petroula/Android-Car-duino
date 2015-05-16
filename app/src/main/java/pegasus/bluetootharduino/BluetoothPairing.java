package pegasus.bluetootharduino;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import java.util.ArrayList;
import java.util.Set;

public class BluetoothPairing {

    BluetoothAdapter adapter;
    static BluetoothDevice MiDevice;
    boolean btEnabled = true;
    String listNames;
    String listAddress;
    String listNamesAddress;
    ArrayList<String> listDevices = new ArrayList<>();

    //find paired devices
    public void BTsearch() throws NullPointerException{

        adapter = BluetoothAdapter.getDefaultAdapter();

        if (adapter == null || !adapter.isEnabled()) {
            btEnabled = false;
        }

        Set<BluetoothDevice> pairedDevices = adapter.getBondedDevices();
        if(pairedDevices.size() > 0) {
            for(BluetoothDevice device : pairedDevices) {
                listNames = device.getName();
                listAddress = "\n" + device.getAddress();
                listNamesAddress = listNames + listAddress;
                listDevices.add(listNamesAddress);
            }
        }
    }

    // pair user's choice
    public void BTpair() {
        MiDevice = adapter.getRemoteDevice(PairDeviceActivity.pair);
    }
}