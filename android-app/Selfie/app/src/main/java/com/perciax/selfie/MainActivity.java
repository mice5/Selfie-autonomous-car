package com.perciax.selfie;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button btnBtSettings;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

         /* Button */

        btnBtSettings = (Button) findViewById(R.id.btn_bt_setings);
        //button listener starts PwmButtonControlActivity
        btnBtSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(getApplicationContext(), BtSettingsActivity.class);
                //i.putExtra(OmniPlatformControlActivity.ROBOT_ID, 0);
                startActivity(i);
            }
        });
    }

    @Override
    public void onDestroy() {

        super.onDestroy();
    }
}
