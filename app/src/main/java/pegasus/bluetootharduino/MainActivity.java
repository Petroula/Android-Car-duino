package pegasus.bluetootharduino;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener, CompoundButton.OnCheckedChangeListener, TextWatcher {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        /* BUTTONS */
        findViewById(R.id.auto).setOnClickListener(this);
        findViewById(R.id.parking).setOnClickListener(this);
        findViewById(R.id.manual).setOnClickListener(this);
        findViewById(R.id.advanced).setOnClickListener(this);
        findViewById(R.id.bluetooth).setOnClickListener(this);

        //DISPLAY DEBUG INFORMATION SWITCH
        ((Switch)findViewById(R.id.DisplayDebugSwitch)).setOnCheckedChangeListener(this);
        //USE LIGHT NORMALIZATION SWITCH
        ((Switch)findViewById(R.id.LightNormalizationSwitch)).setOnCheckedChangeListener(this);
        //USE LEFT LINE SWITCH
        ((Switch)findViewById(R.id.LeftLineSwitch)).setOnCheckedChangeListener(this);

        /* TEXT INPUTS */
        TextView carLength = (TextView) findViewById(R.id.carLength);
        carLength.setText("40");
        carLength.addTextChangedListener(this);
    }

    /* BUTTONS */
    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.manual:
                Intent changeToManual = new Intent(getApplicationContext(),
                ManualActivity.class);
                startActivity(changeToManual);
                break;
            case R.id.parking:
                Autodrive.setParkingMode();
            case R.id.auto:
                Intent changeToCamera = new Intent(getApplicationContext(),
                CameraActivity.class);
                startActivity(changeToCamera);
                break;
            case R.id.advanced:
                Intent changeToSettings= new Intent(getApplicationContext(),
                AdvSettingsActivity.class);
                startActivity(changeToSettings);
                break;
            case R.id.bluetooth:
                Intent changeToPair= new Intent(getApplicationContext(),
                PairDeviceActivity.class);
                startActivity(changeToPair);
                break;
        }
    }

    /* SWITCHES */
    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        switch (buttonView.getId()) {
            case R.id.LightNormalizationSwitch:
                Autodrive.setSettingLightNormalization(isChecked);
                break;
            case R.id.DisplayDebugSwitch:
                Settings.DisplayDebugInformation = isChecked;
                break;
            case R.id.LeftLineSwitch:
                Autodrive.setSettingUseLeftLine(isChecked);
                break;
        }
    }

    @Override
    public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

    }

    @Override
    public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
        String string = charSequence.toString();
        if (string.equals("")) string = "0";
        Integer integer = Integer.parseInt(string);
        Autodrive.setCarLength(integer);
    }

    @Override
    public void afterTextChanged(Editable editable) {

    }
}
