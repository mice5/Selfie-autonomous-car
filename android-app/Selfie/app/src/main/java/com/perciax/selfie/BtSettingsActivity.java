package com.perciax.selfie;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

/**
 * Created by mateu on 10/25/2017.
 */

public class BtSettingsActivity extends AppCompatActivity {
    //buttons declaration
    private Button btnBtOn, btnBtOff, btnBtVisible, btnBtShowPaired, btnBtDiscover;
    private TextView txtBtSupport;
    //Bluetooth Adapter declaration
    private BluetoothAdapter BA;
    private Set<BluetoothDevice> pairedDevices;
    //Progress Dialog declaration
    private ProgressDialog mProgressDlg;
    //Array of bluetooth devices
    private ArrayList<BluetoothDevice> mDeviceList = new ArrayList<BluetoothDevice>();
    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            switch (action) {
                case BluetoothAdapter.ACTION_STATE_CHANGED:
                    final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);

                    if (state == BluetoothAdapter.STATE_ON) {
                        Toast.makeText(getApplicationContext(),
                                "Enabled", Toast.LENGTH_LONG).show();
                        showEnabled();
                    }

                case BluetoothAdapter.ACTION_DISCOVERY_STARTED:
                    mDeviceList = new ArrayList<>();
                    mProgressDlg.show();
                    break;
                case BluetoothAdapter.ACTION_DISCOVERY_FINISHED:
                    mProgressDlg.dismiss();
                    Intent i = new Intent(getApplicationContext(), BtDeviceListActivity.class);
                    i.putParcelableArrayListExtra("device.list", mDeviceList);
                    startActivity(i);
                    break;
                case BluetoothDevice.ACTION_FOUND:
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    mDeviceList.add(device);
                    showToast("Found device " + device.getName());
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bt_settings);

        //buttons initialization
        btnBtOn = (Button) findViewById(R.id.btnBtOn);
        btnBtOff = (Button) findViewById(R.id.btnBtOff);
        btnBtVisible = (Button) findViewById(R.id.btnBtVisible);
        btnBtDiscover = (Button) findViewById(R.id.btnBtDiscover);
        btnBtShowPaired = (Button) findViewById(R.id.btnBtShowPaired);
        txtBtSupport = (TextView) findViewById(R.id.txtBtSupport);
        //get Bluetooth Adapter
        BA = BluetoothAdapter.getDefaultAdapter();

        //filter for receiving information for bluetooth
        IntentFilter filter = new IntentFilter();

        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(mReceiver, filter);

        //define Progress Dialog
        mProgressDlg = new ProgressDialog(this);
        mProgressDlg.setMessage("Scanning...");
        mProgressDlg.setCancelable(false);
        mProgressDlg.setButton(DialogInterface.BUTTON_NEGATIVE, "Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                BA.cancelDiscovery();
            }
        });

        //check if device support bluetooth
        if (BA == null) {
            showUnsupported();
        }

        //if bluetooth is disabled hide buttons apart one to turn bluetooth on
        if (!BA.isEnabled()) {
            showDisabled();
        } else {
            showEnabled();
        }

    }

    public void btOn(View v) {
        if (!BA.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
            showToast("Turned on");
        } else {
            showToast("Already on");
        }
        showEnabled();
    }

    public void btOff(View v) {
        BA.disable();
        Toast.makeText(getApplicationContext(), "Turned off", Toast.LENGTH_LONG).show();
        showDisabled();
    }

    public void btVisible(View v) {
        Intent getVisible = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        startActivityForResult(getVisible, 0);
    }

    public void btShowPaired(View view) {
        Set<BluetoothDevice> pairedDevices = BA.getBondedDevices();

        if (pairedDevices == null || pairedDevices.size() == 0) {
            showToast("No Paired Devices Found");
        } else {
            ArrayList<BluetoothDevice> list = new ArrayList<BluetoothDevice>();

            list.addAll(pairedDevices);

            Intent intent = new Intent(getApplicationContext(), BtDeviceListActivity.class);

            intent.putParcelableArrayListExtra("device.list", list);

            startActivity(intent);
        }

    }

    public void btDiscover(View view) {

        BA.startDiscovery();

    }

    @Override
    public void onPause() {
        if (BA != null) {
            if (BA.isDiscovering()) {
                BA.cancelDiscovery();
            }
        }
        super.onPause();
    }

    //register BroadcoastReceiver

    @Override
    protected void onDestroy() {
        unregisterReceiver(mReceiver);
        super.onDestroy();
    }

    //private methods

    private void showEnabled() {
        btnBtOn.setEnabled(false);
        btnBtOff.setEnabled(true);
        btnBtVisible.setEnabled(true);
        btnBtShowPaired.setEnabled(true);
        btnBtDiscover.setEnabled(true);
    }

    private void showDisabled() {
        btnBtOn.setEnabled(true);
        btnBtOff.setEnabled(false);
        btnBtVisible.setEnabled(false);
        btnBtShowPaired.setEnabled(false);
        btnBtDiscover.setEnabled(false);
    }

    private void showUnsupported() {
        txtBtSupport.setText("Bluetooth is unsupported by this device");
        btnBtOn.setEnabled(false);
        btnBtOff.setEnabled(false);
        btnBtVisible.setEnabled(false);
        btnBtShowPaired.setEnabled(false);
        btnBtDiscover.setEnabled(false);
    }

    private void showToast(String message) {
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }


}