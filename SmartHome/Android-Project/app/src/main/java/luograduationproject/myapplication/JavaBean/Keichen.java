package luograduationproject.myapplication.JavaBean;

import cn.bmob.v3.BmobObject;

/**
 * Created by DQ on 2017/5/19.
 */
public class Keichen  extends BmobObject {
    private String temp;
    private String ges;
    private String led;

    public String getTemp() {
        return temp;
    }

    public void setTemp(String temp) {
        this.temp = temp;
    }

    public String getGes() {
        return ges;
    }

    public void setGes(String ges) {
        this.ges = ges;
    }

    public String getLed() {
        return led;
    }

    public void setLed(String led) {
        this.led = led;
    }
}
