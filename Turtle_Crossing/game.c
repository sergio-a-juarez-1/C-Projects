#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAX_CARS 20
#define STARTING_SPEED 3
#define SPEED_INCREMENT 2

typedef struct { float x, y; } Vector2;
typedef struct { Vector2 pos; float speed; unsigned long color; } Car;

// Game State Vars
Display *dpy;
Window win;
Vector2 player = { 300, 550 };
Car cars[MAX_CARS];
int car_count = 0;
int current_speed = STARTING_SPEED;
int level = 1;
bool game_over = false;

void spawn_car() {
    if (car_count < MAX_CARS && (rand() % 15 == 0)) {
        cars[car_count].pos.x = 620;
        cars[car_count].pos.y = 80 + (rand() % 420);
        cars[car_count].speed = current_speed;
        cars[car_count].color = ((rand()%256)<<16) | ((rand()%256)<<8) | (rand()%256);
        car_count++;
    }
}

void update_game() {
    if (game_over) return;

    spawn_car();

    for (int i = 0; i < car_count; i++) {
        cars[i].pos.x -= cars[i].speed;

        // Collision Check
        if (player.x + 15 > cars[i].pos.x && player.x - 15 < cars[i].pos.x + 50 &&
            player.y + 15 > cars[i].pos.y && player.y - 15 < cars[i].pos.y + 25) {
            game_over = true;
        }

        if (cars[i].pos.x < -60) {
            for (int j = i; j < car_count - 1; j++) cars[j] = cars[j + 1];
            car_count--;
            i--;
        }
    }

    if (player.y < 40) {
        level++;
        current_speed += SPEED_INCREMENT;
        player.x = 300;
        player.y = 550;
        car_count = 0;
    }
}

void draw_rect(float x, float y, float w, float h, unsigned long color) {
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = (color & 0xFF) / 255.0f;
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

// Helper function to draw a simple 3-point vector triangle
void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, unsigned long color) {
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = (color & 0xFF) / 255.0f;
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void reset_game() {
    player.x = 300;
    player.y = 550;
    car_count = 0;
    current_speed = STARTING_SPEED;
    level = 1;
    game_over = false;
}

void draw_frame() {
    glViewport(0, 0, 600, 600);
    glClear(GL_COLOR_BUFFER_BIT);

    // Scoreboard Panel (Crimson Red if dead, Dark Gray if alive)
    unsigned long panel_color = game_over ? 0x990000 : 0x222222;
    draw_rect(0, 0, 600, 40, panel_color);

    // Render Level Indicators (White squares inside dashboard)
    for (int i = 0; i < level; i++) {
        float block_x = 20 + (i * 25);
        if (block_x < 580) {
            draw_rect(block_x, 12, 15, 15, 0xFFFFFF);
        }
    }

    // Road Bounds
    draw_rect(0, 40, 600, 5, 0x00FF00);  // Green Finish line
    draw_rect(0, 535, 600, 5, 0x888888); // Gray Start line

    // Draw Cars
    for (int i = 0; i < car_count; i++) {
        draw_rect(cars[i].pos.x, cars[i].pos.y, 50, 25, cars[i].color);
    }

    // 🐢 DRAW VECTOR TURTLE PLAYER 🐢
    unsigned long turtle_color = game_over ? 0xFF0000 : 0x22AA22; // Deep green (Red if hit)
    unsigned long head_color   = game_over ? 0xAA0000 : 0x66FF66; // Bright lime nose

    // Front Left & Right Flippers
    draw_triangle(player.x - 12, player.y - 12, player.x - 18, player.y - 8, player.x - 10, player.y, turtle_color);
    draw_triangle(player.x + 12, player.y - 12, player.x + 18, player.y - 8, player.x + 10, player.y, turtle_color);
    
    // Back Left & Right Legs
    draw_triangle(player.x - 10, player.y + 10, player.x - 16, player.y + 14, player.x - 8, player.y + 6, turtle_color);
    draw_triangle(player.x + 10, player.y + 10, player.x + 16, player.y + 14, player.x + 8, player.y + 6, turtle_color);

    // Central Shell Body
    draw_rect(player.x - 10, player.y - 10, 20, 20, turtle_color);
    
    // Pointy Turtle Head (Facing upward)
    draw_triangle(player.x, player.y - 18, player.x - 6, player.y - 10, player.x + 6, player.y - 10, head_color);
}

int main() {
    srand(time(NULL));
    dpy = XOpenDisplay(NULL);
    if (!dpy) return printf("Cannot open display\n"), 1;

    Window root = DefaultRootWindow(dpy);
    GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    
    XSetWindowAttributes swa = { 
        .colormap = cmap, 
        .event_mask = ExposureMask | KeyPressMask | StructureNotifyMask 
    };
    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Native C Turtle Crossing");
    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 600, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    while (true) {
        while (XPending(dpy)) {
            XEvent xev; XNextEvent(dpy, &xev);
            
            if (xev.type == ConfigureNotify) {
                glViewport(0, 0, xev.xconfigure.width, xev.xconfigure.height);
            }
            
            if (xev.type == KeyPress) {
                KeySym key = XLookupKeysym(&xev.xkey, 0);
                if (key == XK_Escape) goto cleanup;
                
                if (game_over && (key == XK_r || key == XK_R)) {
                    reset_game();
                }

                if (!game_over) {
                    if (key == XK_Up)    player.y -= 15;
                    if (key == XK_Down)  player.y += 15;
                    if (key == XK_Left)  player.x -= 15;
                    if (key == XK_Right) player.x += 15;
                }
            }
        }
        update_game();
        draw_frame();
        glXSwapBuffers(dpy, win);
        usleep(16666);
    } 

cleanup:
    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}
