package luograduationproject.myapplication.Fragment;

import android.app.Activity;
import android.app.Fragment;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import cn.bmob.v3.exception.BmobException;
import cn.bmob.v3.listener.SaveListener;
import luograduationproject.myapplication.CommonFile.CommonData;
import luograduationproject.myapplication.CommonFile.SimpleTCPClient;
import luograduationproject.myapplication.JavaBean.Keichen;
import luograduationproject.myapplication.QurryFile.QurryDataAct;
import luograduationproject.myapplication.R;
/**
 * Created by DQ on 2017/4/25.
 */
public class Fragment3  extends Fragment {
    @BindView(R.id.tv_recive)
    TextView tvRecive;
    @BindView(R.id.tv)
    TextView mtvConnect;

    @BindView(R.id.tempe)
    TextView tvTempe;
    @BindView(R.id.led)
    TextView tvLED;
    @BindView(R.id.gas_test)
    TextView tvGAS;
    @BindView(R.id.bnt2)
    Button Bnt2;

    private String TEMP="28";
    private String LED="off";
    private String GST="Security";
    private String Tag;
    private static String ledstatus;

    private SimpleTCPClient mTCPClient;
    //广播接收
    LocalBroadcastManager broadcastManager;
    IntentFilter intentFilter;
    BroadcastReceiver mReceiver;

    private String reciverData;


    public Fragment3(){}


    public void setData(SimpleTCPClient mTCPClient) {
        this.mTCPClient = mTCPClient;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment3, null, false);
        ButterKnife.bind(this,view);
        //注册广播接收
        broadcastManager = LocalBroadcastManager.getInstance(getActivity());
        intentFilter = new IntentFilter();
        intentFilter.addAction("android.intent.action.CART_BROADCAST");
        mReceiver=new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                reciverData=intent.getExtras().getString("ReciveData");
                Log.i("tag", "接收内容Fragment" + reciverData);
//                tvRecive.setText(reciverData);
                byte[] bytes=reciverData.getBytes();
                 Tag=new String(bytes,0,1);
                if (Tag.equals("3")) {
                    tvRecive.setText(reciverData);
                    TEMP = new String(bytes, 2, 2);
                    ledstatus = new String(bytes, 1, 1);
                    if (ledstatus.equals("1")) {
                        LED = "on";
                        Bnt2.setText("关闭LED");
                    }
                    else{
                        LED="off";
                        Bnt2.setText("打开LED");
                    }
                    GST = new String(bytes, 4, 3);
                   if(Integer.valueOf(GST)>13){
                       GST="dangerous";
                       Notificatin("厨房");
                   }
                }
                tvTempe.setText(TEMP+" C");
                tvLED.setText(LED);
                tvGAS.setText(GST);



            }
        };
        broadcastManager.registerReceiver(mReceiver, intentFilter);
//        init();
        return view;
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
    }



    @Override
    public void onDestroy() {
        super.onDestroy();
        broadcastManager.unregisterReceiver(mReceiver);
    }





    @OnClick({R.id.bnt1,R.id.bnt2,R.id.bnt3})
    public void Onclick(View view){
        switch (view.getId()){
            case R.id.bnt1://启动监听，连接
//                if (CommonData.isConnect){
//                    mTCPClient.send("cmd1");
//                }else{
//                    Toast.makeText(getActivity(),"还未监听",Toast.LENGTH_SHORT).show();
//                }
                upLoadData(TEMP,GST,LED);
                break;
            case R.id.bnt2: //发送命令1
                if (CommonData.isConnect){
                    if(ledstatus.equals("1")) {
                        mTCPClient.send("30000000");
                        Bnt2.setText("打开LED");
                    }
                    else{
                        mTCPClient.send("31000000");
                        Bnt2.setText("关闭LED");
                    }
                }else{
                    Toast.makeText(getActivity(),"还监听",Toast.LENGTH_SHORT).show();
                }

                break;
            case R.id.bnt3: //发送命令2
//                if (CommonData.isConnect){
//                    mTCPClient.send("cmd3");
//                }else{
//                    Toast.makeText(getActivity(),"还未监听",Toast.LENGTH_SHORT).show();
//                }
                Intent intent=new Intent();
                intent.setClass(getActivity(), QurryDataAct.class);
                intent.putExtra("name","Keichen");
                startActivity(intent);
                break;

        }
    }



private Keichen keichen=new Keichen();
    private void upLoadData(String temp,String ges,String led){
        keichen.setGes(ges);
        keichen.setTemp(temp);
        keichen.setLed(led);
        keichen.save(new SaveListener<String>() {
            @Override
            public void done(String s, BmobException e) {
                if(e==null){
                    Toast.makeText(getActivity(),"添加数据成功",Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getActivity(),"创建数据失败：" + e.getMessage(),Toast.LENGTH_SHORT).show();
                }
            }
        });
    }


    /*通知消息函數*/
    private NotificationManager mNManager;
    private Notification notify1;
    private static final int NOTIFYID_1 = 1;
    public void Notificatin(String name) {
        Notification.Builder mBuilder=new Notification.Builder(getActivity());
        mBuilder.setContentTitle("通知")
                .setContentText(name+": 可燃气体超出正常范围！")
//                .setSubText("子內容")
                .setTicker("收到报警消息")
                .setWhen(System.currentTimeMillis())
                .setSmallIcon(R.mipmap.warning_icon)            //设置小图标
                .setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_VIBRATE)    //设置默认的三色灯与振动器
                .setAutoCancel(true) ;                          //设置点击后取消Notification
        notify1 = mBuilder.build();
        Log.i("TAG","产生的随机数是："+(int) (Math.random() * 100));
        mNManager.notify((int) (Math.random() * 100), notify1);

    }

}
