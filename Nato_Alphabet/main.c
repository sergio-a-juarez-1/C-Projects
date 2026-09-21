#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODE_LEN 50
#define BUFFER_SIZE 64
#define OUTPUT_SIZE 1024

// Immersive Dark Theme Palette
#define COLOR_BACKGROUND  GetColor(0x121214FF) // Deep OLED Slate
#define COLOR_CARD        GetColor(0x1D1D21FF) // Lighter dark container panel
#define COLOR_TEXT_MAIN   GetColor(0xF5F5F7FF) // Crisp paper white
#define COLOR_TEXT_MUTED  GetColor(0x7E7E86FF) // Muted graphite gray
#define COLOR_ACCENT      GetColor(0x0A84FFFF) // Electric Neon Blue (Highly readable)
#define COLOR_BORDER      GetColor(0x2C2C2EFF) // Subtle boundary panel division

// Custom pure C word wrap function to measure space metrics cleanly
void DrawTextWrapped(const char *text, int posX, int posY, int maxWidth, int fontSize, Color color) {
    char word[MAX_CODE_LEN + 10];
    int wordIdx = 0;
    int curX = posX;
    int curY = posY;
    int spaceWidth = MeasureText(" ", fontSize);

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ' && wordIdx < (int)sizeof(word) - 1) {
            word[wordIdx++] = text[i];
        } 
        if (text[i] == ' ' || text[i+1] == '\0') {
            word[wordIdx] = '\0';
            if (wordIdx > 0) {
                int wordWidth = MeasureText(word, fontSize);
                
                if (curX + wordWidth > posX + maxWidth) {
                    curX = posX;
                    curY += fontSize + 6; 
                }
                
                DrawText(word, curX, curY, fontSize, color);
                curX += wordWidth;
                wordIdx = 0;
            }
            if (text[i] == ' ') {
                curX += spaceWidth;
            }
        }
    }
}

int main() {
    // --- PART 1: Read CSV and populate dictionary ---
    // FIXED: Formatted safely into a 2D string matrix array [26 entries][50 chars each]
    char nato_dict[26][MAX_CODE_LEN] = {{0}}; 
    
    FILE *file = fopen("alphabet.csv", "r");
    if (file == NULL) {
        printf("Error: Could not open alphabet.csv\n");
        return 1;
    }

    // FIXED: Allocated a proper array line buffer size 
    char line[100]; 
    if (fgets(line, sizeof(line), file) == NULL) {
        // Handle empty file edge cases safely
    }

    while (fgets(line, sizeof(line), file)) {
        char letter;
        char code[MAX_CODE_LEN];
        if (sscanf(line, "%c,%s", &letter, code) == 2) {
            if (letter >= 'A' && letter <= 'Z') {
                strcpy(nato_dict[letter - 'A'], code);
            }
        }
    }
    fclose(file);

    // --- PART 2: Initialize GUI Window ---
    InitWindow(600, 360, "NATO Phonetic Alphabet Converter");
    SetTargetFPS(60);

    char input_text[BUFFER_SIZE] = {0};
    char output_text[OUTPUT_SIZE] = "Type a word to begin live phonetic translation...";
    bool edit_mode = true; 
    int letter_count = 0;

    while (!WindowShouldClose()) {
        bool input_changed = false;

        // --- LIVE KEYBOARD INPUT SAMPLING ---
        if (edit_mode) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letter_count < BUFFER_SIZE - 1)) {
                    input_text[letter_count] = (char)key;
                    letter_count++;
                    input_text[letter_count] = '\0';
                    input_changed = true; 
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (letter_count > 0) {
                    letter_count--;
                    input_text[letter_count] = '\0';
                    input_changed = true; 
                }
            }
        }

        // --- LIVE TRANSLATION CORE LOGIC ---
        if (input_changed && letter_count > 0) {
            memset(output_text, 0, sizeof(output_text)); 
            int first = 1;

            for (int i = 0; input_text[i] != '\0'; i++) {
                if (input_text[i] == '\n') continue;

                if (!first) {
                    strcat(output_text, " ");
                }
                first = 0;

                if (input_text[i] == ' ') {
                    strcat(output_text, "[SPACE]");
                    continue;
                }

                char upper = toupper((unsigned char)input_text[i]);
                if (upper >= 'A' && upper <= 'Z') {
                    int idx = upper - 'A';
                    if (strlen(nato_dict[idx]) > 0) {
                        strcat(output_text, nato_dict[idx]);
                    }
                }
            }
        } else if (letter_count == 0) {
            strcpy(output_text, "Type a word to begin live phonetic translation...");
        }

        // --- DRAWING THE INTERFACE ---
        BeginDrawing();
        ClearBackground(COLOR_BACKGROUND);

        DrawText("Enter a word or phrase:", 30, 30, 20, COLOR_TEXT_MAIN);

        GuiTextBox((Rectangle){ 30, 70, 420, 40 }, input_text, BUFFER_SIZE, edit_mode);

        if (GuiButton((Rectangle){ 465, 70, 105, 40 }, "CLEAR")) {
            memset(input_text, 0, sizeof(input_text));
            letter_count = 0;
            strcpy(output_text, "Type a word to begin live phonetic translation...");
        }

        // --- RENDER TEXT OUTPUT BLOCK PANEL ---
        DrawText("Result:", 30, 145, 18, COLOR_TEXT_MUTED);
        
        DrawRectangleRec((Rectangle){ 30, 175, 540, 150 }, COLOR_CARD);
        DrawRectangleLinesEx((Rectangle){ 30, 175, 540, 150 }, 1.0f, COLOR_BORDER);

        DrawTextWrapped(
            output_text, 
            45,                     
            195,                    
            510,                    
            22, 
            COLOR_ACCENT            
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
