/* Author: Sergio Juarez
Purpose:  Zero-Dependency Graphical Password Generator with Clipboard & Strength Metrics (Pure X11)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

// UI Element Interaction Boundaries
typedef struct {
    int x, y, w, h;
    const char *label;
} SimpleButton;

// Original Generation Logic
void GeneratePassword(int letters_count, int symbols_count, int numbers_count, char *output_buffer, int max_len) {
    int total_length = letters_count + symbols_count + numbers_count;
    if (total_length <= 0 || total_length >= max_len) {
        strcpy(output_buffer, "Set counts > 0");
        return;
    }

    const char letters[] = "abcdefghijklmnopqrstuvwxyz";
    const char symbols[] = "!@#$%^&*";
    const char numbers[] = "0123456789";
    int index = 0;

    for (int s = 0; s < letters_count; s++) {
        int selector = rand() % 26;
        if (rand() % 2 == 1) {
            output_buffer[index++] = letters[selector];
        } else {
            output_buffer[index++] = letters[selector] - 32; 
        }
    }

    for (int s = 0; s < symbols_count; s++) {
        output_buffer[index++] = symbols[rand() % 8];
    }

    for (int nums = 0; nums < numbers_count; nums++) {
        output_buffer[index++] = numbers[rand() % 10];
    }
    output_buffer[index] = '\0';

    for (int i = total_length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = output_buffer[i];
        output_buffer[i] = output_buffer[j];
        output_buffer[j] = temp;
    }
}

// Password Strength Evaluation Engine
const char* GetPasswordStrength(int L, int S, int N, unsigned long *color) {
    int total = L + S + N;
    int types = (L > 0) + (S > 0) + (N > 0);
    
    if (total == 0) {
        *color = 0x888888; // Gray
        return "None";
    }
    if (total >= 12 && types == 3) {
        *color = 0x00E430; // Neon Green
        return "STRONG";
    }
    if (total >= 8 && types >= 2) {
        *color = 0xFFD700; // Bright Gold
        return "MEDIUM";
    }
    *color = 0xFF3333; // Electric Red
    return "WEAK";
}

int check_click(int mx, int my, int bx, int by, int bw, int bh) {
    return (mx >= bx && mx <= bx + bw && my >= by && my <= by + bh);
}
int main() {
    srand(time(NULL));

    // Connect to local X Server Display buffer
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Unable to open X display context. Run inside a desktop environment.\n");
        return 1;
    }

    int scr = DefaultScreen(dpy);
    Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 100, 100, 600, 480, 1,
                                    WhitePixel(dpy, scr), BlackPixel(dpy, scr));

    XStoreName(dpy, win, "C-Password Generator");
    
    // Listen for clipboard events alongside basic user inputs
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask | KeyPressMask);
    XMapWindow(dpy, win);

    GC gc = XCreateGC(dpy, win, 0, NULL);
    
    // Set up Clipboard Inter-Process Communication Atoms
    Atom clipboard_atom = XInternAtom(dpy, "CLIPBOARD", False);
    Atom targets_atom = XInternAtom(dpy, "TARGETS", False);
    Atom utf8_atom = XInternAtom(dpy, "UTF8_STRING", False);

    // Dynamic Tracking App States
    int letterCount = 5;
    int symbolCount = 4;
    int numberCount = 7;
    char generatedPassword[128] = "Click GENERATE to start";
    char clipboardFeedback[128] = "";

    // Immediate-mode layout elements
    SimpleButton btnLessLetters = { 280, 100, 30, 30, "-" };
    SimpleButton btnMoreLetters = { 360, 100, 30, 30, "+" };
    SimpleButton btnLessSymbols = { 280, 150, 30, 30, "-" };
    SimpleButton btnMoreSymbols = { 360, 150, 30, 30, "+" };
    SimpleButton btnLessNumbers = { 280, 200, 30, 30, "-" };
    SimpleButton btnMoreNumbers = { 360, 200, 30, 30, "+" };
    SimpleButton btnGenerate    = { 150, 260, 130, 40, "GENERATE" };
    SimpleButton btnClear       = { 310, 260, 130, 40, "CLEAR" };

    SimpleButton buttons[] = { btnLessLetters, btnMoreLetters, btnLessSymbols, btnMoreSymbols, btnLessNumbers, btnMoreNumbers, btnGenerate, btnClear };
    int num_buttons = 8;

    // Clipboard Area Dimensions
    int clipX = 40, clipY = 340, clipW = 520, clipH = 65;

    XEvent ev;
    int running = 1;

    while (running) {
        XNextEvent(dpy, &ev);

        // Repaint Window Text Elements and Canvas Structures
        if (ev.type == Expose) {
            // High-contrast OLED dark palette clear
            XSetForeground(dpy, gc, 0x121212);
            XFillRectangle(dpy, win, gc, 0, 0, 600, 480);

            // Draw Header Text
            XSetForeground(dpy, gc, 0x00E430); // Neon Green Accent
            XDrawString(dpy, win, gc, 40, 45, "PASSWORD GENERATOR", 18);
            XSetForeground(dpy, gc, 0x888888);
            XDrawString(dpy, win, gc, 40, 70, "Configure text arrays to sample output layout pools:", 52);

            // Draw Control Panel Row Data
            XSetForeground(dpy, gc, 0xFFFFFF);
            XDrawString(dpy, win, gc, 60, 120, "Letters Count:", 14);
            XDrawString(dpy, win, gc, 60, 170, "Symbols Count:", 14);
            XDrawString(dpy, win, gc, 60, 220, "Numbers Count:", 14);

            char textBuf[16];
            XSetForeground(dpy, gc, 0x00E430);
            
            // Fixed string renderings for ALL parameters
            sprintf(textBuf, "%d", letterCount); XDrawString(dpy, win, gc, 323, 120, textBuf, strlen(textBuf));
            sprintf(textBuf, "%d", symbolCount); XDrawString(dpy, win, gc, 323, 170, textBuf, strlen(textBuf));
            sprintf(textBuf, "%d", numberCount); XDrawString(dpy, win, gc, 323, 220, textBuf, strlen(textBuf));

            // Evaluate and Draw Password Strength metrics dynamically
            unsigned long strengthColor = 0x888888;
            const char* strengthStr = GetPasswordStrength(letterCount, symbolCount, numberCount, &strengthColor);
            XSetForeground(dpy, gc, 0x888888);
            XDrawString(dpy, win, gc, 60, 320, "Password Strength:", 18);
            XSetForeground(dpy, gc, strengthColor);
            XDrawString(dpy, win, gc, 190, 320, strengthStr, strlen(strengthStr));

            // Draw Interface Button Elements
            for (int i = 0; i < num_buttons; i++) {
                XSetForeground(dpy, gc, 0x282828);
                XFillRectangle(dpy, win, gc, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h);
                XSetForeground(dpy, gc, 0x555555);
                XDrawRectangle(dpy, win, gc, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h);
                
                XSetForeground(dpy, gc, (i == 6) ? 0x00E430 : 0xFFFFFF); // Green text for generate
                XDrawString(dpy, win, gc, buttons[i].x + 10, buttons[i].y + 20, buttons[i].label, strlen(buttons[i].label));
            }

            // Draw Result Key Layout Section Box
            XSetForeground(dpy, gc, 0x1C1C1C);
            XFillRectangle(dpy, win, gc, clipX, clipY, clipW, clipH);
            XSetForeground(dpy, gc, 0x3C3C3C);
            XDrawRectangle(dpy, win, gc, clipX, clipY, clipW, clipH);

            XSetForeground(dpy, gc, 0x888888);
            XDrawString(dpy, win, gc, 55, 355, "Generated Key Layout (Click box to Copy):", 41);
            XSetForeground(dpy, gc, 0xFFFFFF);
            XDrawString(dpy, win, gc, 55, 385, generatedPassword, strlen(generatedPassword));

            // Draw Clipboard Notice Action Feedback Text
            if (strlen(clipboardFeedback) > 0) {
                XSetForeground(dpy, gc, 0x00E430);
                XDrawString(dpy, win, gc, 40, 435, clipboardFeedback, strlen(clipboardFeedback));
            }
        }

        // Handle User Input Events
        if (ev.type == ButtonPress && ev.xbutton.button == Button1) {
            int mx = ev.xbutton.x;
            int my = ev.xbutton.y;

            if (check_click(mx, my, btnLessLetters.x, btnLessLetters.y, btnLessLetters.w, btnLessLetters.h) && letterCount > 0) letterCount--;
            if (check_click(mx, my, btnMoreLetters.x, btnMoreLetters.y, btnMoreLetters.w, btnMoreLetters.h) && letterCount < 30) letterCount++;
            if (check_click(mx, my, btnLessSymbols.x, btnLessSymbols.y, btnLessSymbols.w, btnLessSymbols.h) && symbolCount > 0) symbolCount--;
            if (check_click(mx, my, btnMoreSymbols.x, btnMoreSymbols.y, btnMoreSymbols.w, btnMoreSymbols.h) && symbolCount < 30) symbolCount++;
            if (check_click(mx, my, btnLessNumbers.x, btnLessNumbers.y, btnLessNumbers.w, btnLessNumbers.h) && numberCount > 0) numberCount--;
            if (check_click(mx, my, btnMoreNumbers.x, btnMoreNumbers.y, btnMoreNumbers.w, btnMoreNumbers.h) && numberCount < 30) numberCount++;

            if (check_click(mx, my, btnGenerate.x, btnGenerate.y, btnGenerate.w, btnGenerate.h)) {
                GeneratePassword(letterCount, symbolCount, numberCount, generatedPassword, sizeof(generatedPassword));
                strcpy(clipboardFeedback, ""); // Clear stale copy notices
            }
            if (check_click(mx, my, btnClear.x, btnClear.y, btnClear.w, btnClear.h)) {
                strcpy(generatedPassword, "Cleared.");
                strcpy(clipboardFeedback, "");
                letterCount = 0; symbolCount = 0; numberCount = 0;
            }

            // Fire selection mechanism if password container box is targeted
            if (check_click(mx, my, clipX, clipY, clipW, clipH) && strcmp(generatedPassword, "Click GENERATE to start") != 0 && strcmp(generatedPassword, "Cleared.") != 0) {
                XSetSelectionOwner(dpy, clipboard_atom, win, ev.xbutton.time);
                if (XGetSelectionOwner(dpy, clipboard_atom) == win) {
                    strcpy(clipboardFeedback, "Success: Password copied to OS clipboard tracker!");
                } else {
                    strcpy(clipboardFeedback, "Error: Selection registration failed.");
                }
            }

            // Dispatch sync window layout update event
            XEvent exp_ev;
            memset(&exp_ev, 0, sizeof(exp_ev));
            exp_ev.type = Expose;
            exp_ev.xexpose.window = win;
            XSendEvent(dpy, win, False, ExposureMask, &exp_ev);
        }

        // Native X11 Clipboard Handshake (Respond to foreign app copy paste requests)
        if (ev.type == SelectionRequest) {
            XSelectionRequestEvent *req = &ev.xselectionrequest;
            XEvent respond;
            memset(&respond, 0, sizeof(respond));
            
            respond.xselection.type = SelectionNotify;
            respond.xselection.display = req->display;
            respond.xselection.requestor = req->requestor;
            respond.xselection.selection = req->selection;
            respond.xselection.target = req->target;
            respond.xselection.time = req->time;
            respond.xselection.property = None;

            if (req->target == targets_atom) {
                Atom supported[] = { targets_atom, utf8_atom, XA_STRING };
                XChangeProperty(dpy, req->requestor, req->property, XA_ATOM, 32, 
                                PropModeReplace, (unsigned char*)supported, 3);
                respond.xselection.property = req->property;
            } else if (req->target == utf8_atom || req->target == XA_STRING) {
                XChangeProperty(dpy, req->requestor, req->property, req->target, 8, 
                                PropModeReplace, (unsigned char*)generatedPassword, strlen(generatedPassword));
                respond.xselection.property = req->property;
            }

            XSendEvent(dpy, req->requestor, True, 0, &respond);
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
