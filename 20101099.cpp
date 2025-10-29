#include <bits/stdc++.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;

// Utility for creating and saving simple RGB images
struct Image {
    int w,h;
    vector<unsigned char> data;
    Image(int w,int h):w(w),h(h),data(w*h*3,255){}
    void setPixel(int x,int y,unsigned char r,unsigned char g,unsigned char b){
        if(x>=0&&y>=0&&x<w&&y<h){
            int idx=(y*w+x)*3;
            data[idx]=r;data[idx+1]=g;data[idx+2]=b;
        }
    }
    void save(const char* fn){stbi_write_png(fn,w,h,3,data.data(),w*3);}
};

// Task 1: Bresenham Line Drawing with thickness
void drawLine(Image &img,int x1,int y1,int x2,int y2,int thickness){
    int dx=abs(x2-x1),dy=abs(y2-y1);
    int sx=(x1<x2)?1:-1;int sy=(y1<y2)?1:-1;
    int err=dx-dy;
    while(true){
        for(int i=-thickness/2;i<=thickness/2;i++){
            for(int j=-thickness/2;j<=thickness/2;j++){
                if(i*i+j*j<=thickness*thickness/4)
                    img.setPixel(x1+i,y1+j,0,0,0);
            }
        }
        if(x1==x2&&y1==y2)break;
        int e2=2*err;
        if(e2>-dy){err-=dy;x1+=sx;}
        if(e2<dx){err+=dx;y1+=sy;}
    }
}

// Task 2: Concentric Circles with gradient and thickness
void drawCircle(Image&img,int xc,int yc,int r,int thick,unsigned char R,unsigned char G,unsigned char B){
    for(int t=0;t<thick;t++){
        int rr=r+t;int x=0,y=rr;int d=3-2*rr;
        while(y>=x){
            img.setPixel(xc+x,yc+y,R,G,B);img.setPixel(xc-x,yc+y,R,G,B);
            img.setPixel(xc+x,yc-y,R,G,B);img.setPixel(xc-x,yc-y,R,G,B);
            img.setPixel(xc+y,yc+x,R,G,B);img.setPixel(xc-y,yc+x,R,G,B);
            img.setPixel(xc+y,yc-x,R,G,B);img.setPixel(xc-y,yc-x,R,G,B);
            x++;if(d>0){y--;d=d+4*(x-y)+10;}else d=d+4*x+6;
        }
    }
}

// Task 3: Liang-Barsky Line Clipping
bool clip(float &x1,float &y1,float &x2,float &y2,float xmin,float ymin,float xmax,float ymax){
    float p[4],q[4];
    p[0]=-(x2-x1);p[1]=x2-x1;p[2]=-(y2-y1);p[3]=y2-y1;
    q[0]=x1-xmin;q[1]=xmax-x1;q[2]=y1-ymin;q[3]=ymax-y1;
    float u1=0,u2=1;
    for(int i=0;i<4;i++){
        if(p[i]==0&&q[i]<0)return false;
        float r=q[i]/p[i];
        if(p[i]<0)u1=max(u1,r);
        else if(p[i]>0)u2=min(u2,r);
    }
    if(u1>u2)return false;
    x2=x1+u2*(x2-x1);y2=y1+u2*(y2-y1);
    x1=x1+u1*(x2-x1);y1=y1+u1*(y2-y1);
    return true;
}
void drawLineSimple(Image&img,int x1,int y1,int x2,int y2,unsigned char r,unsigned char g,unsigned char b){
    int dx=abs(x2-x1),dy=abs(y2-y1),sx=x1<x2?1:-1,sy=y1<y2?1:-1,err=dx-dy;
    while(true){img.setPixel(x1,y1,r,g,b);if(x1==x2&&y1==y2)break;
        int e2=2*err;if(e2>-dy){err-=dy;x1+=sx;}if(e2<dx){err+=dx;y1+=sy;}
    }
}

int main(){
    // Task 1
    Image img1(500,500);
    drawLine(img1,50,100,450,400,1);
    drawLine(img1,50,400,450,100,6);
    img1.save("task1_bresenham.png");

    // Task 2
    Image img2(500,500);
    int xc=250,yc=250;
    for(int r=20;r<=200;r+=20){
        float ratio=float(r-20)/180;
        unsigned char R=255*ratio,G=255*(1-ratio),B=128;
        drawCircle(img2,xc,yc,r,5,R,G,B);
    }
    img2.save("task2_concentric_circles.png");

    // Task 3
    Image img3(500,500);
    float xmin=100,ymin=100,xmax=400,ymax=400;
    vector<array<float,4>> lines={{50,50,450,450},{150,50,150,450},{50,250,450,250},{450,50,50,450}};
    for(auto &l:lines)drawLineSimple(img3,l[0],l[1],l[2],l[3],255,0,0);
    for(auto &l:lines){
        float x1=l[0],y1=l[1],x2=l[2],y2=l[3];
        if(clip(x1,y1,x2,y2,xmin,ymin,xmax,ymax))
            drawLineSimple(img3,round(x1),round(y1),round(x2),round(y2),0,0,255);
    }
    for(int x=xmin;x<=xmax;x++){img3.setPixel(x,ymin,0,255,0);img3.setPixel(x,ymax,0,255,0);}
    for(int y=ymin;y<=ymax;y++){img3.setPixel(xmin,y,0,255,0);img3.setPixel(xmax,y,0,255,0);}
    img3.save("task3_liang_barsky.png");

    cout << \"All tasks completed. PNGs saved.\" << endl;
    return 0;
}
