package luograduationproject.myapplication.JavaBean;

import java.io.Serializable;

import cn.bmob.v3.BmobObject;

/**
 * Created by DQ on 2017/5/19.
 */
public class Flower  extends  BmobObject implements Serializable{
    private String temp;
    private String humidity;
    private String led;

    public String getTemp() {
        return temp;
    }

    public void setTemp(String temp) {
        this.temp = temp;
    }

    public String getHumidity() {
        return humidity;
    }

    public void setHumidity(String humidity) {
        this.humidity = humidity;
    }

    public String getLed() {
        return led;
    }

    public void setLed(String led) {
        this.led = led;
    }
}
