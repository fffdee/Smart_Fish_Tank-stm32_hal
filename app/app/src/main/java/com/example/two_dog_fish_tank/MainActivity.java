package com.example.two_dog_fish_tank;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;

import android.os.Bundle;
import android.view.View;

import android.widget.EditText;



public class MainActivity extends AppCompatActivity implements View.OnClickListener {



    @SuppressLint("StaticFieldLeak")
    private static EditText editText;


    @SuppressLint("CommitPrefEdits")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.B1).setOnClickListener(this);
        editText = findViewById(R.id.E1);



    }
    @Override
    public void onClick(View v) {
        if(v.getId()==R.id.B1){


            Intent intent = new Intent(MainActivity.this,fishtank.class);
            Bundle bundle = new Bundle();
            bundle.putString("IP",editText.toString());
            intent.putExtras(bundle);
            startActivity(intent);

        }

    }
}