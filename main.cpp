
#include <vector>
#include <stdio.h>

#include "typedefs.h"
#include "Rectangle.h"
#include "Camera.h"

typedef Rectangle Wall;

std::vector<Object*> objects;

void setupScene(){
    printf("Setting up scene...\n");
    //walls
    float width = 500.f;
    float height = 500.f;

    Wall    wTop    (X_AXIS*width, -Z_AXIS*height, Pos3(   0.f, 500.f,    0.f), BLUE),
            wRight  (Z_AXIS*width,  Y_AXIS*height, Pos3( 250.f, 250.f,    0.f), GREEN),
            wLeft   (Z_AXIS*width,  Y_AXIS*height, Pos3(-250.f, 250.f,    0.f), GREEN),
            wBack   (X_AXIS*width,  Y_AXIS*height, Pos3(   0.f, 250.f, -250.f), RED),
            wBottom (X_AXIS*width, -Z_AXIS*height, Pos3(   0.f,   0.f,    0.f), BLUE);

    objects.push_back(&wTop);
    objects.push_back(&wRight);
    objects.push_back(&wLeft);
    objects.push_back(&wBack);
    objects.push_back(&wBottom);

    //camera
    Camera cam(WINDOW_WIDTH, WINDOW_HEIGHT, Pos4(0.f, 250.f, 700.f, 1.f), 200.f, -Z_AXIS, Y_AXIS);
    printf("Rendering started...\n");
    cam.writePPM("test", &objects);
}

int main(int argc, const char* argv[]){
    setupScene();
    return 0;
}
