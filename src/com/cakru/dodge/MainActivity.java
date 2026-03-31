package com.cakru.dodge;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.view.Gravity;
import android.graphics.Color;

public class MainActivity extends Activity {
    static {
        // Nama library harus sesuai dengan LOCAL_MODULE di Android.mk
        System.loadLibrary("hello");
    }

    // Nama fungsi ini akan dicari di hello.c
    public native String stringFromRust();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        TextView tv = new TextView(this);
        tv.setTextSize(25);
        tv.setGravity(Gravity.CENTER);
        tv.setTextColor(Color.BLUE);
        
        // Ambil teks dari Rust!
        tv.setText(stringFromRust());
        
        setContentView(tv);
    }
}
