package com.example.falling.iotserver.sokcet;

import android.os.Bundle;
import android.os.Message;

import com.example.falling.iotserver.ui.Fragment_iot_con;
import com.example.falling.iotserver.ui.Fragment_iot_dis;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 *
 */
public class dealSocket extends Thread {
    byte[] bytes = new byte[56];
    Socket s = null;
    InputStream is;
    DataInputStream in;
    DataOutputStream out;
    OutputStream os;

    public dealSocket(Socket s) {
        this.s = s;
    }

    @Override
    public void run() {
        deal();
    }

    private void deal() {

        try {
            System.out.println("socket deal start");
            while (SocketServer.isContinue) {
                is = s.getInputStream();
                os = s.getOutputStream();
                out = new DataOutputStream(os);
                in = new DataInputStream(is);
                in.read(bytes);


                //解析显示数据
                senddata(bytes);

                //发送控制数据

                if(Fragment_iot_con.Instance().getisUpdate()) {
                    byte[] myByte = Fragment_iot_con.Instance().getbytes();
                    out.write(myByte);
                    out.flush();
                }

            }
        } catch (IOException e1) {
            e1.printStackTrace();
        } finally {
            try {
                is.close();
                in.close();
                s.close();
                os.close();
                out.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    private void senddata(byte[] content) {
        Message msg = new Message();
        msg.what = 0;
        Bundle b = new Bundle();
        b.putByteArray("content", content);
        msg.setData(b);
        Fragment_iot_dis.Instance().handler.sendMessage(msg);
    }

}
