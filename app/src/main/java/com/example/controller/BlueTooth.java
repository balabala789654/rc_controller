package com.example.controller;

import static androidx.core.app.ActivityCompat.startActivityForResult;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.util.Log;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
public class BlueTooth extends AppCompatActivity{
    private static final int MY_PERMISSION_ACCESS_COARSE_LOCATION = 11;
    private static final int MY_PERMISSION_ACCESS_FINE_LOCATION = 12;
    private static final int MY_PERMISSION_BlueTooth = 13;
    private static final int MY_PERMISSION_BlueTooth_ADMIN = 14;
    BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    BluetoothSocket bluetoothSocket;
    OutputStream outputStream;
    btpublicThread BTpublicThread = new btpublicThread();
    public String massage = "000";

    public boolean BlueTooth_init() {
        if (bluetoothAdapter == null) {
            Log.i("lex", "设备不支持蓝牙");
            return false;
        } else {
            Log.i("lex", "设备支持蓝牙");
            if (!bluetoothAdapter.isEnabled()) {
                Log.i("lex", "请启用蓝牙");
            }
            return true;
        }
    }
    public void bt_connect(Activity activity){
        if (ContextCompat.checkSelfPermission(activity , Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED){
            Log.d("lex", "ACCESS_COARSE_LOCATION 无权限");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, MY_PERMISSION_ACCESS_COARSE_LOCATION);
        }
        if (ContextCompat.checkSelfPermission(activity, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_DENIED){
            Log.d("lex", "ACCESS_FINE_LOCATION 无权限");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, MY_PERMISSION_ACCESS_FINE_LOCATION);
        }
        if (ContextCompat.checkSelfPermission(activity, Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED){
            Log.d("lex", "BLUETOOTH 无权限");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH}, MY_PERMISSION_BlueTooth);
        }
        if (ContextCompat.checkSelfPermission(activity, Manifest.permission.BLUETOOTH_ADMIN) != PackageManager.PERMISSION_GRANTED){
            Log.d("lex", "BLUETOOTH_ADMIN 无权限");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH_ADMIN}, MY_PERMISSION_BlueTooth_ADMIN);
        }
        if (ContextCompat.checkSelfPermission(activity, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED){
            Log.d("lex", "BLUETOOTH_CONNECT 无权限");
            ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, MY_PERMISSION_BlueTooth_ADMIN);
        }
        if ( ContextCompat.checkSelfPermission(activity, android.Manifest.permission.BLUETOOTH) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(activity, android.Manifest.permission.BLUETOOTH_ADMIN) == PackageManager.PERMISSION_GRANTED){
            Log.d("lex", "work");
            Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();
            for (BluetoothDevice bluetoothDevice : devices){
                @SuppressLint("MissingPermission") String deviceName = bluetoothDevice.getName();
                @SuppressLint("MissingPermission") String deviceAddress = bluetoothDevice.getAddress();
                Log.i("lex", "Device Name: " + deviceName + ", Address: " + deviceAddress);
                if ("Lex".equals(deviceName)){
                    try {
                        bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
                        bluetoothSocket.connect();
                        Log.d("lex", "connect");
                        BTpublicThread.start();
                    }
                    catch (IOException ioException){
                        ioException.printStackTrace();
                    }
                    break;
                }
            }
        }
    }
    public void BlueTooth_public(String msg){
        if (bluetoothSocket != null){
            try {
                outputStream = bluetoothSocket.getOutputStream();
                byte[] bytes = msg.getBytes();
                outputStream.write(bytes);
                Log.d("lex", "public: " + msg);
            }
            catch (IOException ioException){
                ioException.printStackTrace();
            }
        }
    }
    @SuppressLint("MissingSuperCall")
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        Log.d("lex", "权限请求");
        switch (requestCode) {
            case MY_PERMISSION_ACCESS_COARSE_LOCATION: {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    // 权限已授予，继续操作
                    Log.e("lex", "1 权限已授予");
                }
                else {
                    // 权限被拒绝，根据需要进行处理
                    Log.e("lex", "1 权限被拒绝");
                }
                break;
            }
            case MY_PERMISSION_ACCESS_FINE_LOCATION: {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    // 权限已授予，继续操作
                    Log.e("lex", "2 权限已授予");
                }
                else {
                    // 权限被拒绝，根据需要进行处理
                    Log.e("lex", "2 权限被拒绝");
                }
                break;
            }
            case MY_PERMISSION_BlueTooth: {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    // 权限已授予，继续操作
                    Log.e("lex", "2 权限已授予");
                }
                else {
                    // 权限被拒绝，根据需要进行处理
                    Log.e("lex", "2 权限被拒绝");
                }
                break;
            }
            case MY_PERMISSION_BlueTooth_ADMIN: {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    Log.e("lex", "2 权限已授予");
                }
                else {
                    // 权限被拒绝，根据需要进行处理
                    Log.e("lex", "2 权限被拒绝");

                }
                break;
            }
        }
    }
    public class btpublicThread extends Thread {
        private boolean running = true;
        public void stopThread(){
            running = false;
        }
        public void run(){
            Log.d("lex", "Thread");
            while(running) {
                BlueTooth_public(massage);
                try {
                    Thread.sleep(100);
                } catch (InterruptedException interruptedException) {
                    interruptedException.printStackTrace();
                }
            }
        }
    }

}

