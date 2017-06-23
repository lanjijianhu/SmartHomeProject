package luograduationproject.myapplication.CommonFile;

import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import java.lang.reflect.Field;

import luograduationproject.myapplication.R;

public class BaseAct extends AppCompatActivity {

    protected RelativeLayout mChildContent;
    private static final int ACTIVITY_RESUME = 0;
    private static final int ACTIVITY_STOP = 1;
    private static final int ACTIVITY_PAUSE = 2;
    private static final int ACTIVITY_DESTROY = 3;
    public int activityState;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //强制竖屏
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        setTheme(null);
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(R.layout.activity_base);
        mChildContent = (RelativeLayout) findViewById(R.id.main_content);
        View childView = LayoutInflater.from(this).inflate(layoutResID,
                mChildContent);
    }


    @Override
    protected void onStart() {
        super.onStart();
        Log.d("tag", "---------onStart ");
    }

    @Override
    protected void onResume() {
        super.onResume();
        activityState = ACTIVITY_RESUME;
        Log.d("tag", "---------onResume ");
    }

    @Override
    protected void onStop() {
        super.onResume();
        super.onStop();
        activityState = ACTIVITY_STOP;
        Log.d("tag", "---------onStop ");
    }

    @Override
    protected void onPause() {
        super.onPause();
        activityState = ACTIVITY_PAUSE;
        Log.d("tag", "---------onPause ");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.d("tag", "---------onRestart ");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        activityState = ACTIVITY_DESTROY;
        Log.d("tag", "---------onDestroy ");
    }

    //设置标题栏

    public void setTheme(String color) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            View v = new View(this);
            ViewGroup.LayoutParams lParams = new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, getStatusBarHeight());
            if (color == null) {
                v.setBackgroundColor(ContextCompat.getColor(this, R.color.background_blue_white));
            } else {
                v.setBackgroundColor(Color.parseColor(color));
            }
            v.setLayoutParams(lParams);
            ViewGroup view = (ViewGroup) getWindow().getDecorView();
            view.addView(v);
            Window window = getWindow();
            window.setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS,
                    WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);

        }
    }

    // 获取手机状态栏高度
    public int getStatusBarHeight() {
        Class<?> c = null;
        Object obj = null;
        Field field = null;
        int x = 0, statusBarHeight = 0;
        try {
            c = Class.forName("com.android.internal.R$dimen");
            obj = c.newInstance();
            field = c.getField("status_bar_height");
            x = Integer.parseInt(field.get(obj).toString());
            statusBarHeight = getResources().getDimensionPixelSize(x);
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        return statusBarHeight;
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }
}
