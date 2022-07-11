package com.example.team_9projectcalibration;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;

import android.widget.TextView;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import okio.ByteString;

public class Gyroscope extends AppCompatActivity implements SensorEventListener {

    private static final String TAG = "Gyroscope";
    private SensorManager sensorManager3;
    private Sensor sensor3;
    private long last_update;
    private long last_update1;
    private double sum;
    private double sum1;
    double rotation_degree;
    double rotation_degree1;
    public int z_motion = 0;
    public int x_motion = 0;
    public int motion;
    Calibration c = new Calibration();
    String mg = "Message";


    public OkHttpClient client;
    public String message;

    //////////////////////////////////////////////////////////////////////////////
    /// Creating WebSocketListener to transfer data
    public final class ClientWebSocketListener extends WebSocketListener {
        private static final int NORMAL_CLOSURE_STATUS = 1000;

        //////////////////////////
        // Sending a message
        @Override
        public void onOpen(WebSocket webSocket, Response response) {
            webSocket.send(String.valueOf(motion));
            webSocket.close(NORMAL_CLOSURE_STATUS, "Goodbye !");
        }

        ///////////////////////////
        ///////////////////////////
        // when message is received
        @Override
        public void onMessage(WebSocket webSocket, String text) {
            Log.d("TAG", "Receiving : " + text);
            Log.d("TAG", "Receiving : " + text);
            String[] seperated = text.split("@");
            Log.d("Object0", seperated[0]);
            Log.d("Object0", seperated[1]);
            if (seperated[0].equals(mg)) {
                message = seperated[1];
                Log.d("Object0", seperated[1]);
            }
        }

        ////////////////////////////
        @Override
        public void onMessage(WebSocket webSocket, ByteString bytes) {
            Log.d("TAG", "Receiving bytes : " + bytes.hex());
        }

        @Override
        public void onClosing(WebSocket webSocket, int code, String reason) {
            webSocket.close(NORMAL_CLOSURE_STATUS, null);
            Log.d("TAG", "Closing : " + code + " / " + reason);
        }

        @Override
        public void onFailure(WebSocket webSocket, Throwable t, Response response) {
            Log.d("TAG", "Error : " + t.getMessage());
        }
    }
    ////////////////////////////////////////////////////////////////////////////////

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gyroscope);
        client = new OkHttpClient();
        sensorManager3 = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensor3 = sensorManager3.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        sensorManager3.registerListener(Gyroscope.this, sensor3, SensorManager.SENSOR_DELAY_UI);

        last_update = System.currentTimeMillis();
        //last_update1 = System.currentTimeMillis();
        rotation_degree = 0;
        rotation_degree1 = 0;
