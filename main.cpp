
#include <stdio.h>

#include "typedefs.h"
#include "Rectangle.h"
#include "Camera.h"

typedef Rectangle Wall;

void setupScene(){
    printf("Setting up scene...\n");
    //walls
    Wall    /*wRight  (500, 500, Point(0.f, 0.f, 0.f, 1.f), -X_AXIS),
            wLeft   (500, 500, Point(0.f, 0.f, 0.f, 1.f),  X_AXIS),
            wTop    (500, 500, Point(0.f, 0.f, 0.f, 1.f), -Y_AXIS),
            wBack   (500, 500, Point(0.f, 0.f, 0.f, 1.f),  Z_AXIS),*/
            wBottom (500, 500, ORIGIN,  Y_AXIS, BLUE);
    //objects.push_back(wBottom);

    //camera
    Camera cam(WINDOW_WIDTH, WINDOW_HEIGHT, Point(0.f, 250.f, 700.f, 1.f), 200.f, -Z_AXIS, Y_AXIS);
    printf("Rendering started...\n");
    cam.writePPM("test"/*, &objects*/);
}

int main(int argc, const char* argv[]){
    setupScene();

    return 0;
}
