// use the callbacks instead of main() for SDL3
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool fullscreen;
} AppState;

typedef struct
{
    SDL_FRect rectangle;
    SDL_Color color;
    bool selected;
} RectangleColored;

std::vector<RectangleColored> rectangles = {
    {{100, 100, 50, 50}, {255, 0, 0, 255}, false},
    {{150, 150, 75, 75}, {0, 255, 0, 255}, false},
    {{250, 250, 125, 125}, {0, 0, 255, 255}, false},
    {{350, 350, 175, 175}, {255, 255, 0, 255}, false},
    {{450, 450, 225, 225}, {255, 0, 255, 255}, false}};

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
    SDL_RenderClear(as->renderer);

    {
        // Draw the background, covering the whole window

        // Set the color to a dark gray ( shared color for all lines )
        SDL_SetRenderDrawColor(as->renderer, 50, 50, 50, 255);

        int width, height;
        SDL_GetWindowSize(as->window, &width, &height);
        const int gridSize = 50;

        // Draw the background-covering grid
        for (int i = 0; i <= width; i += gridSize)
        {
            // vertical lines
            SDL_RenderLine(as->renderer, i, 0, i, height);
        }
        for (int j = 0; j <= height; j += gridSize)
        {
            // horizontal lines
            SDL_RenderLine(as->renderer, 0, j, width, j);
        }
    }
    // Draw the rectangles
    for (const auto &iteratedRectangle : rectangles)
    {
        const SDL_FRect &rectangle = iteratedRectangle.rectangle;

        // rectangle fill
        const SDL_Color &currentColor = iteratedRectangle.selected ? SDL_Color{255, 255, 255, 100} : iteratedRectangle.color;
        SDL_SetRenderDrawColor(as->renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderFillRect(as->renderer, &rectangle);

        // rectangle outline
        SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
        SDL_RenderRect(as->renderer, &rectangle);
    }
    SDL_RenderPresent(as->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *as = (AppState *)appstate;

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
    {
        SDL_Log("Quit event received.");
        return SDL_APP_SUCCESS;
    }
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_Scancode scancode = event->key.scancode;
        switch (scancode)
        {
        /* Quit. */
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_Q:
            return SDL_APP_SUCCESS;
            break;
        /* Toggle fullscreen. */
        case SDL_SCANCODE_F:
        {
            as->fullscreen = !as->fullscreen;
            SDL_SetWindowFullscreen(as->window, as->fullscreen);
            break;
        }
        default:
        {
            break;
        }
        } // for switch (scancode)
    } // for SDL_EVENT_KEY_DOWN
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        SDL_Log("Mouse button %d pressed at (%g, %g)", event->button.button, event->button.x, event->button.y);
        // Check if the mouse is inside any of the rectangles
        for (auto &iteratedRectangle : rectangles)
        {
            const SDL_FRect &rectangle = iteratedRectangle.rectangle;

            SDL_FPoint point = {event->button.x, event->button.y};
            if (SDL_PointInRectFloat(&point, &rectangle))
            {
                SDL_Log("Mouse is inside rectangle at (%g, %g)", point.x, point.y);
                iteratedRectangle.selected = !iteratedRectangle.selected;
            }
        }
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        SDL_Log("Mouse button %d released at (%g, %g)", event->button.button, event->button.x, event->button.y);
        break;
    }
    case SDL_EVENT_MOUSE_MOTION:
    {
        SDL_Log("Mouse moved to (%g, %g)", event->motion.x, event->motion.y);
        break;
    }
    } // for switch (event->type)
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // allocate the appstate
    // this is a pointer to the appstate struct
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as)
    {
        return SDL_APP_FAILURE;
    }
    *appstate = as;

    // Create a window and renderer
    // SDL_WINDOW_RESIZABLE is a flag that allows the window to be resized
    if (!SDL_CreateWindowAndRenderer("SDL3 via C++ ( libsdl.org app )", 700, 500, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    // Initially set the window fullscreen to false
    as->fullscreen = false;

    SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate != NULL)
    {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}
