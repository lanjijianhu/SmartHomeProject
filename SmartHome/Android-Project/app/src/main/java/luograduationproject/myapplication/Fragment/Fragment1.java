package luograduationproject.myapplication.Fragment;

import android.app.Activity;
import android.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.annotation.Nullable;
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
import luograduationproject.myapplication.JavaBean.Flower;
import luograduationproject.myapplication.QurryFile.QurryDataAct;
import luograduationproject.myapplication.R;

/**
 * Created by DQ on 2017/4/25.
 */
public class Fragment1 extends Fragment {
    @BindView(R.id.bnt1)
    Button Bnt1;
    @BindView(R.id.bnt2)
    Button Bnt2;
    @BindView(R.id.bnt3)
    Button Bnt3;
    @BindView(R.id.tv_recive)
    TextView tvRecive;
    @BindView(R.id.tv)
    TextView mtvConnect;
    @BindView(R.id.tempe)
    TextView tvTempe;
    @BindView(R.id.led)
    TextView tvLED;
    @BindView(R.id.humidity)
    TextView tvHumidity;


    private SimpleTCPClient mTCPClient;
    private String reciverData;

    private String TEMP = "27";
    private static String LED = "off";
    private String HUMI1 = "28";
    private String HUMI2 = "66";
    private String Tag;
    private static String ledStatus = "0";
    //广播接收
    LocalBroadcastManager broadcastManager;
    IntentFilter intentFilter;
    BroadcastReceiver mReceiver;

    public void setData(SimpleTCPClient mTCP) {
        this.mTCPClient = mTCP;

    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
    }


    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment1, null, false);
        ButterKnife.bind(this, view);
        //注册广播接收
        broadcastManager = LocalBroadcastManager.getInstance(getActivity());
        intentFilter = new IntentFilter();
        intentFilter.addAction("android.intent.action.CART_BROADCAST");
        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                reciverData = intent.getExtras().getString("ReciveData");
                Log.i("tag", "接收内容Fragment" + reciverData);
                //解析数据并显示
                byte[] bytes = reciverData.getBytes();
                Tag = new String(bytes, 0, 1);
                if (Tag.equals("1")) {
                    tvRecive.setText(reciverData);
                    TEMP = new String(bytes, 2, 2);
                    ledStatus = new String(bytes, 1, 1);
                    if (ledStatus.equals("1")) {
                        LED = "on";
                        Bnt2.setText("关闭LED");
                    } else {
                        LED = "off";
                        Bnt2.setText("打开LED");
                    }

                    HUMI1 = new String(bytes, 4, 2);  //温度
                    HUMI2 = new String(bytes, 6, 2);    //湿度
                }

                tvTempe.setText(TEMP + " C");
                tvLED.setText(LED);
                tvHumidity.setText(HUMI1 + "C" + "  " + HUMI2 + "H");

            }
        };
        broadcastManager.registerReceiver(mReceiver, intentFilter);
        return view;
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        broadcastManager.unregisterReceiver(mReceiver);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @OnClick({R.id.bnt1, R.id.bnt2, R.id.bnt3})
    public void Onclick(View view) {
        switch (view.getId()) {
            case R.id.bnt1:
                upLoadData(HUMI1, HUMI2, LED);
                break;
            case R.id.bnt2: //发送命令1
                if (CommonData.isConnect) {

                    if (ledStatus.equals("1")) {
                        mTCPClient.send("10000000");
                        Bnt2.setText("打开LED");
                    } else {
                        mTCPClient.send("11000000");
                        Bnt2.setText("关闭LED");
                    }

                } else {
                    Toast.makeText(getActivity(), "还未监听", Toast.LENGTH_SHORT).show();
                }

                break;
            case R.id.bnt3: //查询数据
//                if (CommonData.isConnect){
//                    mTCPClient.send("cmd");
//                }else{
//                    Toast.makeText(getActivity(),"还未监听",Toast.LENGTH_SHORT).show();
//                }
                Intent intent = new Intent();
                intent.setClass(getActivity(), QurryDataAct.class);
                intent.putExtra("name", "Flower");
                startActivity(intent);
                break;
        }
    }


    private Flower flower = new Flower();

    private void upLoadData(String temp, String humi, String led) {
        flower.setHumidity(humi);
        flower.setTemp(temp);
        flower.setLed(led);
        flower.save(new SaveListener<String>() {
            @Override
            public void done(String s, BmobException e) {
                if (e == null) {
                    Toast.makeText(getActivity(), "添加数据成功", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(getActivity(), "创建数据失败：" + e.getMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }



}
