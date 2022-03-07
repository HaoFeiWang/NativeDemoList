package com.whf.nativedemolist;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "NativeDemo_" + MainActivity.class.getSimpleName();

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_native_inotify).setOnClickListener(this);
        findViewById(R.id.btn_app_inotify).setOnClickListener(this);
        findViewById(R.id.btn_epoll).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_native_inotify:
                nativeInotify();
                break;
            case R.id.btn_app_inotify:
                appInotify();
                break;
            case R.id.btn_epoll:
                nativeEpoll();
                break;
        }
    }

    private void nativeEpoll() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                epoll();
            }
        }).start();
    }

    private void nativeInotify() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                inotify();
            }
        }).start();
    }

    private void appInotify() {

    }

    public native void inotify();

    public native void epoll();

}