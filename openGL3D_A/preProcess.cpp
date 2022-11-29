#include "preProcess.h"

void preProcess::convert3Dpoint(readPLY& data)
{
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
    }
}

Point3D_singlepoint preProcess::getLowerBound(readPLY& data)
{
    // find the Lower value for three axis
    lowerBound.x = data.thePoint.x->at<float>(0);
    lowerBound.y = data.thePoint.y->at<float>(0);
    lowerBound.z = data.thePoint.z->at<float>(0);
    for (int i = 1; i < data.thePoint.size; i++) {
        if (data.thePoint.x->at<float>(i) < lowerBound.x)//x
            lowerBound.x = data.thePoint.x->at<float>(i);
        if (data.thePoint.y->at<float>(i) < lowerBound.y)//y
            lowerBound.y = data.thePoint.y->at<float>(i);
        if (data.thePoint.z->at<float>(i) < lowerBound.z)//z
            lowerBound.z = data.thePoint.z->at<float>(i);
    }
    return lowerBound;
}

Point3D_singlepoint preProcess::getUpperBound(readPLY& data)
{
    // find the upper value for three axis
    upperBound.x = data.thePoint.x->at<float>(0);
    upperBound.y = data.thePoint.y->at<float>(0);
    upperBound.z = data.thePoint.z->at<float>(0);
    for (int i = 1; i < data.thePoint.size; i++) {
        if (data.thePoint.x->at<float>(i) > upperBound.x)//x
            upperBound.x = data.thePoint.x->at<float>(i);
        if (data.thePoint.y->at<float>(i) > upperBound.y)//y
            upperBound.y = data.thePoint.y->at<float>(i);
        if (data.thePoint.z->at<float>(i) > upperBound.z)//z
            upperBound.z = data.thePoint.z->at<float>(i);
    }

    return upperBound;
}

Point3D_singlepoint preProcess::getCenter(readPLY& data)
{
    vector<float>x_array, y_array, z_array; // initial three vector to store the array to find the mid point of the pointcloud
    for (int i = 0; i < data.thePoint.size; i++) {
        x_array.push_back(data.thePoint.x->at<float>(i));
        y_array.push_back(data.thePoint.y->at<float>(i));
        z_array.push_back(data.thePoint.z->at<float>(i));
    }
    midPoint.x = findMid(x_array);
    midPoint.y = findMid(y_array);
    midPoint.z = findMid(z_array);

    return midPoint;
}

void preProcess::reCenter(readPLY& data)
{
    if (midPoint.x == -INFINITY && midPoint.y == -INFINITY && midPoint.z == -INFINITY) {
        std::cout << "midPoint cannot find" << endl;
    }
    for (int i = 0; i < data.thePoint.size; i++) {
        data.thePoint.x->at<float>(i) -= midPoint.x;
        data.thePoint.y->at<float>(i) -= midPoint.y;
        data.thePoint.z->at<float>(i) -= midPoint.z;
        data.thePoint.x->at<float>(i) *= 10;
        data.thePoint.y->at<float>(i) *= 10;
        data.thePoint.z->at<float>(i) *= 10;
    }
    midPoint = { 0,0,0 };
}

void preProcess::PointDownsize(readPLY& data, int density)
{
    int ratio = data.thePoint.size / density;
    if (ratio >= 1) {
        Point3D_data temp;
        for (int i = 0; i < data.thePoint.size; i++) {
            if ((i % ratio) == 0) 
                Point_downsize.push_back(Converted_3Dpoint[i]);
        }
        cout << "After downsizing, the size of the pointcloud is" << Point_downsize.size() << endl;
        cout << "The downsize ratio is: " << ratio << endl;
    }
}

float preProcess::findMid(vector<float> array) {
    float median;
    std::sort(array.begin(), array.end(), std::greater<>());
    if ((array.size() % 2) == 0)
        median = array[array.size()/2];
    else
        median = array[array.size() / 2-1];
    return median;
}

void preProcess::drawPoint(readPLY& data)
{
    
    glBegin(GL_POINTS);
   
    for (int i = 0; i < Point_downsize.size(); i++) {
        glColor3ub((int)Point_downsize[i].r, (int)Point_downsize[i].g, (int)Point_downsize[i].b);
        glPointSize(1);    
        glVertex3f(Point_downsize[i].x, Point_downsize[i].y, Point_downsize[i].z);
    }
    glEnd();
    
}
