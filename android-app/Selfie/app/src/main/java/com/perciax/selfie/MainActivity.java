package com.perciax.selfie;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button btnBtSettings;
    private Button btnBtConnect;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

         /* Button */

        btnBtConnect = (Button) findViewById(R.id.btn_bt_conn);

        btnBtConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(getApplicationContext(), BtConnectActivity.class);
                startActivity(i);
            }
        });

         /* Button */

        btnBtSettings = (Button) findViewById(R.id.btn_bt_setings);

        btnBtSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(getApplicationContext(), BtSettingsActivity.class);
                startActivity(i);
            }
        });
    }

    @Override
    public void onDestroy() {

        super.onDestroy();
    }

}
