package luograduationproject.myapplication.JavaBean;

import cn.bmob.v3.BmobObject;

/**
 * Created by DQ on 2017/5/19.
 */
public class TheDoor extends BmobObject {
    private String temp;
    private String alarm;
    private String bee;

    public String getTemp() {
        return temp;
    }

    public void setTemp(String temp) {
        this.temp = temp;
    }

    public String getAlarm() {
        return alarm;
    }

    public void setAlarm(String alarm) {
        this.alarm = alarm;
    }

    public String getBee() {
        return bee;
    }

    public void setBee(String bee) {
        this.bee = bee;
    }
}
