package com.gotye.bibo.filter;

import android.content.Context;
import android.graphics.PointF;
import android.opengl.GLES10;

import com.gotye.bibo.R;
import com.gotye.bibo.gles.GlUtil;


/**
 * Created by Michael.Ma on 2016/8/26.
 */
public class ImageFilterPinchDistortion extends CameraFilterPinchDistortion {
    public ImageFilterPinchDistortion(Context applicationContext) {
        super(applicationContext);
    }

    public ImageFilterPinchDistortion(Context applicationContext,
                                       float radius, float scale, PointF center) {
        super(applicationContext, radius, scale, center);
    }

    @Override
    public int getTextureTarget() {
        return GLES10.GL_TEXTURE_2D;
    }

    @Override
    protected int createProgram(Context applicationContext) {
        return GlUtil.createProgram(applicationContext, R.raw.vertex_shader,
                R.raw.fragment_shader_2d_pinch_distortion);
    }
}
