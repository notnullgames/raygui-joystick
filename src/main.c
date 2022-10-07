#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h> 

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

// Euclidean int modulo
int emod(const int a, const int b) {
    return (a < 0 ? (((a % b) + b) % b) : (a % b));
}

int currentSelection = 0;
int currentActive = -1;
int currentDown = -1;
int lastIndex = 0;
bool js_ui(int index, bool control) {
    // find the top index the user asked for
    if (index > lastIndex) {
        lastIndex = index;
    }

    // control draws before GuiSetState, so we loop around with mod
    int i = emod(index + 1, lastIndex + 1);

    if (i == currentDown) {
        GuiSetState(STATE_PRESSED);
    } else {
        if (currentSelection == i) {
            GuiSetState(STATE_FOCUSED);
        } else {
            GuiSetState(STATE_NORMAL);
        }
    }

    // button is pressed with mouse
    if (control) {
        return true;
    }

    if (currentActive == index) {
        currentActive = -1;
        return true;
    }

    // TODO: debounce this like IsKeyPressed or GuiButton (1 true per up/down)
    return control || (currentActive == index);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window title");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // I am, testing with keys, but this could be a joystick, too
        if (IsKeyPressed(KEY_DOWN)) {
            currentSelection = emod(currentSelection + 1, lastIndex + 1);
            currentActive = -1;
            currentDown = -1;
        }
        if (IsKeyPressed(KEY_UP)) {
            currentSelection = emod(currentSelection - 1, lastIndex + 1);
            currentActive = -1;
            currentDown = -1;
        }
        if (IsKeyPressed(KEY_Z)) {
            if (currentActive == currentSelection){
                currentActive = -1;
                currentDown = -1;
            } else {
                currentActive = currentSelection;
                currentDown = currentSelection;
            }
        }
        if (IsKeyReleased(KEY_Z)) {
            currentActive = -1;
            currentDown = -1;
        }

        // TraceLog(LOG_INFO, "%d/%d", currentSelection, lastIndex);

        BeginDrawing();
        
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        if (js_ui(0, GuiButton((Rectangle){ 10, 10, 100, 20 }, "Button A"))) {
            TraceLog(LOG_INFO, "A");
        }

        if (js_ui(1, GuiButton((Rectangle){ 10, 40, 100, 20 }, "Button B"))) {
            TraceLog(LOG_INFO, "B");
        }

        if (js_ui(2, GuiButton((Rectangle){ 10, 70, 100, 20 }, "Button C"))) {
            TraceLog(LOG_INFO, "C");
        }

        if (js_ui(3, GuiButton((Rectangle){ 10, 100, 100, 20 }, "Button D"))) {
            TraceLog(LOG_INFO, "D");
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
