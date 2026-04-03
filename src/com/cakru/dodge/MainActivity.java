package com.cakru.dodge;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("hello");
    }

    // Satu-satunya pintu masuk ke Rust
    public native String prosesDiRust(String input);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText input = findViewById(R.id.input_teks);
        Button btn = findViewById(R.id.btn_proses);
        TextView output = findViewById(R.id.hasil_rust);

        btn.setOnClickListener(v -> {
            String teksUser = input.getText().toString();
            // Kirim ke JNI -> Rust
            String hasil = prosesDiRust(teksUser);
            output.setText(hasil);
        });
    }
}
