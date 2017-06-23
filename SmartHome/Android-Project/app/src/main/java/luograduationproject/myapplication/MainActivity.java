package luograduationproject.myapplication;

import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.Toast;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import luograduationproject.myapplication.CommonFile.BaseAct;
import luograduationproject.myapplication.CommonFile.CommonData;
import luograduationproject.myapplication.CommonFile.SimpleTCPClient;

public class MainActivity extends BaseAct {
    @BindView(R.id.port_et)
    EditText getPort;
    @BindView(R.id.connect_ip)
    EditText getIP;

    private SimpleTCPClient mTCPClient;
    private boolean isConnect;
    private Dialog progressDialog;
    private View view;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        init();
    }

    @OnClick(R.id.inser)
    public void Onclick(View view){
        switch (view.getId()){
            case R.id.inser:
                CommonData.PORT=getPort.getText().toString().trim();
                CommonData.IP=getIP.getText().toString().trim();
                if (CommonData.PORT!=null&&CommonData.IP!=null){
//                startActivity(new Intent(this,ConnectAct.class));
                    buildProgressDialog();
                    mVoid();
//                    testConnect();
                }else{
                    Toast.makeText(this,"不能为空",Toast.LENGTH_LONG).show();
                }
                break;
        }
    }

    private void mVoid(){
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                testConnect();
            }
        },1000);
    }

    private void testConnect(){
        isConnect= mTCPClient.connect(CommonData.IP,Integer.parseInt(CommonData.PORT));
        if (isConnect){
            Toast.makeText(this,"成功启动",Toast.LENGTH_LONG).show();
            cancelProgressDialog();
            startActivity(new Intent(MainActivity.this,MainOperationAct.class));
//            finish();
        }else{
            cancelProgressDialog();
            Toast.makeText(this,"启动失败，请检查port与IP是否正确",Toast.LENGTH_LONG).show();
        }
    }




    public void init(){
        mTCPClient=new SimpleTCPClient(this) {
            @Override
            public void processData(byte[] data) {
            }
        };
    }




    /**
     * @Description: TODO 固定加载提示内容
     * @author Sunday
     */
    public void buildProgressDialog() {
        if (progressDialog == null) {
            progressDialog = new Dialog(this, R.style.DialogTheme);
        }
        view = LayoutInflater.from(this).inflate(R.layout.dialog, null);
        progressDialog.setContentView(view);
        progressDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
        Window dialogWindow = progressDialog.getWindow();
        WindowManager m = this.getWindowManager();
        Display d = m.getDefaultDisplay(); // 获取屏幕宽、高度
        WindowManager.LayoutParams p = dialogWindow.getAttributes(); // 获取对话框当前的参数值
        p.height = (int) (d.getHeight() * 0.6); // 高度设置为屏幕的0.6，根据实际情况调整
        p.width = (int) (d.getWidth() * 0.8); // 宽度设置为屏幕的0.65，根据实际情况调整
        dialogWindow.setAttributes(p);
        progressDialog.setCanceledOnTouchOutside(false);// 设置点击屏幕Dialog不消失
        progressDialog.show();
    }


    public void cancelProgressDialog() {
        if (progressDialog != null) {
            progressDialog.dismiss();
        }
    }


}
