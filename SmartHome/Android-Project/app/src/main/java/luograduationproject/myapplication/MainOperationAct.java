package luograduationproject.myapplication;

import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.View;
import android.widget.RadioButton;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import cn.bmob.v3.Bmob;
import luograduationproject.myapplication.CommonFile.BaseAct;
import luograduationproject.myapplication.CommonFile.CommonData;
import luograduationproject.myapplication.CommonFile.SimpleTCPClient;
import luograduationproject.myapplication.Fragment.Fragment1;
import luograduationproject.myapplication.Fragment.Fragment2;
import luograduationproject.myapplication.Fragment.Fragment3;

public class MainOperationAct extends BaseAct {
    @BindView(R.id.tab1)
    RadioButton item1;
    @BindView(R.id.tab2)
    RadioButton item2;
    @BindView(R.id.tab3)
    RadioButton item3;

    private  SimpleTCPClient mTCPClient;
    private String mReciverData;

    private Fragment1 fg1 = new Fragment1();
    private Fragment2 fg2 = new Fragment2();
    private Fragment3 fg3 = new Fragment3();
    private FragmentManager fManager;
    private String APP_ID="0c70a15be51d2274cf350f1ca000266c";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_operation);
        ButterKnife.bind(this);
        item1.setChecked(true);//默认选第一个
        fManager = getFragmentManager();
        FragmentTransaction fragmentTransaction = fManager.beginTransaction();
        fragmentTransaction.add(R.id.ly_content, fg1);
        fragmentTransaction.show(fg1);
        fragmentTransaction.commit();
        init();
        dataInit();
        BmobInit();

    }



    private void BmobInit(){
    //第一：默认初始化
    Bmob.initialize(this, APP_ID);
    // 注:自v3.5.2开始，数据sdk内部缝合了统计sdk，开发者无需额外集成，传渠道参数即可，不传默认没开启数据统计功能
    //Bmob.initialize(this, "Your Application ID","bmob");

    //第二：自v3.4.7版本开始,设置BmobConfig,允许设置请求超时时间、文件分片上传时每片的大小、文件的过期时间(单位为秒)，
    //BmobConfig config =new BmobConfig.Builder(this)
    ////设置appkey
    //.setApplicationId("Your Application ID")
    ////请求超时时间（单位为秒）：默认15s
    //.setConnectTimeout(30)
    ////文件分片上传时每片的大小（单位字节），默认512*1024
    //.setUploadBlockSize(1024*1024)
    ////文件的过期时间(单位为秒)：默认1800s
    //.setFileExpiration(2500)
    //.build();
    //Bmob.initialize(config);
}


    private void dataInit() {
        if (mTCPClient != null) {
            fg1.setData(mTCPClient);
            fg2.setData(mTCPClient);
            fg3.setData(mTCPClient);
        }

    }

    public void init() {
        mTCPClient = new SimpleTCPClient(this) {
            @Override
            public void processData(byte[] data) {
                try {
                    if (data != null) {
                        mReciverData = new String(data, "UTF-8");
                        Log.i("tag", "接收内容" + mReciverData);
                        Intent intent=new Intent("android.intent.action.CART_BROADCAST");
                        intent.putExtra("ReciveData",mReciverData);
                        LocalBroadcastManager.getInstance(MainOperationAct.this).sendBroadcast(intent);
                        MainOperationAct.this.sendBroadcast(intent);
                        Log.i("tag","广播发送成功");
                    }
                } catch (Exception e) {
                    new RuntimeException();
                }
            }
        };

        CommonData.isConnect = mTCPClient.connect(CommonData.IP, Integer.parseInt(CommonData.PORT));
    }

    @Override
    protected void onResume() {
        init();
        super.onResume();
    }

    @Override
    protected void onPause() {
        mTCPClient.close();
        super.onPause();

    }

    @Override
    protected void onDestroy() {
        mTCPClient.close();
        super.onDestroy();
    }

    @OnClick({R.id.tab1, R.id.tab2, R.id.tab3, R.id.back, R.id.set})
    public void Onclick(View view) {
        FragmentTransaction fragmentTransaction = fManager.beginTransaction();
        switch (view.getId()) {
            case R.id.tab1:
                if (!fg1.isAdded())
                    fragmentTransaction.add(R.id.ly_content, fg1);
                hideAllFragment(fragmentTransaction);
                fragmentTransaction.show(fg1);
                fragmentTransaction.commit();
                break;
            case R.id.tab2:
                if (!fg2.isAdded())
                    fragmentTransaction.add(R.id.ly_content, fg2);
                hideAllFragment(fragmentTransaction);
                fragmentTransaction.show(fg2);
                fragmentTransaction.commit();
                break;
            case R.id.tab3:
                if (!fg3.isAdded())
                    fragmentTransaction.replace(R.id.ly_content, fg3);
                hideAllFragment(fragmentTransaction);
                fragmentTransaction.show(fg3);
                fragmentTransaction.commit();
                break;
            case R.id.back:
                finish();
                break;
            case R.id.set:
//                Toast.makeText(this, "Seting", Toast.LENGTH_SHORT).show();
                break;

        }

    }

    //隐藏所有Fragment
    private void hideAllFragment(FragmentTransaction fragmentTransaction) {
        if (fg1 != null) fragmentTransaction.hide(fg1);
        if (fg2 != null) fragmentTransaction.hide(fg2);
        if (fg3 != null) fragmentTransaction.hide(fg3);
    }



}



