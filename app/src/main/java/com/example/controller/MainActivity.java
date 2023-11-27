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
    private final int Button_NO_press = 0;
    private final int Button_up_press = 1;
    private final int Button_down_press = 2;
    private final int Button_left_press = 3;
    private final int Button_right_press = 4;
    Button button_1;
    Button button_2;
    Button  button_up;
    Button  button_down;
    Button  button_left;
    Button  button_right;
    BlueTooth blueTooth;
    btpublicThread BTpublicThread = new btpublicThread();
    private boolean Handler_running = false;
    private int button_press_check = 0;
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

        final Handler handler = new Handler();
        final Runnable mLongPressed = new Runnable() {
            @Override
            public void run() {
                if (Handler_running){
                    switch (button_press_check){
                        case Button_NO_press:{
                            blueTooth.massage = "000";
                            handler.removeCallbacks(this);
                            Handler_running = false;
                            break;
                        }
                        case Button_up_press:{
                            blueTooth.massage = "111";
                            break;
                        }
                        case Button_down_press: {
                            blueTooth.massage = "222";
                            break;
                        }
                        case Button_left_press: {
                            blueTooth.massage = "333";
                            break;
                        }
                        case Button_right_press: {
                            blueTooth.massage = "444";
                            break;
                        }
                    }
//                    Log.d("lex", "handler");
                    handler.postDelayed(this, 100);
                }
            }
        };
        button_up.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                    button_press_check = Button_up_press;
                    Handler_running = true;
                handler.postDelayed(mLongPressed, 100);
                if (motionEvent.getAction() == MotionEvent.ACTION_UP || motionEvent.getAction() == MotionEvent.ACTION_CANCEL)
                {
                    button_press_check = Button_NO_press;
                }
                return false;
            }
        });
        button_down.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                    button_press_check = Button_down_press;
                    Handler_running = true;
                handler.postDelayed(mLongPressed, 100);
                if (motionEvent.getAction() == MotionEvent.ACTION_UP || motionEvent.getAction() == MotionEvent.ACTION_CANCEL)
                {
                    button_press_check = Button_NO_press;
                }
                return false;
            }
        });
        button_left.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                    button_press_check = Button_left_press;
                    Handler_running = true;
                handler.postDelayed(mLongPressed, 100);
                if (motionEvent.getAction() == MotionEvent.ACTION_UP || motionEvent.getAction() == MotionEvent.ACTION_CANCEL)
                {
                    button_press_check = Button_NO_press;
                }
                return false;
            }
        });
        button_right.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN)
                    button_press_check = Button_right_press;
                    Handler_running = true;
                handler.postDelayed(mLongPressed, 100);
                if (motionEvent.getAction() == MotionEvent.ACTION_UP || motionEvent.getAction() == MotionEvent.ACTION_CANCEL)
                {
                    button_press_check = Button_NO_press;
                }
                return false;
            }
        });
    }
}