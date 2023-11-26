package com.example.controller;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    Button button_1;
    Button button_2;
    Button  button_up;
    Button  button_down;
    Button  button_left;
    Button  button_right;
    BlueTooth blueTooth;
    btpublicThread BTpublicThread = new btpublicThread();
    public String massage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        blueTooth = new BlueTooth();
        button_1 = (Button) findViewById(R.id.button_connect);
        button_2 = (Button) findViewById(R.id.button_public);
        button_up = (Button) findViewById(R.id.button_up);
        button_down = (Button) findViewById(R.id.button_down);
        button_left = (Button) findViewById(R.id.button_left);
        button_right = (Button) findViewById(R.id.button_right);

        blueTooth.BlueTooth_init();
        button_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.i("lex", "button_1 on click");
                blueTooth.bt_connect(MainActivity.this);
            }
        });
        button_2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.i("lex", "button_2 on click");
            }
        });
        button_up.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                Log.d("lex", "long click");
                return false;
            }
        });

        final Handler handler = new Handler();
        final Runnable mLongPressed = new Runnable() {
            @Override
            public void run() {
            }
        };
        button_up.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                    handler.postDelayed(mLongPressed, 1000);
                return false;
            }
        });
    }
}