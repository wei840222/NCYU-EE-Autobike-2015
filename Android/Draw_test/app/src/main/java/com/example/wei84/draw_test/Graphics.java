package com.example.wei84.draw_test;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by wei84 on 2016/3/18.
 */
public class Graphics extends SurfaceView implements SurfaceHolder.Callback, Runnable {
    public static int speed = 0, slope = 0;
    public boolean drawing = false;
    private SurfaceHolder mSurfHold;

    public Graphics(Context context) {
        super(context);
        mSurfHold = getHolder();
        mSurfHold.addCallback(this);
        setFocusable(true);
    }
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        //程式畫面大小被改變時執行
    }
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //程式畫面Canvas被建立時執行
        new Thread(this).start();
    }
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        //程式畫面Canvas被銷毀時執行
    }

    public void run() {
        Canvas canvas = null;
        while(drawing) {
            try {
                canvas = mSurfHold.lockCanvas();
                synchronized (mSurfHold) {
                    background(canvas);
                    animation(canvas);
                }
            } finally {
                if (canvas != null)
                    mSurfHold.unlockCanvasAndPost(canvas);
            }
        }
    }

    public void background(Canvas canvas) {
        if (canvas == null)
            return;
        super.draw(canvas);
        //背景
        canvas.drawColor(Color.BLACK);
        //畫筆
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(30);
        //外框
        RectF rectf=new RectF();
        rectf.left = 400;
        rectf.right = 900;
        rectf.top = 325;
        rectf.bottom = 825;
        canvas.drawArc(rectf, 30, 265, false, paint);
        canvas.drawCircle(1150, 425, 400, paint);
    }

    public void animation(Canvas canvas) {
        if (canvas == null)
            return;
        //畫筆
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(5);
        //畫筆2
        Paint paint2 = new Paint();
        paint2.setColor(Color.WHITE);
        paint2.setStyle(Paint.Style.STROKE);
        paint2.setStrokeWidth(10);
        //畫筆3
        Paint paint3 = new Paint();
        paint3.setColor(Color.WHITE);
        paint3.setStyle(Paint.Style.STROKE);
        paint3.setStrokeWidth(15);
        //畫筆4(寫字)
        Paint paint4 = new Paint();
        paint4.setColor(Color.WHITE);
        paint4.setStyle(Paint.Style.FILL);
        paint4.setTypeface(Typeface.SANS_SERIF);
        //速度表
        paint4.setTextSize(200);
        if (speed < 10)
            canvas.drawText("  " + Integer.toString(speed), 490, 635, paint4);
        else
            canvas.drawText(Integer.toString(speed), 490, 635, paint4);
        paint4.setTextSize(80);
        canvas.drawText("km/h", 600, 735, paint4);
        //坡度計
        canvas.clipRect(750, 25, 1550, 825);
        canvas.save();
        canvas.rotate(slope, 1150, 425);
        canvas.drawLine(970, 555, 1090, 555, paint);
        canvas.drawLine(1030, 615, 1030, 495, paint);
        canvas.drawLine(988, 597, 1072, 513, paint);
        canvas.drawLine(1072, 597, 988, 513, paint);
        canvas.drawLine(1210, 555, 1330, 555, paint);
        canvas.drawLine(1270, 615, 1270, 495, paint);
        canvas.drawLine(1228, 597, 1312, 513, paint);
        canvas.drawLine(1312, 597, 1228, 513, paint);
        canvas.drawLine(1030, 555, 1070, 425, paint2);
        canvas.drawLine(1030, 460, 1100, 400, paint2);
        canvas.drawLine(1055, 475, 1150, 555, paint2);
        canvas.drawLine(1145, 553, 1270, 555, paint2);
        canvas.drawLine(1270, 555, 1205, 485, paint2);
        canvas.drawLine(1210, 490, 1055, 475, paint2);
        canvas.drawLine(1145, 550, 1220, 465, paint2);
        canvas.drawLine(1190, 467, 1235, 465, paint2);
        canvas.drawCircle(1030, 555, 60, paint3);
        canvas.drawCircle(1270, 555, 60, paint3);
        canvas.drawLine(800, 625, 1500, 625, paint3);
        canvas.restore();

    }

    public void setValue(int speed, int slope) {
        this.speed = speed;
        this.slope = slope;
    }
}
