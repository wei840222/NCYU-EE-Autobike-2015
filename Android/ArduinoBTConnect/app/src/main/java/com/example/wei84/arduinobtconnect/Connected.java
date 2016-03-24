package com.example.wei84.arduinobtconnect;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

public class Connected extends AppCompatActivity {

    //Widgets
    EditText textMsg;
    TextView textSlope;
    Button btnDissconnect;
    String address = null;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;

    //SPP UUID
    static  final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connected);
        //receive the address of the bluetooth device
        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS);
        //call the widgtes
        btnDissconnect = (Button) findViewById(R.id.button);
        textMsg = (EditText) findViewById(R.id.editText);
        textSlope = (TextView) findViewById(R.id.textView);

        //Call the class to connect
        new ConnectBT().execute();

        handler.postDelayed(runnable, 200);

        btnDissconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (btSocket != null) {
                    try {
                        btSocket.close();
                    } catch (IOException e) {
                        msg("錯誤");
                    }
                }
                //return to the first layout
                handler.removeCallbacks(runnable);
                finish();
            }
        });
    }

    private Handler handler = new Handler( );
    private Runnable runnable = new Runnable( ) {
        public void run() {
            byte[] BTbuffer = new byte[32];
            try {
                btSocket.getOutputStream().write(textMsg.getText().toString().getBytes());
                if(btSocket.getInputStream().available() > 0) {
                    btSocket.getInputStream().read(BTbuffer);
                    String BTstring = new String(BTbuffer, "US-ASCII");
                    textSlope.setText("坡度: " + BTstring);
                }
            } catch (IOException e) {
                msg("錯誤");
            }
            handler.postDelayed(this, 200);
        }
    };

    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        //if it's here, it's almost connect
        private boolean ConnectSuccess = true;
        private boolean isBtConnected = false;
        private ProgressDialog progress;
        protected void onPreExecute(){
            //show a progress dialog
            progress = ProgressDialog.show(Connected.this, "藍芽連線中...", "請稍後...");
        }
        protected Void doInBackground(Void... devices){
            //while the progress dialog is shown, the connect is done background
            try{
                if(btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            }
            catch (IOException e){
                ConnectSuccess = false;
            }
            return null;
        }

        protected void onPostExecute(Void result){
            super.onPostExecute(result);
            if(!ConnectSuccess){
                msg("連線錯誤 請重試");
                finish();
            }
            else{
                msg("已連線");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }
}