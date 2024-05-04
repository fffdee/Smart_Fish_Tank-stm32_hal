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
import android.widget.ProgressBar;
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

import static java.sql.Types.NULL;

public class fishtank extends AppCompatActivity implements View.OnClickListener{
    private Handler handler;
    private final int MOTOR1 = 1;
    private final int MOTOR2 = 2;
    private final int SEND = 3;
    private final int GET1 = 5;
    private final int EAT = 6;
    private final int HOT = 9;

    private ToggleButton toggleButton,PrevalMode,Motor1,Motor2;

    private EditText editText;
    private TextView Ttemp,Tptemp,Tzhuod,Tpzhuod,motor1,motor2,hotflag,waterflag;
    int tempVal,PtempVal,zhuod,Pzhuod,motor1Flag,motor2Flag,hotFlag,waterFlagDown,waterFlagUp,prevalModeFlag;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fishtank);


        findViewById(R.id.MOTOR1).setOnClickListener(this);
        findViewById(R.id.M2).setOnClickListener(this);
        findViewById(R.id.HOT).setOnClickListener(this);
        findViewById(R.id.Eat).setOnClickListener(this);

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
        Motor1 = findViewById(R.id.MOTOR1);
        Motor2 = findViewById(R.id.M2);
        PrevalMode = findViewById(R.id.SetMode);

        findViewById(R.id.Send).setOnClickListener(this);
        findViewById(R.id.SetMode).setOnClickListener(this);



        HandlerThread handlerThread = new HandlerThread("Http");
        handlerThread.start();


        handler = new HttpHandler(handlerThread.getLooper());
        @SuppressLint("SetTextI18n") Thread thread = new Thread(() -> {
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
                    //获取的消息处理
                    String rev_msg = jsonObject.optString("data");
                    String msg2 = rev_msg.replace("["," ");
                    rev_msg = msg2.replace("]"," ");
                    Log.d("msg",rev_msg);
                    JSONObject msg_data = new JSONObject(rev_msg);
                    String msg_json = msg_data.optString("msg");
                    Log.d("msg", msg_json);
                    //处理msg的json信息
                    JSONObject json_data = new JSONObject(msg_json);
                    tempVal =  json_data.optInt("temp");
                    PtempVal =  json_data.optInt("Ptemp");
                    if(json_data.optInt("zhuod")*10<1500)
                    zhuod =  json_data.optInt("zhuod")*10;
                    if(json_data.optInt("Pzhuod")!=0)
                    Pzhuod =  json_data.optInt("Pzhuod");
                    motor1Flag =  json_data.optInt("motor1");
                    motor2Flag =  json_data.optInt("motor2");
                    hotFlag =  json_data.optInt("hot");
                    waterFlagDown =  json_data.optInt("waterFlagDown");
                    waterFlagUp =  json_data.optInt("waterFlagUp");
                    Ttemp.setText("当前温度："+tempVal+"°");
                    Tptemp.setText("预设温度："+ PtempVal+"°");
                    Tzhuod.setText("当前浊度："+zhuod);
                    Tpzhuod.setText("预设浊度："+Pzhuod+"0");
                    if(motor1Flag==0) {
                        motor1.setText("供氧：关闭");
                        Motor1.setChecked(true);
                    } else{
                        motor1.setText("供氧：启动");
                        Motor1.setChecked(false);
                    }
                    if(motor2Flag==0) {
                        motor2.setText("过滤：关闭");
                        Motor2.setChecked(true);
                    }
                    else {
                        motor2.setText("过滤：启动");
                        Motor2.setChecked(false);
                    }
                    if(hotFlag==0) {
                        hotflag.setText("加热：关闭");
                        toggleButton.setChecked(false);
                    }
                    else {
                        hotflag.setText("加热：打开");
                        toggleButton.setChecked(true);
                    }
                    waterFlagDown  = 0;

                } catch (Exception e)
                {
                    e.printStackTrace();
                }
                // 延迟10秒
                try {
                    Thread.sleep(1000);
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

        else if(v.getId()==R.id.Send)
            handler.sendEmptyMessage(SEND);

        else if(v.getId()==R.id.SetMode) {
            if(PrevalMode.isChecked()) prevalModeFlag=1;
            else prevalModeFlag = 0;

        }
        else if(v.getId()==R.id.MOTOR1) {
            handler.sendEmptyMessage(MOTOR1);

        }
        else if(v.getId()==R.id.M2) {
            handler.sendEmptyMessage(MOTOR2);

        }else if(v.getId()==R.id.Eat) {
            handler.sendEmptyMessage(EAT);

        }

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

                case SEND:
                    send();
                    break;
                case HOT:
                    HOT();
                    break;
                case EAT:
                    EAT();
                    break;
                default:
                    break;
            }
        }




    }

    private void EAT() {
        waterFlagDown  = 1;
        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                            ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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

    private void HOT(){
        if(toggleButton.isChecked()){
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
            hotFlag = 1;
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
            hotFlag = 0;
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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
    private void MOTOR2() {
        if(Motor2.isChecked()){
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
            motor2Flag = 0;
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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
            motor2Flag = 1;
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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
    private void MOTOR1() {



        if(Motor1.isChecked()){
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
            motor1Flag = 0;
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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

                if(editText.getText().toString().equals("")) {
                    Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
                }else {

                    Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
                }

            motor1Flag = 1;
            try
            {
                OkHttpClient client1 =new OkHttpClient();
                FormBody formBody=new FormBody.Builder()
                        .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                        .add("topic","fishtank")
                        .add("type","3")
                        .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                                ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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
    private void send() {

        if(prevalModeFlag==1){
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "浊值为空", Toast.LENGTH_SHORT).show();
            }else {

                Pzhuod = Integer.parseInt(String.valueOf(editText.getText()))/10;
            }
        }
        else {
            if(editText.getText().toString().equals("")) {
                Toast.makeText(this, "温度值为空", Toast.LENGTH_SHORT).show();
            }else{
                if (Integer.parseInt(String.valueOf(editText.getText())) > 40
                        || Integer.parseInt(String.valueOf(editText.getText())) < 10

                )
                    Toast.makeText(this, "温度值不合适", Toast.LENGTH_SHORT).show();
                else PtempVal = Integer.parseInt(String.valueOf(editText.getText()));
            }
        }

        try
        {
            OkHttpClient client1 =new OkHttpClient();
            FormBody formBody=new FormBody.Builder()
                    .add("uid","c03b0385d6468c31245bd9f86236fc4d")
                    .add("topic","fishtank")
                    .add("type","3")
                    .add("msg","{\"temp\":"+tempVal+",\"Ptemp\":"+PtempVal+",\"zhuod\":"+zhuod+",\"Pzhuod\":"+Pzhuod+",\"motor1\":"+motor1Flag+
                            ",\"motor2\":"+motor2Flag+",\"hot\":"+hotFlag+",\"waterFlagDown\":"+waterFlagDown+",\"waterFlagUp\":"+waterFlagUp+"}")
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


}