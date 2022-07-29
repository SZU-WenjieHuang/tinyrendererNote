#include <vector>
#include <cmath>
#include "tgaimage.h" //tga画图库
#include "model.h" //模型库，主要实现模型的读取
#include "geometry.h" //几何库，主要定义vec2与vec3类型

//定义颜色
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

//初始空模型
Model *model = NULL;

//页面宽度，高度
const int width  = 800;
const int height = 800;


//Bresenham’s 画线算法
//输入坐标1，坐标2，TGA目标指针，颜色
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;

    //要是斜率大于1，则flip一下，避免中间出现空隙
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    //确保画线是从左到右
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    //Bresenham算法画线
    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    //命令行控制方式和代码方式构造model
    //构造模型(obj文件路径)
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    //构造tga（宽高，颜色空间
    TGAImage image(width, height, TGAImage::RGB);

    //以模型的面的数量作为指标开始控制变量
    for (int i=0; i<model->nfaces(); i++) {
        //创建face数组保存一个face的三个顶点坐标
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            //顶点v0
            Vec3f v0 = model->vert(face[j]);
            //顶点v1
            Vec3f v1 = model->vert(face[(j+1)%3]);
            //首先进行屏幕空间坐标的转换
            //(-1,-1)映射到(0，0),(1,1)映射到(width，height)
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            //调用函数画线
            line(x0, y0, x1, y1, image, white);
        }
    }

    //TGA默认原点在左上角，先需要指定在左下角，故flip
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

