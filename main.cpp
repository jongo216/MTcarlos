
#include <vector>
#include <stdio.h>

#include "typedefs.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Box.h"
#include "Camera.h"
#include "Light.h"

typedef Rectangle Wall;

std::vector<Object*> objects;
std::vector<Light*> lights;

void setupScene(){
    printf("Setting up scene...\n");
    //walls
    float width  = 700.f;
    float height = 500.f;
    float depth  = 400.f;

    Wall    wTop    ( X_AXIS*width,  Z_AXIS*depth,  Pos3(     0.f,   height,      0.f), Material(LAMBERTIAN, WHITE*0.5f)),
            wBottom ( X_AXIS*width, -Z_AXIS*depth,  Pos3(     0.f,      0.f,      0.f), Material(LAMBERTIAN, WHITE*0.5f)),
            wRight  ( Z_AXIS*depth,  Y_AXIS*height, Pos3( width/2, height/2,      0.f), Material(LAMBERTIAN, GREEN)),
            wLeft   (-Z_AXIS*depth,  Y_AXIS*height, Pos3(-width/2, height/2,      0.f), Material(LAMBERTIAN, RED)),
            wBack   ( X_AXIS*width,  Y_AXIS*height, Pos3(     0.f, height/2, -depth/2), Material(LAMBERTIAN, BLUE));

    objects.push_back(&wTop);
    objects.push_back(&wBottom);
    objects.push_back(&wRight);
    objects.push_back(&wLeft);
    objects.push_back(&wBack);

    printf("Walls added...\n");


    Sphere sphere1(Pos3( 0.f, 100.f, 0.f), 100.f, Material(GLOSSY, WHITE));
    Sphere sphere2(Pos3(200.f, 100.f, 100.f), 100.f, Material(TRANSPARENT, GREEN+BLUE));

    objects.push_back(&sphere1);
    objects.push_back(&sphere2);

    Box box1(150.f, 150.f, 150.f, Pos3(-175.f, 75.f, -175.f), Material(LAMBERTIAN, GREEN+BLUE/*Color(0.5f, 0.5f, 0.5f)*/));
    objects.push_back(&box1);

    printf("Other objects added...\n");

    Light lightsource1(Pos3(0.f, height-ERROR_CORRECTION, 0.f), Color(255.f/255, 248.f/255, 239.f/255), false, width/4, depth/4);
    lights.push_back(&lightsource1);
    //make the lightsource visible
    Rectangle lightPlane = lightsource1.getArea();//(X_AXIS*(width/4), Z_AXIS*(depth/4), Pos3(0.f, height-ERROR_CORRECTION, 0.f), Material(EMISSIVE, WHITE));
    objects.push_back(&lightPlane);

    printf("Light sources added...\n");

    //camera
    Camera cam(WINDOW_WIDTH, WINDOW_HEIGHT, Pos3(0.f, height/2, 700.f), 200.f, -Z_AXIS, Y_AXIS);
    printf("Rendering started...\n");
    cam.writePPM("test", objects, lights);
}

int main(int argc, const char* argv[]){
    setupScene();
    return 0;
}
