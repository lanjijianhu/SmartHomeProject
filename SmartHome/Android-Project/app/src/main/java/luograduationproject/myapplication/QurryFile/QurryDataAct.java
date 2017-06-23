package luograduationproject.myapplication.QurryFile;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import cn.bmob.v3.BmobBatch;
import cn.bmob.v3.BmobObject;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.datatype.BatchResult;
import cn.bmob.v3.exception.BmobException;
import cn.bmob.v3.listener.FindListener;
import cn.bmob.v3.listener.QueryListListener;
import luograduationproject.myapplication.CommonFile.BaseAct;
import luograduationproject.myapplication.JavaBean.Flower;
import luograduationproject.myapplication.JavaBean.Keichen;
import luograduationproject.myapplication.R;

public class QurryDataAct extends BaseAct {

    private FlowerAdap mAdap;
    private List<Flower> mFlowerslist;
    private List<Keichen> mKeichenlist;
    private ListView lv;
    private TextView mTVTip;
    private String Name;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qurry_data);
        Name= getIntent().getStringExtra("name");
        initView();
    }

    private  void  initView(){
        TextView title=(TextView)findViewById(R.id.title);
        TextView temp=(TextView)findViewById(R.id.part1);
        TextView humi=(TextView)findViewById(R.id.part2);
        TextView led=(TextView)findViewById(R.id.part3);
        TextView cleanBtn=(TextView)findViewById(R.id.tv_clean);

        lv=(ListView)findViewById(R.id.lv);
        mTVTip=(TextView)findViewById(R.id.tv_tip);

        if (Name.equals("Flower")) {
            title.setText("阳台节点");
            temp.setText("温度.C");
            humi.setText("湿度.H");
            led.setText("灯光");
            QurryData();
        } else if(Name.equals("Keichen")) {
            title.setText("厨房节点");
            temp.setText("温度.C");
            humi.setText("气体检测.H");
            led.setText("灯光");
            QurryData2();
        }

        cleanBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (Name.equals("Flower")&&mFlowerslist!=null) {
                    cleanFlowerData();
                    mAdap.notifyDataSetChanged();

                } else if(Name.equals("Keichen")&&mKeichenlist!=null) {
                    cleanFlowerData();
                    mAdap.notifyDataSetChanged();

                }else{
                    Toast.makeText(QurryDataAct.this,"无清除记录",Toast.LENGTH_LONG).show();
                }
            }
        });


    }

    //执行查询方法  查询阳台
    private void QurryData(){
        BmobQuery<Flower> query = new BmobQuery<Flower>();
        query.setLimit(50);
        query.findObjects(new FindListener<Flower>() {
            @Override
            public void done(List<Flower> list, BmobException e) {
                if(e==null){
//                    ("查询成功：共"+list.size()+"条数据。");
                    Toast.makeText(QurryDataAct.this,"查询成功：共"+list.size()+"条数据。",Toast.LENGTH_SHORT);
                    lv.setVisibility(View.VISIBLE);
                    mTVTip.setVisibility(View.GONE);
                     Collections.reverse(list);
                    mFlowerslist=list;
                    mAdap=new FlowerAdap(QurryDataAct.this,mFlowerslist);
                    lv.setAdapter(mAdap);
                }else{
                    Log.i("bmob","失败："+e.getMessage()+","+e.getErrorCode());
                    Toast.makeText(QurryDataAct.this,"失败："+e.getMessage()+","+e.getErrorCode(),Toast.LENGTH_SHORT).show();
                    lv.setVisibility(View.GONE);
                    mTVTip.setVisibility(View.VISIBLE);
                }
            }
        });
    }



    //删除指令函数
public void cleanFlowerData(){
    List<BmobObject> deleData = new ArrayList<BmobObject>();

    if (Name.equals("Flower")) {
        for (Flower f : mFlowerslist) {
            deleData.add(f);
        }
    }else if(Name.equals("Keichen")){
        for (Keichen k : mKeichenlist) {
            deleData.add(k);
        }
    }

    //第二种方式：v3.5.0开始提供
    new BmobBatch().deleteBatch(deleData).doBatch(new QueryListListener<BatchResult>() {

        @Override
        public void done(List<BatchResult> o, BmobException e) {
            if(e==null){
                for(int i=0;i<o.size();i++){
                    BatchResult result = o.get(i);
                    BmobException ex =result.getError();
                    if(ex==null){
                        Log.i("TAG","第"+i+"个数据批量删除成功");
                    }else{
                        Log.i("TAG","第"+i+"个数据批量删除失败："+ex.getMessage()+","+ex.getErrorCode());
                    }
                }
            }else{
                Log.i("bmob","失败："+e.getMessage()+","+e.getErrorCode());
            }
        }
    });

}


    //执行查询方法  查询厨房
    private void QurryData2(){
        BmobQuery<Keichen> query = new BmobQuery<Keichen>();
        query.setLimit(50);
        query.findObjects(new FindListener<Keichen>() {
            @Override
            public void done(List<Keichen> list, BmobException e) {

                if(e==null){
//                    ("查询成功：共"+list.size()+"条数据。");
                    Toast.makeText(QurryDataAct.this,"查询成功：共"+list.size()+"条数据。",Toast.LENGTH_SHORT);
                    lv.setVisibility(View.VISIBLE);
                    mTVTip.setVisibility(View.GONE);
                    mKeichenlist=list;
                    Collections.reverse(list);
                    mAdap=new FlowerAdap(true,QurryDataAct.this,list);
                    lv.setAdapter(mAdap);
                }else{
                    Log.i("bmob","失败："+e.getMessage()+","+e.getErrorCode());
                    Toast.makeText(QurryDataAct.this,"失败："+e.getMessage()+","+e.getErrorCode(),Toast.LENGTH_SHORT).show();
                    lv.setVisibility(View.GONE);
                    mTVTip.setVisibility(View.VISIBLE);
                }

            }
        });

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
