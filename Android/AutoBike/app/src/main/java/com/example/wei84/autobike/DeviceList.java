package com.example.wei84.autobike;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class DeviceList extends AppCompatActivity {
    //Widgets
    Button btnRefresh;
    ListView listDevice;
    //Bliuetooth
    private BluetoothAdapter bluetoothAdapter = null;
    private Set<BluetoothDevice> pairedDevices;
    public static String EXTRA_ADDRESS = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_list);
        //Calling widgets
        btnRefresh = (Button)findViewById(R.id.button);
        listDevice = (ListView) findViewById(R.id.listView);
        //if the device has bluetooth
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (bluetoothAdapter == null) {
            //Show a mensag. that the device has no bluetooth adapter
            Toast.makeText(getApplicationContext(), "藍芽未開啟", Toast.LENGTH_LONG).show();
            //finish apk
            finish();
        }
        else{
            if (bluetoothAdapter.isEnabled()) {
            }
            else {
                //Ask to the user turn the bluetooth on
                Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTon, 1);
            }
        }
        refreshDevicesList();

        btnRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                refreshDevicesList();
            }
        });
    }

    private AdapterView.OnItemClickListener listDeviceClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick (AdapterView<?> av, View v, int arg2, long arg3) {
            // Get the device MAC address, the last 17 chars in the View
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

            //Make an intent to start next activity.
            Intent i = new Intent(DeviceList.this, Dashboard.class);

            //Change the activity.
            i.putExtra(EXTRA_ADDRESS, address);
            startActivity(i);
        }
    };

    private void refreshDevicesList() {
        pairedDevices = bluetoothAdapter.getBondedDevices();
        ArrayList list=new ArrayList();

        if(pairedDevices.size() > 0) {
            for(BluetoothDevice bt : pairedDevices){
                //Get the device's name and the address
                list.add(bt.getName() + "\n" + bt.getAddress());
            }
        }
        else{
            Toast.makeText(getApplicationContext(), "找不到配對的藍芽裝置", Toast.LENGTH_SHORT).show();
        }
        final ArrayAdapter adapter = new ArrayAdapter(this,android.R.layout.simple_list_item_1,list);
        listDevice.setAdapter(adapter);
        //Method called when the device from the list is clicked
        listDevice.setOnItemClickListener(listDeviceClickListener);
    }
}