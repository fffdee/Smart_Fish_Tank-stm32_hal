package com.example.two_dog_fish_tank;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpCookie;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Calendar;

public class fishtank extends AppCompatActivity implements View.OnTouchListener,View.OnClickListener{
    private Handler handler;
    private final int MOTOR1 = 1;
    private final int MOTOR2 = 2;
    private final int SEND = 3;
    private final int GET1 = 5;
    private final int GET2 = 6;
    private final int HOT = 9;

    private ToggleButton toggleButton,T1;
    String ip;
    private EditText editText;
    private TextView Ttemp,Tptemp,Tzhuod,Tpzhuod,motor1,motor2,hotflag,waterflag;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fishtank);
        findViewById(R.id.GO).setOnTouchListener(this);

        findViewById(R.id.MOTOR1).setOnTouchListener(this);
        findViewById(R.id.MOTOR2).setOnTouchListener(this);
        findViewById(R.id.HOT).setOnClickListener(this);

        toggleButton = findViewById(R.id.HOT);
        Ttemp = findViewById(R.id.Ttemp);
        Tptemp = findViewById(R.id.Tptemp);
        Tzhuod = findViewById(R.id.Tzhuod);
        Tpzhuod = findViewById(R.id.Tpzhuod);
        motor1 = findViewById(R.id.motorFlag1);
        motor2 = findViewById(R.id.motorFlag2);
        hotflag = findViewById(R.id.hotFlag);
        waterflag = findViewById(R.id.waterFlag);

        editText = findViewById(R.id.Emode);

        findViewById(R.id.SetMode).setOnClickListener(this);
        findViewById(R.id.MOTOR1).setOnTouchListener(this);
        findViewById(R.id.MOTOR2).setOnTouchListener(this);

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        ip = bundle.getString("IP","");
        Toast.makeText(fishtank.this,"获取IP:"+ip,Toast.LENGTH_SHORT).show();
        HandlerThread handlerThread = new HandlerThread("Http");
        handlerThread.start();


        handler = new HttpHandler(handlerThread.getLooper());

    }

    @Override
    public void onClick(View v) {
        if(v.getId()==R.id.HOT)
            handler.sendEmptyMessage(HOT);

        else if(v.getId()==R.id.SetMode)
            handler.sendEmptyMessage(SEND);

    }

    private class HttpHandler extends Handler {
        public HttpHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MOTOR1:
                    MOTOR1();
                    break;
                case MOTOR2:
                    MOTOR2();
                    break;

                case GET1:
                    get1();
                    break;
                case GET2:
                    get2();
                    break;

                case SEND:
                    send();
                    break;
                case HOT:
                    HOT();
                    break;

                default:
                    break;
            }
        }




    }

    private void HOT(){
        if(toggleButton.isChecked()){


            String registerUrl = "http://"+ip+"/control?HOT=0";
            try
            {
                URL url = new URL(registerUrl);
                final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
                httpURLConnection.setRequestMethod("GET");
                httpURLConnection.setConnectTimeout(1000 * 5);
                httpURLConnection.setReadTimeout(1000 * 5);
                httpURLConnection.setDoInput(true);
                httpURLConnection.connect();

                final StringBuilder buffer = new StringBuilder();
                int code = httpURLConnection.getResponseCode();
                if (code == 200)
                {
                    httpURLConnection.disconnect();
                    InputStream inputStream = httpURLConnection.getInputStream();
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                    String line = null;
                    while ((line = bufferedReader.readLine()) != null)
                    {
                        buffer.append(line);
                    }
                    runOnUiThread(() -> {

                        // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                    });
                }
                httpURLConnection.disconnect();

            } catch (Exception e)
            {
                e.printStackTrace();
            }

        }else{


            String registerUrl = "http://"+ip+"/control?HOT=1";
            try
            {
                URL url = new URL(registerUrl);
                final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
                httpURLConnection.setRequestMethod("GET");
                httpURLConnection.setConnectTimeout(1000 * 5);
                httpURLConnection.setReadTimeout(1000 * 5);
                httpURLConnection.setDoInput(true);
                httpURLConnection.connect();

                final StringBuilder buffer = new StringBuilder();
                int code = httpURLConnection.getResponseCode();
                if (code == 200)
                {
                    httpURLConnection.disconnect();
                    InputStream inputStream = httpURLConnection.getInputStream();
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                    String line = null;
                    while ((line = bufferedReader.readLine()) != null)
                    {
                        buffer.append(line);
                    }
                    runOnUiThread(() -> {

                        // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                    });
                }
                httpURLConnection.disconnect();

            } catch (Exception e)
            {
                e.printStackTrace();
            }

        }
    }


    @SuppressLint("SetTextI18n")
    private void get1() {


        String registerUrl = "http://"+ip+"/control?HOT=1";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void MOTOR1() {


        String registerUrl = "http://"+ip+"/control?MOTOR1=1";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void get2() {


        String registerUrl = "http://"+ip+"/control?MOTOR2=0";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void MOTOR2() {

        String registerUrl = "http://"+ip+"/control?MOTOR2=1";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    @SuppressLint("SetTextI18n")
    private void back() {


        String registerUrl = "http://"+ip+"/control?back=1";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void send() {


        String registerUrl = "http://"+ip+"/control?back=0";
        try
        {
            URL url = new URL(registerUrl);
            final HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.setConnectTimeout(1000 * 5);
            httpURLConnection.setReadTimeout(1000 * 5);
            httpURLConnection.setDoInput(true);
            httpURLConnection.connect();

            final StringBuilder buffer = new StringBuilder();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)
            {
                httpURLConnection.disconnect();
                InputStream inputStream = httpURLConnection.getInputStream();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String line = null;
                while ((line = bufferedReader.readLine()) != null)
                {
                    buffer.append(line);
                }
                runOnUiThread(() -> {

                    // Toast.makeText(MainActivity.this, buffer.toString(), Toast.LENGTH_SHORT).show();
                });
            }
            httpURLConnection.disconnect();

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }


    @SuppressLint({"NonConstantResourceId", "ClickableViewAccessibility"})
    @Override
    public boolean onTouch(View v, MotionEvent event) {

        switch (v.getId()) {
            case R.id.MOTOR2:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(MOTOR2);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    handler.sendEmptyMessage(GET2);
                }
                break;
            case R.id.MOTOR1:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(MOTOR1);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    handler.sendEmptyMessage(GET1);
                }
                break;


            default:
                break;

        }
        return false;
    }
}