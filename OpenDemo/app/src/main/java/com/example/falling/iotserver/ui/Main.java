package com.example.falling.iotserver.ui;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import com.example.falling.iotserver.R;
import com.example.falling.iotserver.sokcet.SocketServer;

import java.io.IOException;

public class Main extends AppCompatActivity {

    private SectionsPagerAdapter mSectionsPagerAdapter;
    private ViewPager mViewPager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_iot_server);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        setTitle("传感器"); //显示toolbar的默认title

        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());
        mViewPager = (ViewPager) findViewById(R.id.container);
        mViewPager.setAdapter(mSectionsPagerAdapter);


        //左右切换fragment的时候改变title
        mViewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
            }

            @Override
            public void onPageSelected(int position) {
                //页面切换的时候改变title
                if (position == 0)
                    setTitle("传感器");
                else if (position == 1)
                    setTitle("控制器");
            }

            @Override
            public void onPageScrollStateChanged(int state) {
            }
        });
    }


    @Override
    protected void onStop() {
        super.onStop();

        //socket 在程序处于后台或者屏幕关闭的时候关闭socket
        try {
            if (!SocketServer.sst.isClosed()) {
                SocketServer.sst.close(); //关闭Server socket
                SocketServer.isContinue = false;//停止 socket继续执行
                Fragment_iot_dis.Instance().swiPort.setChecked(false);
                System.out.println("Server close");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            if (position == 0) {
                //第一个fragment界面 ， 显示界面
                return Fragment_iot_dis.Instance();
            } else {
                //第二个fragment界面 ，控制界面
                return Fragment_iot_con.Instance();
            }
        }

        @Override
        public int getCount() {
            // Show 2 total pages.
            //总共两个界面，
            return 2;
        }
    }
}
