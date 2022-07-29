#include <vector>
#include <cmath>
#include "tgaimage.h" //tga��ͼ��
#include "model.h" //ģ�Ϳ⣬��Ҫʵ��ģ�͵Ķ�ȡ
#include "geometry.h" //���ο⣬��Ҫ����vec2��vec3����

//������ɫ
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

//��ʼ��ģ��
Model *model = NULL;

//ҳ���ȣ��߶�
const int width  = 800;
const int height = 800;


//Bresenham��s �����㷨
//��������1������2��TGAĿ��ָ�룬��ɫ
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;

    //Ҫ��б�ʴ���1����flipһ�£������м���ֿ�϶
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    //ȷ�������Ǵ�����
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    //Bresenham�㷨����
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
    //�����п��Ʒ�ʽ�ʹ��뷽ʽ����model
    //����ģ��(obj�ļ�·��)
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    //����tga����ߣ���ɫ�ռ�
    TGAImage image(width, height, TGAImage::RGB);

    //��ģ�͵����������Ϊָ�꿪ʼ���Ʊ���
    for (int i=0; i<model->nfaces(); i++) {
        //����face���鱣��һ��face��������������
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            //����v0
            Vec3f v0 = model->vert(face[j]);
            //����v1
            Vec3f v1 = model->vert(face[(j+1)%3]);
            //���Ƚ�����Ļ�ռ������ת��
            //(-1,-1)ӳ�䵽(0��0),(1,1)ӳ�䵽(width��height)
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            //���ú�������
            line(x0, y0, x1, y1, image, white);
        }
    }

    //TGAĬ��ԭ�������Ͻǣ�����Ҫָ�������½ǣ���flip
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

