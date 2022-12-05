#include "preProcess.h"




void preProcess::initPointcloud()
{
    lowerBound = { -INFINITY, -INFINITY,-INFINITY };
    upperBound = { -INFINITY, -INFINITY,-INFINITY };
    midPoint = { -INFINITY, -INFINITY,-INFINITY };
    Converted_3Dpoint.clear();
    Point_downsize.clear();
    PointShow.clear();
    std::cout << "init completed " << endl;
}

void preProcess::convert3Dpoint(readPLY& data)
{
    initPointcloud();
    Point3D_data temp; // set a variable to store each set of struct
    for (int i = 0; i < data.thePoint.size; i++) {
        temp.x = data.thePoint.x->at<float>(i);
        temp.y = data.thePoint.y->at<float>(i);
        temp.z = data.thePoint.z->at<float>(i);
        temp.nx = data.thePoint.nx->at<float>(i);
        temp.ny = data.thePoint.ny->at<float>(i);
        temp.nz = data.thePoint.nz->at<float>(i);
        temp.r = data.thePoint.r->at<unsigned char>(i);
        temp.g = data.thePoint.g->at<unsigned char>(i);
        temp.b = data.thePoint.b->at<unsigned char>(i);
        temp.alpha = data.thePoint.alpha->at<unsigned char>(i);
        Converted_3Dpoint.push_back(temp);
        PointShow.push_back(temp);
    }
    std::cout << "convert finish " << endl;
}

Point3D_singlepoint preProcess::getLowerBound()
{
    // find the Lower value for three axis
    lowerBound.x = Converted_3Dpoint[0].x;
    lowerBound.y = Converted_3Dpoint[0].y;
    lowerBound.z = Converted_3Dpoint[0].z;
    for (int i = 1; i < Converted_3Dpoint.size(); i++) {
        if (Converted_3Dpoint[i].x < lowerBound.x)//x
            lowerBound.x = Converted_3Dpoint[i].x;
        if (Converted_3Dpoint[i].y < lowerBound.y)//y
            lowerBound.y = Converted_3Dpoint[i].y;
        if (Converted_3Dpoint[i].z < lowerBound.z)//z
            lowerBound.z = Converted_3Dpoint[i].z;
    }
    return lowerBound;
}

Point3D_singlepoint preProcess::getUpperBound()
{
    // find the upper value for three axis
    lowerBound.x = Converted_3Dpoint[0].x;
    lowerBound.y = Converted_3Dpoint[0].y;
    lowerBound.z = Converted_3Dpoint[0].z;
    for (int i = 1; i < Converted_3Dpoint.size(); i++) {
        if (Converted_3Dpoint[i].x > upperBound.x)//x
            upperBound.x = Converted_3Dpoint[i].x;
        if (Converted_3Dpoint[i].y > upperBound.y)//y
            upperBound.y = Converted_3Dpoint[i].y;
        if (Converted_3Dpoint[i].z > upperBound.z)//z
            upperBound.z = Converted_3Dpoint[i].z;
    }

    return upperBound;
}

Point3D_singlepoint preProcess::getCenter()
{
    vector<float>x_array, y_array, z_array; // initial three vector to store the array to find the mid point of the pointcloud
    for (int i = 0; i < Converted_3Dpoint.size(); i++) {
        x_array.push_back(Converted_3Dpoint[i].x);
        y_array.push_back(Converted_3Dpoint[i].y);
        z_array.push_back(Converted_3Dpoint[i].z);
    }
    midPoint.x = findMid(x_array);
    midPoint.y = findMid(y_array);
    midPoint.z = findMid(z_array);

    return midPoint;
}

void preProcess::reCenter()
{
    if (midPoint.x == -INFINITY && midPoint.y == -INFINITY && midPoint.z == -INFINITY) {
        std::cout << "midPoint cannot find" << endl;
    }
    for (int i = 0; i < Converted_3Dpoint.size(); i++) {
        Converted_3Dpoint[i].x -= midPoint.x;
        Converted_3Dpoint[i].y -= midPoint.y;
        Converted_3Dpoint[i].z -= midPoint.z;
        Converted_3Dpoint[i].x *= 10;
        Converted_3Dpoint[i].y *= 10;
        Converted_3Dpoint[i].z *= 10;
    }
    midPoint = { 0,0,0 };
}

