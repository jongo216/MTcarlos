
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

    Wall    wTop    ( X_AXIS*width,  Z_AXIS*depth,  Pos3(     0.f,   height,      0.f), Material(LAMBERTIAN, BLUE)),
            wBottom ( X_AXIS*width, -Z_AXIS*depth,  Pos3(     0.f,      0.f,      0.f), Material(LAMBERTIAN, BLUE)),
            wRight  ( Z_AXIS*depth,  Y_AXIS*height, Pos3( width/2, height/2,      0.f), Material(LAMBERTIAN, GREEN)),
            wLeft   (-Z_AXIS*depth,  Y_AXIS*height, Pos3(-width/2, height/2,      0.f), Material(LAMBERTIAN, GREEN)),
            wBack   ( X_AXIS*width,  Y_AXIS*height, Pos3(     0.f, height/2, -depth/2), Material(LAMBERTIAN, RED));

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

    Box box1(150.f, 150.f, 150.f, Pos3(-200.f, 75.f, -100.f), Material(GLOSSY, Color(0.5f, 0.5f, 0.5f)));
    objects.push_back(&box1);

    printf("Other objects added...\n");

    Light lightsource1, lightsource2;
    lightsource1.position = Pos3(0.f, height-ERROR_CORRECTION, 0.f);
    lightsource2.position = Pos3(-width/2+ERROR_CORRECTION, height/2, 0.f);
    lightsource1.color = Color(255.f/255, 218.f/255, 187.f/255);
    lightsource2.color = Color(203.f/255, 219.f/255, 255.f/255);
    lights.push_back(&lightsource1);
    //lights.push_back(&lightsource2);

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
