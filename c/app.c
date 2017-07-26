#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

#include "game/game.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

// Glew includes
#include "lib/glew-2.0.0/include/GL/glew.h"

#ifdef OS_WINDOWS
#include "lib/glew-2.0.0/src/wglew.c"
#else

#include "lib/glew-2.0.0/src/glew.c"

#endif

#include "vendor/world.h"


#define NUM_SHADERS 6
char *fragmentList[NUM_SHADERS] = {
        "resources/shaders/fog.frag",
        "resources/shaders/metaballs.frag",
        "resources/shaders/cave.frag",
        "resources/shaders/warp.frag",
        "resources/shaders/water.frag",
        "resources/shaders/fractal.frag"
};
int selection = 0;

char *vertexPath = "resources/shaders/simple.vert";
GLuint shader = 0;
unsigned char autoReload = 1; // if true, program will auto-reload the shader every second (useful for debugging)

void setup() {
    hideHelpfulOrientation();
    shader = loadShader(vertexPath, fragmentList[selection]);
}

void update() {
    if (frameNum % 60 == 0) { if (autoReload) shader = loadShader(vertexPath, fragmentList[selection]); }
    float resolution[2] = {WIDTH, HEIGHT};
    setShaderUniformVec2f(shader, "u_resolution", resolution);
    setShaderUniform1f(shader, "u_time", frameNum / 60.0);
}

void draw3D() {

}

void draw2D() {
    glUseProgram(shader);
    drawRect(0, 0, 0, WIDTH, HEIGHT);
    glUseProgram(0);
}

void keyDown(unsigned int key) {
    if (key == ' ') {
        selection = (selection + 1) % NUM_SHADERS;
        shader = loadShader(vertexPath, fragmentList[selection]);
    }
}

void keyUp(unsigned int key) {}

void mouseDown(unsigned int button) {}

void mouseUp(unsigned int button) {}

void mouseMoved(int x, int y) {}

int main(int argc, char **argv) {
    world_main(argc, argv);
}

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop


