package com.perciax.selfie;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class BtConnectActivity extends Activity {
    private BluetoothAdapter mBluetoothAdapter;
    private ListView mListView;
    private BtConnectListAdapter mAdapter;
    private final BroadcastReceiver mPairReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(action)) {
                final int state = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, BluetoothDevice.ERROR);
                final int prevState = intent.getIntExtra(BluetoothDevice.EXTRA_PREVIOUS_BOND_STATE, BluetoothDevice.ERROR);

                if (state == BluetoothDevice.BOND_BONDED && prevState == BluetoothDevice.BOND_BONDING) {
                    showToast("Paired");
                } else if (state == BluetoothDevice.BOND_NONE && prevState == BluetoothDevice.BOND_BONDED) {
                    showToast("Unpaired");
                }

                mAdapter.notifyDataSetChanged();
            }
        }
    };
    //Progress Dialog declaration
    private ProgressDialog mProgressDlg;
    //device list
    private ArrayList<BluetoothDevice> mDeviceList;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_bt_connect);

        //get Bluetooth Adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();

        mDeviceList = new ArrayList<BluetoothDevice>();

        if (pairedDevices == null || pairedDevices.size() == 0) {
            showToast("No Paired Devices Found");
        } else {

            mDeviceList.addAll(pairedDevices);
            showToast(mDeviceList.toString());

        }

        mListView = (ListView) findViewById(R.id.lv_paired);

        mAdapter = new BtConnectListAdapter(this);

        mAdapter.setData(mDeviceList);
        mAdapter.setListener(new BtConnectListAdapter.OnButtonClickListener() {
            @Override
            public void onButtonClick(int position) {
                BluetoothDevice device = mDeviceList.get(position);
                String macAddress = device.getAddress();
                showToast("MAC: " + macAddress);
                Intent i = new Intent(getApplicationContext(), BtCommActivity.class);
                i.putExtra("address", macAddress);
                startActivity(i);
            }
        });

        mListView.setAdapter(mAdapter);

        //registerReceiver(mPairReceiver, new IntentFilter(BluetoothDevice.ACTION_BOND_STATE_CHANGED));

    }

    @Override
    public void onDestroy() {
        //unregisterReceiver(mPairReceiver);

        super.onDestroy();
    }

    private void showToast(String message) {
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }

}