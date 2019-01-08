package com.example.falling.iotserver.ui;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.example.falling.iotserver.R;
import com.example.falling.iotserver.sokcet.DataDeal;
import com.example.falling.iotserver.sokcet.SocketServer;

import java.io.IOException;

/**
 *
 */
//显示的fragment
public class Fragment_iot_dis extends Fragment {
    private static Fragment_iot_dis iot_dis;
    private static SocketServer server;
    public Switch swiPort;//socket开关
    private TextView disID;//显示IOT 的ID （示例）
    private EditText edtPort;//输入端口号
    private TextView Heartbeat;
    private TextView Temp;
    private TextView Hum;
    private TextView RGB;
    private TextView IR;
    private TextView stuID;

    public static Fragment_iot_dis Instance() {
        if (iot_dis == null)
            iot_dis = new Fragment_iot_dis();
        return iot_dis;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_iot_dis, container, false);
        disID = (TextView) rootView.findViewById(R.id.disID);
        edtPort = (EditText) rootView.findViewById(R.id.port);
        swiPort = (Switch) rootView.findViewById(R.id.swiPort);
        Heartbeat = (TextView) rootView.findViewById(R.id.HeartBeat);
        Temp = (TextView) rootView.findViewById(R.id.TEMP);
        Hum = (TextView) rootView.findViewById(R.id.HUM);
        RGB = (TextView) rootView.findViewById(R.id.RGB);
        IR = (TextView) rootView.findViewById(R.id.IR);
        stuID = (TextView) rootView.findViewById(R.id.stuID);

        // socket的开关
        swiPort.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (swiPort.isChecked()) {
                    try {
                        //开启socket服务
                        int port = Integer.parseInt(edtPort.getText().toString());
                        server = new SocketServer(port);
                        server.start();
                        SocketServer.isContinue = true;
                        Toast.makeText(getContext(), "Socket 服务开启，端口号为:" + port, Toast.LENGTH_SHORT).show();
                    } catch (NumberFormatException e) {
                        //端口号输入不是整数
                        swiPort.setChecked(false);
                        Toast.makeText(getContext(), "输入有效的端口号！", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    try {
                        //关闭socket服务
                        if (!SocketServer.sst.isClosed()) {
                            SocketServer.sst.close();//关闭Server socket
                            SocketServer.isContinue = false; //接入的socket不在继续执行
                            Toast.makeText(getContext(), "Socket 服务关闭", Toast.LENGTH_SHORT).show();
                            System.out.println("Server close");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        return rootView;
    }

    public Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (msg.what == 0) {
                Bundle b = msg.getData();
                byte[] content = (byte[]) b.get("content");
                Fragment_iot_dis.Instance().disIOT(content);
            }
            super.handleMessage(msg);
        }
    };

    private void disIOT(byte[] bytes) {
        if (bytes[0] == (byte) 0x31 && bytes[1] == (byte) 0x50 && bytes[2] == (byte) 0x10 && bytes[3] == (byte) 0x84) {
            int id1 = DataDeal.byteToint(bytes[0]);
            int id2 = DataDeal.byteToint(bytes[1]);
            int id3 = DataDeal.byteToint(bytes[2]);
            int id4 = DataDeal.byteToint(bytes[3]);
            String str1 = Integer.toHexString(id1) + "";
            String str2 = Integer.toHexString(id2) + "";
            String str3 = Integer.toHexString(id3) + "";
            String str4 = Integer.toHexString(id4) + "";
            String strstuid = str1 + str2 + str3 + str4;
            byte ID = bytes[4];
            int heartbeat = bytes[6] & 0xFF;
            int R = DataDeal.bytesToInt(bytes, 8);
            int G = DataDeal.bytesToInt(bytes, 12);
            int B = DataDeal.bytesToInt(bytes, 16);
            int temp = DataDeal.bytesToInt(bytes, 24);
            int hum = DataDeal.bytesToInt(bytes, 28);
            int IR = DataDeal.bytesToInt(bytes, 40);

            iot_dis.disID.setText(ID + "");
            iot_dis.Heartbeat.setText(heartbeat + "");
            iot_dis.RGB.setText("R:" + R + " G:" + G + " B:" + B);
            iot_dis.Temp.setText(temp + "");
            iot_dis.Hum.setText(hum + "");
            if (IR == 1)
                iot_dis.IR.setText("开");
            else
                iot_dis.IR.setText("关");
            iot_dis.stuID.setText(strstuid);
        }
    }
}