void preProcess::PointDownsize(int density = 0, int ratio_set = 0)
{
    int ratio = 0;
    PointShow.clear();
    if (ratio_set == 0 && density != 0)
        ratio = Converted_3Dpoint.size() / density;
    else if (density == 0 && ratio_set != 0) {
        ratio = ratio_set;
    }
    else
        cout << "Worng input" << endl;

    if (ratio >= 1) {
        Point3D_data temp;
        for (int i = 0; i < Converted_3Dpoint.size(); i++) {
            if ((i % ratio) == 0)
                PointShow.push_back(Converted_3Dpoint[i]);
        }
        cout << "After downsizing, the size of the pointcloud is: " << PointShow.size() << endl;
        cout << "The downsize ratio is: " << ratio << endl;
        Converted_3Dpoint = PointShow;
    }
}

float preProcess::findMid(vector<float> array) {
    float median;
    std::sort(array.begin(), array.end(), std::greater<>());
    if ((array.size() % 2) == 0)
        median = array[array.size() / 2];
    else
        median = array[array.size() / 2 - 1];
    return median;
}

void preProcess::drawPoint(readPLY& data)
{

    glBegin(GL_POINTS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1);
    for (int i = 0; i < PointShow.size(); i++) {
        glColor4ub((int)PointShow[i].r, (int)PointShow[i].g, (int)PointShow[i].b, (int)PointShow[i].alpha);
        glVertex3f(PointShow[i].x, PointShow[i].y, PointShow[i].z);
    }
    glEnd();

}

void preProcess::PointShow_reset() {
    PointShow.clear();
    for (int i = 0; i < Converted_3Dpoint.size(); i++) {
        PointShow.push_back(Converted_3Dpoint[i]);
    }

}

bool PointCompWithX(Point3D_data x, Point3D_data y)
{
    return x.x < y.x;
}

bool PointCompWithY(Point3D_data x, Point3D_data y)
{
    return x.y < y.y;
}

bool PointCompWithZ(Point3D_data x, Point3D_data y)
{
    return x.z < y.z;
}

void preProcess::sudoColor(string axis_str = "", bool if_one_color = FALSE, int red = 0, int green = 0, int blue = 0)
{
    if (if_one_color == TRUE) {
        std::cout << "single color mode enable" << endl;
        for (int i = 0; i < PointShow.size(); i++) {
            PointShow[i].r = red;
            PointShow[i].g = green;
            PointShow[i].b = blue;
        }
    }
    else {
        float PI = 3.14159265;
        int width = 127, center = 128;
        float frequency = 2 * PI / PointShow.size();
        std::cout << "rainbow color mode enable" << endl;
        if (axis_str == "x") {
            std::sort(PointShow.begin(), PointShow.end(), PointCompWithX);
        }
        else if (axis_str == "y") {
            std::sort(PointShow.begin(), PointShow.end(), PointCompWithY);
        }
        else if (axis_str == "z") {
            std::sort(PointShow.begin(), PointShow.end(), PointCompWithZ);
        }


        for (int i = 0; i < PointShow.size(); i++) {

            int red = (int)(sin(frequency * i + 0) * width + center);
            int green = (int)(sin(frequency * i + 2) * width + center);
            int blue = (int)(sin(frequency * i + 4) * width + center);
                  
            PointShow[i].r = red;
            PointShow[i].g = green;
            PointShow[i].b = blue;

        }
    }


}

void preProcess::getallvalue() {

    getCenter();
    reCenter();
    getLowerBound();
    getUpperBound();
    std::cout << "MidPoint: " << "x: " << midPoint.x << " y: " << midPoint.y << " z: " << midPoint.z << endl;
    std::cout << "lowerBound: " << "x: " << lowerBound.x << " y: " << lowerBound.y << " z: " << lowerBound.z << endl;
    std::cout << "upperBound: " << "x: " << upperBound.x << " y: " << upperBound.y << " z: " << upperBound.z << endl;

}