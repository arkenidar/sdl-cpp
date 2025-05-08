// use the callbacks instead of main() for SDL3
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool fullscreen;
} AppState;

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
    SDL_RenderClear(as->renderer);

    SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, 255);
    SDL_FRect rect = {50, 50, 200, 100};
    SDL_RenderFillRect(as->renderer, &rect);

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
