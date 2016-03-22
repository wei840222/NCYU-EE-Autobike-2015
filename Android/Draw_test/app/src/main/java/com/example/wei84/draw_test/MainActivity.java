package com.example.wei84.draw_test;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //ShapeSurfaceView作為畫面
        Graphics shapeView = new Graphics(this);
        setContentView(shapeView);
        shapeView.drawing = true;

        handler.postDelayed(runnable,100);
    }

    private Handler handler = new Handler( );
    private Runnable runnable = new Runnable( ) {
        public void run ( ) {
            if (Graphics.speed >= 99)
                Graphics.speed = 0;
            else
                Graphics.speed++;
            if (Graphics.slope >= 360)
                Graphics.slope = 0;
            else
                Graphics.slope++;
            handler.postDelayed(this, 100);
        }
    };
}