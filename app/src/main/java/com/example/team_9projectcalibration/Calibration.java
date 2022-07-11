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


public class Calibration extends AppCompatActivity implements SensorEventListener {

    private static final String TAG = "Threshold";
    private SensorManager sensorManager3;
    private Sensor sensor3;
    private long last_update;
    private long last_update1;
    private double sum;
    private double sum1;
    double rotation_degree;
    double rotation_degree1;
    public double calibration[] = new double[]{0,0};

    public OkHttpClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gyroscope);
        client = new OkHttpClient();
        sensorManager3 = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensor3 = sensorManager3.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        sensorManager3.registerListener(Calibration.this, sensor3, SensorManager.SENSOR_DELAY_UI);

        last_update = System.currentTimeMillis();
        rotation_degree = 0;
        rotation_degree1 = 0;
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

        final TextView output2 = (TextView) findViewById(R.id.textView6);
        final TextView output1 = (TextView) findViewById(R.id.textView7);


        long actual_time;
        long time_elapsed;

        long actual_time1;
        long time_elapsed1;


        if (sum > 500 && sensorEvent.values[2] > -0.01 && sensorEvent.values[2] < 0.01) {
            calibration[0] = sum;
            Log.d(TAG, "onSensorChanged: calibration: " + calibration[0]);
        }

        if (sum1 > 500 && sensorEvent.values[1] > -0.01 && sensorEvent.values[1] < 0.01) {
            calibration[1] = sum1;
            Log.d(TAG, "onSensorChanged: calibration: " + calibration[1]);
        }

        Threshold.threshold[1] = calibration[0];
        Log.d(TAG, String.valueOf(Threshold.threshold[1]));



        Threshold.threshold[0] = calibration[1];
        Log.d("Thresholdx", String.valueOf(Threshold.threshold[0]));

        output1.setText("Sideways Threshold: " + "  " + String.valueOf(Threshold.threshold[1]));
        output2.setText("Forward Threshold: " + "   " + String.valueOf(Threshold.threshold[0]));


        if (sum > 0 && sensorEvent.values[2] < -1.5) sum -= rotation_degree;
        if (sum < 0 && sensorEvent.values[2] > 1.5) sum += rotation_degree;

        if (sum1 > 0 && sensorEvent.values[1] < -1.5) sum1 -= rotation_degree1;
        if (sum1 < 0 && sensorEvent.values[1] > 1.5) sum1 += rotation_degree1;



        actual_time = System.currentTimeMillis();
        actual_time1 = System.currentTimeMillis();

        if (sensorEvent.values[2] > 0.1 || sensorEvent.values[2] < -0.1) {

            time_elapsed = actual_time - last_update;
            rotation_degree = sensorEvent.values[2] * time_elapsed;
            sum += rotation_degree;
            last_update = actual_time;
            return;
        }

        if (sensorEvent.values[1] > 0.1 || sensorEvent.values[1] < -0.1) {

            time_elapsed1 = actual_time1 - last_update;
            rotation_degree1 = sensorEvent.values[1] * time_elapsed1;
            sum1 += rotation_degree1;
            last_update = actual_time1;
            return;
        }

        last_update = System.currentTimeMillis();
        last_update1 = System.currentTimeMillis();
    }


}