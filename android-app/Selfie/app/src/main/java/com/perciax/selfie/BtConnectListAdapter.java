package com.perciax.selfie;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

import java.util.List;

/**
 * Created by mateu on 10/25/2017.
 */


public class BtConnectListAdapter extends BaseAdapter {
    private LayoutInflater mInflater;
    private List<BluetoothDevice> mData;
    private OnButtonClickListener mListener;

    public BtConnectListAdapter(Context context) {
        mInflater = LayoutInflater.from(context);
    }

    public void setData(List<BluetoothDevice> data) {
        mData = data;
    }

    public void setListener(OnButtonClickListener listener) {
        mListener = listener;
    }

    public int getCount() {
        return (mData == null) ? 0 : mData.size();
    }

    public Object getItem(int position) {
        return null;
    }

    public long getItemId(int position) {
        return position;
    }

    public View getView(final int position, View convertView, ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.bt_connect_list_item, null);

            holder = new ViewHolder();

            holder.tvName = (TextView) convertView.findViewById(R.id.tv_name);
            holder.tvMac = (TextView) convertView.findViewById(R.id.tv_address);
            holder.btnAction = (Button) convertView.findViewById(R.id.btn_action);

            convertView.setTag(holder);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        BluetoothDevice device = mData.get(position);

        holder.tvName.setText(device.getName());
        holder.tvMac.setText(device.getAddress());
        holder.btnAction.setText((device.getBondState() == BluetoothDevice.BOND_BONDED) ? "Connect" : "Pair");
        holder.btnAction.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mListener != null) {
                    mListener.onButtonClick(position);
                }
            }
        });

        return convertView;
    }

    public interface OnButtonClickListener {
        void onButtonClick(int position);
    }

    static class ViewHolder {
        TextView tvName;
        TextView tvMac;
        TextView btnAction;
    }
}
