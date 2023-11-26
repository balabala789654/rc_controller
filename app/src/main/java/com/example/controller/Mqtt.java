package com.example.controller;

import android.util.Log;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class Mqtt {
    MqttClient mqttClient;
    MqttConnectOptions mqttConnectOptions;
    MemoryPersistence memoryPersistence;

    public Mqtt(){};
    public void Mqtt_init(String URL, String clientid){
        memoryPersistence = new MemoryPersistence();
        try{
            mqttClient = new MqttClient(URL, clientid, memoryPersistence);
        } catch (MqttException e){
            e.printStackTrace();
        }

        mqttClient.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                Log.d("lex", "connectComplete");
            }

            @Override
            public void connectionLost(Throwable cause) {

                Log.d("lex", "connectionLost");
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                Log.d("lex", "messageArrived: ");
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });
        Log.d("lex", "init");

    }
    public void Mqtt_connect(String username, String password, String topic) {
        mqttConnectOptions = new MqttConnectOptions();
        mqttConnectOptions.setUserName(username);
        mqttConnectOptions.setPassword(password.toCharArray());
        mqttConnectOptions.setKeepAliveInterval(30);
        mqttConnectOptions.setConnectionTimeout(10);

        try {
            mqttClient.connect(mqttConnectOptions);
        } catch (MqttException e) {
            e.printStackTrace();
        }

        if (mqttClient.isConnected() == false) {
            Log.d("lex", "connect failed");
        } else {
            Mqtt_subscribe(topic);
        }
    }
    public void Mqtt_public(String topic, String message){
        MqttMessage mqttMessage;
        mqttMessage = new MqttMessage(message.getBytes());
        //mqttMessage.setQos(0);
        try{
            mqttClient.publish(topic, mqttMessage);
            Log.d("lex", "public_msg: "+ mqttMessage.toString());
        } catch (MqttException e){
            Log.d("lex", "public error");
            e.printStackTrace();
        }

    }
    public void Mqtt_subscribe(String topic){
        try{
            mqttClient.subscribe(topic);
            Log.d("lex", "subscribe finish");
        } catch (MqttException e){
            e.printStackTrace();
            Log.d("lex", "subscribe error");
        }

    }


}
