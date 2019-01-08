package com.example.falling.iotserver.sokcet;

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;

/**
 * Created by chenwenjie on 2019/1/4.
 */

public class DataDeal {
    public static int bytesToInt(byte[] bytes, int i) {
        int result = 0;
        result = (int) ((bytes[i] & 0xFF)
                | ((bytes[i + 1] & 0xFF) << 8)
                | ((bytes[i + 2] & 0xFF) << 16)
                | ((bytes[i + 3] & 0xFF) << 24));
        return result;
    }

    public static byte[] IntTobytes(int value) {
        byte[] src = new byte[4];
        src[3] = (byte) ((value >> 24) & 0xFF);
        src[2] = (byte) ((value >> 16) & 0xFF);
        src[1] = (byte) ((value >> 8) & 0xFF);
        src[0] = (byte) (value & 0xFF);

        return src;
    }
    public static int byteToint(byte bytes) {
        int result = 0;
        result = (int)(bytes & 0xFF);
        return result;
    }
}
