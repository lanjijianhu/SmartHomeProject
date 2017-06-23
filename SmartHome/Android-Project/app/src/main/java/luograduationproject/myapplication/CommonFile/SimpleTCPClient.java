package luograduationproject.myapplication.CommonFile;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Serializable;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.charset.Charset;

/**
 * Created by DQ on 2017/4/19.
 */
public abstract class SimpleTCPClient implements Serializable{
    private static final int CONNECT_TIMEOUT = 3000;
    private static final int INPUT_STREAM_READ_TIMEOUT = 300;
    private Socket mSocket;
    private InputStream mInputStream;
    private OutputStream mOutputStream;
    private Context context;

    protected SimpleTCPClient(Context context){
        this.context=context;
    }


    /**
     * 连接主机
     *
     * @param host 主机IP地址
     * @param port 主机端口
     */
    boolean isConnect=false;
    public boolean connect(final String host,final int port){
        if (mSocket!=null){
            close();
        }
      Thread R= new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    mSocket=new Socket();
                    SocketAddress socketAddress=new InetSocketAddress(host, port);
                    // 设置连接超时时间
                    mSocket.connect(socketAddress, CONNECT_TIMEOUT);
                    if (mSocket.isConnected()){
                        // 设置读流超时时间，必须在获取流之前设置
                        mSocket.setSoTimeout(INPUT_STREAM_READ_TIMEOUT);
                        mInputStream = mSocket.getInputStream();
                        mOutputStream = mSocket.getOutputStream();
                        mOutputStream.flush();
                        new ReceiveThread().start();
                        isConnect=true;
                    }else{
                        mSocket.close();
                        Log.i("tag","连接异常");
                        isConnect=false;
                    }
                } catch (IOException e) {
                    Log.i("tag","连接异常");
                    isConnect=false;
                    e.printStackTrace();
                }

            }
        });
        R.start();
        try {
            R.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
      return isConnect;
    }

    /*
    接收进程
     */
    private class ReceiveThread extends Thread{
        @Override
        public void run() {
            super.run();
         while (mSocket != null && mSocket.isConnected() && mInputStream != null) {
             // 读取流
             byte[] data = new byte[0];
             try {
//                Thread.sleep(100);
                 while (mInputStream.available() > 0) {
                     byte[] buf = new byte[1024];
                     int len = mInputStream.read(buf);
                     byte[] temp = new byte[data.length + len];
                     System.arraycopy(data, 0, temp, 0, data.length);
                     System.arraycopy(buf, 0, temp, data.length, len);
                     data = temp;
                 }
             } catch (IOException e) {
                 Log.i("tag", "接收异常");
                 e.printStackTrace();
             }

             // 处理流
             if (data.length != 0) {
                     processData(data);
             }
         }
        }
    }

    /**
     * process data from received，which is not run in the main thread。
     */
    public abstract void processData(byte[] data);



    /**
     * 发送数据
     */
    public void send(final byte[] data) {
     Thread T=new Thread(new Runnable() {
            @Override
            public void run() {
                if (mSocket != null && mSocket.isConnected() && mOutputStream != null) {
                    try {
                        mOutputStream.write(data);
                        mOutputStream.flush();
                    } catch (IOException e) {
                        Toast.makeText(context,"发送失败",Toast.LENGTH_LONG).show();
                        e.printStackTrace();
                    }
                }
            }
        });
        T.start();
        try {
            T.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void send(String data) {

        if (mSocket != null) {
            send(data.getBytes(Charset.forName("UTF-8")));
        }else{
            Toast.makeText(context,"mSock为空",Toast.LENGTH_LONG);
        }

    }

    public void close() {
        if (mInputStream != null) {
            try {
                mInputStream.close();
                // mInputStream输入流不置为null，因为子线程中要用，防止空指针异常
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (mOutputStream != null) {
            try {
                mOutputStream.close();
                mOutputStream = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (mSocket != null) {
            try {
                mSocket.close();
                mSocket = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public Socket getSocket() {
        return mSocket;
    }

}
