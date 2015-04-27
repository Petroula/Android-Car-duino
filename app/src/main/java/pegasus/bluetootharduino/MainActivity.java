package pegasus.bluetootharduino;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;

public class MainActivity extends Activity implements OnClickListener {

    Button auto;
    Button parking;
    Button manual;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        auto = (Button)findViewById(R.id.auto);
        auto.setOnClickListener(this);

        parking = (Button)findViewById(R.id.parking);
        parking.setOnClickListener(this);

        manual = (Button)findViewById(R.id.manual);
        manual.setOnClickListener(this);

    }



    @Override
    public void onClick(View v) {

        if(v.getId() == R.id.auto || v.getId() == R.id.parking) {
            Intent changeToCamera= new Intent(getApplicationContext(), CameraActivity.class);
            startActivity(changeToCamera);

        } else if(v.getId() == R.id.manual) {
            //TO DO
        }


    }



}
