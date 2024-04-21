package com.example.two_dog_fish_tank;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpCookie;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Calendar;

import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class fishtank extends AppCompatActivity implements View.OnTouchListener,View.OnClickListener{
    private Handler handler;
    private final int MOTOR1 = 1;
    private final int MOTOR2 = 2;
    private final int SEND = 3;
    private final int GET1 = 5;
    private final int GET2 = 6;
    private final int HOT = 9;

    private ToggleButton toggleButton,T1;

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


        HandlerThread handlerThread = new HandlerThread("Http");
        handlerThread.start();


        handler = new HttpHandler(handlerThread.getLooper());
        Thread thread = new Thread(() -> {
            // 循环执行
            while (true) {
                // 执行某个任务
                try
                {
                    OkHttpClient client =new OkHttpClient();
                    Request request=new Request.Builder().url("https://apis.bemfa.com/va/getmsg?uid=c03b0385d6468c31245bd9f86236fc4d&topic=fishtank&type=3").build();//使用newcall()创建一个call对象，并调用
                    Response response= client.newCall(request).execute();
                    //得到返回数据
                    String data= response.body().string();
                    Log.d("update", data);
                    //post方式结束
                    JSONObject jsonObject = new JSONObject(data);

                    String msg = jsonObject.optString("data");
                    String msg2 = msg.replace("["," ");
                    msg = msg2.replace("]"," ");
                    Log.d("msg",msg);
                    JSONObject msg_data = new JSONObject(msg);

                    String msg_1 = msg_data.optString("msg");

                    Log.d("msg",msg_1);


                } catch (Exception e)
                {
                    e.printStackTrace();
                }
                // 延迟10秒
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
// 启动线程
        thread.start();

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


            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","on")
                        .build();
                Request request1=new Request.Builder()
                        .url("https://apis.bemfa.com/va/postmsg")
                        .post(formBody)
                        .build();

                Response response1= client1.newCall(request1).execute();
                //得到返回数据
                String data1= response1.body().string();
                //将返回数据添加到ui页面上
                //Ttemp.setText(data1);
                //post方式结束


            } catch (Exception e)
            {
                e.printStackTrace();
            }

        }else{


            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","off")
                        .build();
                Request request1=new Request.Builder()
                        .url("https://apis.bemfa.com/va/postmsg")
                        .post(formBody)
                        .build();

                Response response1= client1.newCall(request1).execute();
                //得到返回数据
                String data1= response1.body().string();
                //将返回数据添加到ui页面上
              //  Ttemp.setText(data1);
                //post方式结束

            } catch (Exception e)
            {
                e.printStackTrace();
            }

        }
    }


    @SuppressLint("SetTextI18n")
    private void get1() {


        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","ok")
                    .build();
            Request request1=new Request.Builder()
                    .url("https://apis.bemfa.com/va/postmsg")
                    .post(formBody)
                    .build();

            Response response1= client1.newCall(request1).execute();
            //得到返回数据
            String data1= response1.body().string();
            //将返回数据添加到ui页面上
            //Ttemp.setText(data1);
            //post方式结束

            Log.d("respond:",data1);
            //post方式结束

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void MOTOR1() {



        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","ok")
                    .build();
            Request request1=new Request.Builder()
                    .url("https://apis.bemfa.com/va/postmsg")
                    .post(formBody)
                    .build();

            Response response1= client1.newCall(request1).execute();
            //得到返回数据
            String data1= response1.body().string();
            //将返回数据添加到ui页面上
            //Ttemp.setText(data1);
            //post方式结束

            Ttemp.setText(data1);
            //post方式结束

        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void get2() {



        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","ok")
                    .build();
            Request request1=new Request.Builder()
                    .url("https://apis.bemfa.com/va/postmsg")
                    .post(formBody)
                    .build();

            Response response1= client1.newCall(request1).execute();
            //得到返回数据
            String data1= response1.body().string();
            //将返回数据添加到ui页面上
            //Ttemp.setText(data1);
            //post方式结束


        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    @SuppressLint("SetTextI18n")
    private void MOTOR2() {


        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","ok")
                    .build();
            Request request1=new Request.Builder()
                    .url("https://apis.bemfa.com/va/postmsg")
                    .post(formBody)
                    .build();

            Response response1= client1.newCall(request1).execute();
            //得到返回数据
            String data1= response1.body().string();
            //将返回数据添加到ui页面上
            //Ttemp.setText(data1);
            //post方式结束


        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }


    @SuppressLint("SetTextI18n")
    private void send() {



        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","ok")
                    .build();
            Request request1=new Request.Builder()
                    .url("https://apis.bemfa.com/va/postmsg")
                    .post(formBody)
                    .build();

            Response response1= client1.newCall(request1).execute();
            //得到返回数据
            String data1= response1.body().string();
            //将返回数据添加到ui页面上
            //Ttemp.setText(data1);
            //post方式结束


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