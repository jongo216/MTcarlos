
#include <vector>
#include <stdio.h>

#include "typedefs.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Box.h"
#include "Camera.h"

typedef Rectangle Wall;

std::vector<Object*> objects;
std::vector<Light*> lights;

void setupScene(){
    printf("Setting up scene...\n");
    //walls
    float width  = 700.f;
    float height = 500.f;
    float depth  = 400.f;

    Wall    wTop    ( X_AXIS*width,  Z_AXIS*depth,  Pos3(     0.f,   height,      0.f), Material(true, BLUE)),
            wBottom ( X_AXIS*width, -Z_AXIS*depth,  Pos3(     0.f,      0.f,      0.f), Material(true, BLUE)),
            wRight  ( Z_AXIS*depth,  Y_AXIS*height, Pos3( width/2, height/2,      0.f), Material(true, GREEN)),
            wLeft   (-Z_AXIS*depth,  Y_AXIS*height, Pos3(-width/2, height/2,      0.f), Material(true, GREEN)),
            wBack   ( X_AXIS*width,  Y_AXIS*height, Pos3(     0.f, height/2, -depth/2), Material(true, RED));

    objects.push_back(&wTop);
    objects.push_back(&wBottom);
    objects.push_back(&wRight);
    objects.push_back(&wLeft);
    objects.push_back(&wBack);

    printf("Walls added...\n");


    Sphere sphere1(Pos3( 0.f, 100.f, 0.f), 100.f, Material(true, WHITE));
    Sphere sphere2(Pos3(200.f, 100.f, 100.f), 100.f, Material(true, GREEN+BLUE));

    objects.push_back(&sphere1);
    objects.push_back(&sphere2);

    Box box1(150.f, 150.f, 150.f, Pos3(-200.f, 75.f, -100.f), Material(true, Color(0.5f, 0.5f, 0.5f)));
    objects.push_back(&box1);

    printf("Other objects added...\n");

    Light lightsource;
    lightsource.position = Pos3(0.f, height-1.f, 0.f);
    lightsource.color = Color(255.f/255, 218.f/255, 187.f/255);
    lights.push_back(&lightsource);

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
