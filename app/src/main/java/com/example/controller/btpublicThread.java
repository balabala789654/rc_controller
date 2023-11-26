package com.example.controller;

import android.app.Activity;
import android.util.Log;

import java.lang.ref.WeakReference;

public class btpublicThread extends Thread {
    BlueTooth bt = new BlueTooth();
    private boolean running = true;
    public void stopThread(){
        running = false;
    }
    public void run(){
        Log.d("lex", "Thread");
        while(running) {
            bt.BlueTooth_public("aaa\r\n");
            try {
                Thread.sleep(500);
            } catch (InterruptedException interruptedException) {
                interruptedException.printStackTrace();
            }
        }
    }
}
