package luograduationproject.myapplication.QurryFile;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

import luograduationproject.myapplication.JavaBean.Flower;
import luograduationproject.myapplication.JavaBean.Keichen;
import luograduationproject.myapplication.R;

/**
 * Created by DQ on 2017/5/19.
 */
public class FlowerAdap extends BaseAdapter {

    private List<Flower> list;
    private List<Keichen>  mKeichenList;
    private Context context;
    private boolean isKeichen=false;

    public  FlowerAdap(Context context,List<Flower> list){
        this.list=list;
        this.context=context;
    }

    public  FlowerAdap(Boolean iskeichen,Context context,List<Keichen> list){
        this.mKeichenList=list;
        this.context=context;
        this.isKeichen=iskeichen;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
     ViewHolder holder=null;
        if (convertView==null){
            holder=new ViewHolder();
            convertView= LayoutInflater.from(context).inflate(R.layout.lv_item,null);
            holder.parm0=(TextView)convertView.findViewById(R.id.time);
            holder.parm1=(TextView)convertView.findViewById(R.id.part1);
            holder.parm2=(TextView)convertView.findViewById(R.id.part2);
            holder.parm3=(TextView)convertView.findViewById(R.id.part3);
            convertView.setTag(holder);
        }else{
            holder=(ViewHolder)convertView.getTag();
        }
        if (isKeichen) {
            holder.parm0.setText(mKeichenList.get(position).getCreatedAt());
            holder.parm1.setText(mKeichenList.get(position).getTemp());
            holder.parm2.setText(mKeichenList.get(position).getGes());
            holder.parm3.setText(mKeichenList.get(position).getLed());
        }else{

            holder.parm0.setText(list.get(position).getCreatedAt());
            holder.parm1.setText(list.get(position).getTemp());
            holder.parm2.setText(list.get(position).getHumidity());
            holder.parm3.setText(list.get(position).getLed());
        }


        return convertView;
    }
    @Override
    public int getCount() {
        if (isKeichen)
            return mKeichenList.size();
        else
        return list.size();
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    private final class ViewHolder{
        private  TextView parm0;
        private TextView parm1;
        private TextView parm2;
        private TextView parm3;
    }
}
