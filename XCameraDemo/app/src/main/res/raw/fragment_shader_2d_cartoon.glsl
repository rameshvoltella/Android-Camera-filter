precision mediump float; //指定默认精度

varying vec2 vTextureCoord;
uniform sampler2D uTexture;

uniform float u_texWidth;
uniform float u_texHeight;
uniform float touch_x_offset;

const float edge_thres = 0.2;
const float edge_thres2 = 5.0;

//将RGB转为HSV
vec3 RGBtoHSV(float r, float g, float b)
{
    float minv, maxv, delta;
    vec3 res;

    minv = min(min(r, g), b);
    maxv = max(max(r, g), b);
    res.z = maxv;           // v

    delta = maxv - minv;

    if( maxv !=0.0 )
       res.y = delta /maxv;     // s
    else {
       // r = g = b =0     // s = 0, v is undefined
       res.y = 0.0;
       res.x = -1.0;
       return res;
    }

    if( r ==maxv )
       res.x = ( g - b ) /delta;     // between yellow & magenta
    else if( g== maxv )
       res.x = 2.0 + ( b - r ) /delta;   // between cyan &yellow
    else
       res.x = 4.0 + ( r - g ) /delta;   // between magenta &cyan

    res.x = res.x *60.0;           // degrees
    if( res.x< 0.0 )
       res.x = res.x + 360.0;

    return res;
}

//HSV转RGB
vec3 HSVtoRGB(float h, float s, float v)
{
    int i;
    float f, p,q, t;
    vec3 res;

    if( s == 0.0)
    {
       // achromatic (grey)
       res.x = v;
       res.y = v;
       res.z = v;
       return res;
    }

    h /= 60.0;        // sector 0 to 5
    i = int(floor( h ));
    f = h -float(i);        // factorial part of h
    p = v * (1.0 - s );
    q = v * (1.0 - s * f );
    t = v * (1.0 - s * ( 1.0 - f ) );

    if (i == 0){
       res.x = v;
       res.y = t;
       res.z = p;
    }
    else if (i ==1) {
       res.x = q;
       res.y = v;
       res.z = p;
    }
    else if (i ==2) {
       res.x = p;
       res.y = v;
       res.z = t;
    }
    else if (i ==3) {
       res.x = p;
       res.y = q;
       res.z = v;
    }
    else if (i ==4) {
       res.x = t;
       res.y = p;
       res.z = v;
    }
    else {
       res.x = v;
       res.y = p;
       res.z = q;
    }

    return res;
}

//根据参数值所在区间返回H值
float nearestHueLevel(float col)
{
    int i = int(ceil(col / 60.0));
    return 60.0 * float(i);
}

float nearestSatLevel(float col)
{
    int i = int(ceil(col * 7.0));
    return float(i) / 7.0;
}

float nearestValLevel(float col)
{
    int i = int(ceil(col * 4.0));
    return float(i) / 4.0;
}

// averaged pixel intensity from 3 color channels
float avg_intensity(vec4 pix)
{
    return (pix.r + pix.g + pix.b) / 3.0;
}

vec4 get_pixel(vec2 coords, float dx, float dy)
{
    return texture2D(uTexture, coords + vec2(dx, dy));
}

//边缘检测（参照链接）
float IsEdge(in vec2 coords)
{
    float dxtex= 1.0 / u_texWidth;
    float dytex= 1.0 / u_texHeight;
    float pix[9];
    float delta;

    // readneighboring pixel intensities
    pix[0] =avg_intensity(get_pixel(coords, -dxtex, -dytex));
    pix[1] =avg_intensity(get_pixel(coords, -dxtex, 0.));
    pix[2] =avg_intensity(get_pixel(coords, -dxtex, dytex));
    pix[3] =avg_intensity(get_pixel(coords, 0.,   -dytex));
    pix[4] =avg_intensity(get_pixel(coords, 0.,    0.));
    pix[5] =avg_intensity(get_pixel(coords, 0.,    dytex));
    pix[6] =avg_intensity(get_pixel(coords,  dxtex,-dytex));
    pix[7] =avg_intensity(get_pixel(coords, dxtex,  0.));
    pix[8] =avg_intensity(get_pixel(coords, dxtex,  dytex));

    // averagecolor differences around neighboring pixels
    delta =(abs(pix[1]-pix[7])+
            abs(pix[5]-pix[3])+
            abs(pix[0]-pix[8])+
            abs(pix[2]-pix[6]))*0.25;

    //returnclamp(5.0*delta,0.0,1.0);
    return clamp(edge_thres2 * delta,0.0,1.0);
}

void main()
{
    vec2 uv =vTextureCoord.xy;
    vec4 tc =vec4(1.0, 0.0, 0.0, 1.0);

    if (uv.x> (touch_x_offset+0.002)) {
        vec3 colorOrg = texture2D(uTexture, uv).rgb;
        //将RGB转化到HSV颜色空间
        vec3 vHSV = RGBtoHSV(colorOrg.r,colorOrg.g,colorOrg.b);
        //将HSV值变换到预先定义的区间值中
        vHSV.x = nearestHueLevel(vHSV.x);
        vHSV.y = nearestSatLevel(vHSV.y);
        vHSV.z = nearestValLevel(vHSV.z);

        //边缘检测
        float edg = IsEdge(uv);
        //边框为黑色，否则将HSV转回RGB
        vec3 vRGB = (edg >= edge_thres) ? vec3(0.0, 0.0, 0.0) :
            HSVtoRGB(vHSV.x,vHSV.y,vHSV.z);

        tc = vec4(vRGB.x,vRGB.y,vRGB.z, 1);
    }
    else if(uv.x < (touch_x_offset-0.002)) {
        tc = texture2D(uTexture, uv);
    }

    gl_FragColor= tc;// * v_fragmentColor;
}