//        int threshold_x = c.threshold[1];
//        int threshold_z = c.threshold[0];
//        int threshold_x_n = -1*c.threshold[1];
//        int threshold_z_n = -1*c.threshold[0];
        Log.d("TAG", "threshold2 : " + Threshold.threshold[1]);
        Log.d("TAG", "threshold1 : " + Threshold.threshold[1] / 2);
        Log.d("TAG", "threshold4 : " + Threshold.threshold[0]);
        Log.d("TAG", "threshold3 : " + Threshold.threshold[0] / 2);


    }

    protected void onPause() {
        super.onPause();
        sensorManager3.unregisterListener(this);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {


        final TextView view = (TextView) findViewById(R.id.textView6);
        final TextView view1 = (TextView) findViewById(R.id.textView7);
        final TextView output = (TextView) findViewById(R.id.textView4);

        long actual_time;
        long time_elapsed;

        long actual_time1;
        long time_elapsed1;

        output.setText("Object" + "    " + message);
        //////////////////////////////////////////////////////////
        // Cummulating rotation values
        if (sum > 0 && sensorEvent.values[2] < -1.5) sum -= rotation_degree;
        if (sum < 0 && sensorEvent.values[2] > 1.5) sum += rotation_degree;

        if (sum1 > 0 && sensorEvent.values[1] < -1.5) sum1 -= rotation_degree1;
        if (sum1 < 0 && sensorEvent.values[1] > 1.5) sum1 += rotation_degree1;
        //////////////////////////////////////////////////////////


        actual_time = System.currentTimeMillis();
        actual_time1 = System.currentTimeMillis();

        //////////////////////////////////////////////////////////
        // Extracting gyroscope values and deriving rotation values
        if (sensorEvent.values[2] > 0.1 || sensorEvent.values[2] < -0.1) {

            time_elapsed = actual_time - last_update;
            rotation_degree = sensorEvent.values[2] * time_elapsed;
            sum += rotation_degree;
            last_update = actual_time;
            Log.d(TAG, "onSensorChanged: sum: " + sum);
            Log.d(TAG, "onSensorChanged: rotation: " + rotation_degree);
            Log.d(TAG, "onSensorChanged: sum: " + sum);
            Log.d(TAG, "onSensorChanged: Motion: " + z_motion);
            return;
        }

        if (sensorEvent.values[1] > 0.1 || sensorEvent.values[1] < -0.1) {

            time_elapsed1 = actual_time1 - last_update;
            rotation_degree1 = sensorEvent.values[1] * time_elapsed1;
            sum1 += rotation_degree1;
            last_update = actual_time1;
            Log.d(TAG, "onSensorChanged: sum: " + sum1);
            Log.d(TAG, "onSensorChanged: rotation: " + rotation_degree1);
            Log.d(TAG, "onSensorChanged: sum: " + sum1);
            Log.d(TAG, "onSensorChanged: Motion: " + x_motion);
            return;
        }
        /////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////
        // Setting threshold values

        if (sum > ((-1 * Threshold.threshold[1]) / 2) && sum < ((Threshold.threshold[1] / 2))) {
            z_motion = 0;
            view.setText("Straight" + "    " + String.valueOf(z_motion));
        }
        if (sum < ((-1 * Threshold.threshold[1]) / 2) && sum > (-1 * Threshold.threshold[1])) {
            z_motion = -1;
            view.setText("Right" + "    " + String.valueOf(z_motion));
        }
        if (sum < (-1 * Threshold.threshold[1]) && sum > -3000) {
            z_motion = -2;
            view.setText("Fast Right" + "    " + String.valueOf(z_motion));
        }

        if (sum > ((Threshold.threshold[1] / 2)) && sum < Threshold.threshold[1]) {
            z_motion = 1;
            view.setText("Left" + "    " + String.valueOf(z_motion));
        }
        if (sum > Threshold.threshold[1] && sum < 3000) {
            z_motion = 2;
            view.setText("Fast Left" + "    " + String.valueOf(z_motion));
        }


        if (sum1 > ((-1 * Threshold.threshold[0]) / 2) && sum1 < (Threshold.threshold[0] / 2)) {
            x_motion = 0;
            view1.setText("Stopped" + "    " + String.valueOf(x_motion));
        }
        if (sum1 < ((-1 * Threshold.threshold[0]) / 2) && sum1 > (-1 * Threshold.threshold[0])) {
            x_motion = -1;
            view1.setText("Reverse" + "    " + String.valueOf(x_motion));
        }
        if (sum1 < (-1 * Threshold.threshold[0]) && sum1 > -3000) {
            x_motion = -2;
            view1.setText("Fast Reverse" + "    " + String.valueOf(x_motion));
        }

        if (sum1 > (Threshold.threshold[0] / 2) && sum1 < Threshold.threshold[0]) {
            x_motion = 1;
            view1.setText("Forward" + "    " + String.valueOf(x_motion));
        }
        if (sum1 > Threshold.threshold[0] && sum1 < 3000) {
            x_motion = 2;
            view1.setText("Fast Forward" + "    " + String.valueOf(x_motion));
        }
        ///////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////
        // Setting motion values depending on the gestures

        if (z_motion == 0 && x_motion == 0) motion = 0;
        if (z_motion == 0 && x_motion == 1) motion = 1;
        if (z_motion == 0 && x_motion == 2) motion = 2;
        if (z_motion == 0 && x_motion == -1) motion = 3;
        if (z_motion == 0 && x_motion == -2) motion = 4;
        if (z_motion == 1 && x_motion == 0) motion = 5;
        if (z_motion == 2 && x_motion == 0) motion = 6;
        if (z_motion == -1 && x_motion == 0) motion = 7;
        if (z_motion == -2 && x_motion == 0) motion = 8;
        if (z_motion == 1 && x_motion == 1) motion = 9;
        if (z_motion == 2 && x_motion == 2) motion = 10;
        if (z_motion == 1 && x_motion == -1) motion = 11;
        if (z_motion == 2 && x_motion == -2) motion = 12;
        if (z_motion == -1 && x_motion == -1) motion = 13;
        if (z_motion == -2 && x_motion == -2) motion = 14;
        if (z_motion == -1 && x_motion == 1) motion = 15;
        if (z_motion == -2 && x_motion == 2) motion = 16;
        if (z_motion == 2 && x_motion == 1) motion = 17;
        if (z_motion == 2 && x_motion == -1) motion = 18;
        if (z_motion == 1 && x_motion == -2) motion = 19;
        if (z_motion == 1 && x_motion == 2) motion = 20;
        if (z_motion == -2 && x_motion == 1) motion = 21;
        if (z_motion == -2 && x_motion == -1) motion = 22;


        last_update = System.currentTimeMillis();
        last_update1 = System.currentTimeMillis();
        start();
        //////////////////////////////////////////////////////
    }

    public void start() {               // Connecting to the web socket server
        Request request = new Request.Builder().url("ws://192.168.4.1:80").build();
        ClientWebSocketListener listener = new ClientWebSocketListener();
        WebSocket ws = client.newWebSocket(request, listener);

    }
}




