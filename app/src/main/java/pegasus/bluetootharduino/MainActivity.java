package pegasus.bluetootharduino;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener, CompoundButton.OnCheckedChangeListener, SeekBar.OnSeekBarChangeListener {


    SeekBar kp, ki, kd;

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
        //USE LEFT LINE SWITCH
        ((Switch)findViewById(R.id.LeftLineSwitch)).setOnCheckedChangeListener(this);

        //PID SEEK BARS
        kp = (SeekBar)findViewById(R.id.kp);
        kp.setMax(50);
        kp.setOnSeekBarChangeListener(this);

        ki = (SeekBar)findViewById(R.id.ki);
        ki.setMax(50);
        ki.setOnSeekBarChangeListener(this);

        kd = (SeekBar)findViewById(R.id.kd);
        kd.setMax(50);
        kd.setOnSeekBarChangeListener(this);


        //PID DESCRIPTIONS
        ((TextView)findViewById(R.id.progress1)).setText("kp default value set to 0.5");
        ((TextView)findViewById(R.id.progress2)).setText("ki default value set to 0.0");
        ((TextView)findViewById(R.id.progress3)).setText("kd default value set to 0.0");

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
            case R.id.LeftLineSwitch:
                Autodrive.setSettingUseLeftLine(isChecked);
                break;
        }
    }

    /* SEEK BAR */
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        Autodrive.setSettingSmoothening(progress);
        switch (seekBar.getId()) {
            case R.id.kp:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress1)).setText("kp value set to " + progressValue);
                    Autodrive.setPIDkp(progressValue);
                }
                break;
            case R.id.ki:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress2)).setText("ki value set to " + progressValue);
                    Autodrive.setPIDki(progressValue);
                }
                break;
            case R.id.kd:
                if(fromUser) {
                    float progressValue = (float) (progress / 10.0);
                    ((TextView)findViewById(R.id.progress3)).setText("kd value set to " + progressValue);
                    Autodrive.setPIDkd(progressValue);
                }
                break;
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {}
}
