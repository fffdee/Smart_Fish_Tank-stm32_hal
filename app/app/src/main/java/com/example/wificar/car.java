package com.example.wificar;

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
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpCookie;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Calendar;

public class car extends AppCompatActivity implements View.OnTouchListener,View.OnClickListener{
    private Handler handler;
    private final int LEFT = 1;
    private final int RIGHT = 2;
    private final int GO = 3;
    private final int BACK = 4;
    private final int GET1 = 5;
    private final int GET2 = 6;
    private final int GET3 = 7;
    private final int GET4 = 8;
    private final int FAN = 9;
    private final int TIME= 10;
    private ToggleButton toggleButton,T1;
    String ip;
    private EditText editText1,editText2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_car);
        findViewById(R.id.GO).setOnTouchListener(this);
        findViewById(R.id.BACK).setOnTouchListener(this);
        findViewById(R.id.LEFT).setOnTouchListener(this);
        findViewById(R.id.RIGHT).setOnTouchListener(this);
        findViewById(R.id.FAN).setOnClickListener(this);
        toggleButton = findViewById(R.id.FAN);
        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        ip = bundle.getString("IP","");
        Toast.makeText(car.this,"获取IP:"+ip,Toast.LENGTH_SHORT).show();
        HandlerThread handlerThread = new HandlerThread("Http");
        handlerThread.start();
        editText1 = findViewById(R.id.E1);
        editText2 = findViewById(R.id.E2);

        T1 = findViewById(R.id.TB1);
        findViewById(R.id.TB1).setOnClickListener(this);
        handler = new HttpHandler(handlerThread.getLooper());

    }

    @Override
    public void onClick(View v) {
        if(v.getId()==R.id.FAN){
            handler.sendEmptyMessage(FAN);
        }
        else if(v.getId()==R.id.TB1){
            handler.sendEmptyMessage(TIME);
        }

    }

    private class HttpHandler extends Handler {
        public HttpHandler(Looper looper) {
        super(looper);
    }

        @Override
        public void handleMessage(Message msg) {
        switch (msg.what) {
            case LEFT:
                left();
                break;
            case RIGHT:
                right();
                break;
            case GO:
                go();
                break;
            case BACK:
                back();
                break;
            case GET1:
                get1();
                break;
            case GET2:
                get2();
                break;
            case GET3:
                get3();
                break;
            case GET4:
                get4();
                break;
            case FAN:
                fan();
                break;
            case TIME:
                time();
                break;
            default:
                break;
        }
    }




    }
    private void time() {

            if(T1.isChecked()){

                String hour = editText1.getText().toString();
                String min  = editText2.getText().toString();

                String registerUrl = "http://"+ip+"/Time?hour="+hour+"&min="+min+"&val=1";
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

                String hour = editText1.getText().toString();
                String min  = editText2.getText().toString();
                String registerUrl = "http://"+ip+"/Time?hour="+hour+"&min="+min+"&val=0";
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
    private void fan(){
        if(toggleButton.isChecked()){


                String registerUrl = "http://"+ip+"/control?fan=0";
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


                String registerUrl = "http://"+ip+"/control?fan=1";
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
    private void go() {


        String registerUrl = "https://api.bemfa.com/api/device/v1/data/1?uid=c03b0385d6468c31245bd9f86236fc4d&topic=led&msg=on";
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
    private void get1() {


        String registerUrl = "https://api.bemfa.com/api/device/v1/data/1?uid=c03b0385d6468c31245bd9f86236fc4d&topic=led&msg=off";
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
    private void left() {


        String registerUrl = "http://"+ip+"/control?left=1";
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


        String registerUrl = "http://"+ip+"/control?left=0";
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
    private void right() {

        String registerUrl = "http://"+ip+"/control?right=1";
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
    private void get3() {


        String registerUrl = "http://"+ip+"/control?right=0";
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
    private void get4() {


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
            case R.id.RIGHT:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(RIGHT);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    handler.sendEmptyMessage(GET3);
                }
                break;
            case R.id.LEFT:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(LEFT);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    handler.sendEmptyMessage(GET2);
                }
                break;
            case R.id.BACK:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(BACK);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    handler.sendEmptyMessage(GET4);
                }
                break;
            case R.id.GO:
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    handler.sendEmptyMessage(GO);
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