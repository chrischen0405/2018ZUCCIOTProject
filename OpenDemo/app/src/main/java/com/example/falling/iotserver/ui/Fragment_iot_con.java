package com.example.falling.iotserver.ui;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Switch;

import com.example.falling.iotserver.R;
import com.example.falling.iotserver.sokcet.DataDeal;


/**
 *
 */
//控制的fragment
public class Fragment_iot_con extends Fragment {
    private static Fragment_iot_con iot_con;
    private Switch Led;
    private SeekBar RGB_R;
    private SeekBar RGB_G;
    private SeekBar RGB_B;
    private SeekBar Speed;
    private Button Send ;
    private EditText Edit;

    private Boolean isUpdate = false;
    private byte heartbeat = 1;
    byte[] sendData = new byte [95];
    byte led;
    private String content = "test";


    public static Fragment_iot_con Instance() {
        if (iot_con == null)
            iot_con = new Fragment_iot_con();
        return iot_con;
    }


    //控制的fragment
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_iot_con, container, false);
        Led = (Switch) rootView.findViewById(R.id.LED);
        RGB_R = (SeekBar) rootView.findViewById( R.id.RGB_R );
        RGB_R.setMax( 255 );
        RGB_G = (SeekBar) rootView.findViewById( R.id.RGB_G );
        RGB_G.setMax( 255 );
        RGB_B = (SeekBar) rootView.findViewById( R.id.RGB_B );
        RGB_B.setMax( 255 );
        Speed = (SeekBar) rootView.findViewById( R.id.Speed );
        Speed.setMax( 10 );
        Edit = (EditText) rootView.findViewById( R.id.Content );
        Send = (Button) rootView.findViewById( R.id.send );

        Led.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener(){
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(Led.isChecked())
                    led = 0x01;
                else
                    led = 0x00;
            }
        });

        Send.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                int r = RGB_R.getProgress();
                int g = RGB_G.getProgress();
                int b = RGB_B.getProgress();
                int speed = Speed.getProgress();
                iot_con.Instance().sendData[0] = 0x31;
                iot_con.Instance().sendData[1] = 0x50;
                iot_con.Instance().sendData[2] = 0x10;
                iot_con.Instance().sendData[3] = (byte) 0x84;
                iot_con.Instance().sendData[4] = 0x04;
                iot_con.Instance().sendData[5] = iot_con.Instance().heartbeat;
                byte[] b1 = DataDeal.IntTobytes( r );
                byte[] b2 = DataDeal.IntTobytes( g );
                byte[] b3 = DataDeal.IntTobytes( b );
                byte[] b4 = DataDeal.IntTobytes( speed );
                for(int i = 0;i<4;i++){
                    iot_con.Instance().sendData[i+6] = b1[i];
                    iot_con.Instance().sendData[i+10] = b2[i];
                    iot_con.Instance().sendData[i+14] = b3[i];
                    iot_con.Instance().sendData[i+18] = b4[i];
                }
                iot_con.Instance().sendData[30] = led;
                iot_con.Instance().isUpdate = true;
                iot_con.Instance().heartbeat++;
                content = Edit.getText().toString();
                for(int i = 42;i < 94; i++)
                    sendData[i] = 0;
                for(int i = 0;i < content.getBytes().length;i++){
                    iot_con.Instance().sendData[42+i] = content.getBytes()[i];
                }
            }
        });

        return rootView;
    }

    public boolean getisUpdate(){
        return iot_con.Instance().isUpdate;
    }
    public byte[] getbytes(){
        return iot_con.Instance().sendData;
    }
}