package pegasus.bluetootharduino;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.CompoundButton;
import android.widget.Switch;

public class MainActivity extends Activity implements OnClickListener, CompoundButton.OnCheckedChangeListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        /* BUTTONS */
        ((Button)findViewById(R.id.auto)).setOnClickListener(this);
        ((Button)findViewById(R.id.parking)).setOnClickListener(this);
        ((Button)findViewById(R.id.manual)).setOnClickListener(this);

        //DISPLAY DEBUG INFORMATION SWITCH
        ((Switch)findViewById(R.id.DisplayDebugSwitch)).setOnCheckedChangeListener(this);
        //USE LIGHT NORMALIZATION SWITCH
        ((Switch)findViewById(R.id.LightNormalizationSwitch)).setOnCheckedChangeListener(this);
    }

    /* BUTTONS */
    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.manual:
                break;
            case R.id.parking:
            case R.id.auto:
                Intent changeToCamera= new Intent(getApplicationContext(),
                CameraActivity.class);
                startActivity(changeToCamera);
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
        }
    }
}
