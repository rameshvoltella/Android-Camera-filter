package com.gotye.gif;

import android.content.Context;
import android.graphics.Bitmap;

import com.gotye.bibo.util.LogUtil;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by zhuoxiuwu on 2017/2/23.
 * email nimdanoob@gmail.com
 */

public class GifDelegate {

    private Context mContext;
    private GifImageDecoder mGifImageDecoder;
    private boolean handled = false;
    private InputStream mInputStream;
    public GifDelegate(Context context, InputStream inputStream) {
        mContext = context;
        mInputStream=  inputStream;
    }

    private Thread handleThread;

    public void decoderGif() {
        handleThread = new Thread(new DecodeGifTask(mContext));
        handleThread.run();
        handled = true;
    }

    int mCurrentBitmapIndex;
    int mBitmapsSize;

    //注意Bitmap 的内存泄漏问题
    public Bitmap getNextBitmap() {
        if (mCurrentBitmapIndex <= mBitmapsSize) {
            mCurrentBitmapIndex++;
        } else {
            mCurrentBitmapIndex = 0;
        }
        LogUtil.debug("gif","picture index"+mCurrentBitmapIndex);
        return mGifImageDecoder.getFrame(mCurrentBitmapIndex);
    }

    class DecodeGifTask implements Runnable {
        Context context;

        public DecodeGifTask(Context context) {
            this.context = context;
        }

        @Override
        public void run() {
            mGifImageDecoder = new GifImageDecoder();
            try {
                mGifImageDecoder.read(mInputStream);
                mBitmapsSize = mGifImageDecoder.mFrameCount;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * 强制终止所有任务，避免内泄漏
     */
    public void shutDown() {
        if (handleThread != null && handleThread.isAlive()) {
            //shut down
        }
    }
}

