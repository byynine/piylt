#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "An SDL3 window",
        1920,
        1200,
        SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (!SDL_SetWindowOpacity(window, 1.0f)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not set window opacity: %s\n", SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool done = false;
    SDL_Event event;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                printf("MM: x: %.0f y: %.0f\n", event.motion.x, event.motion.y);
                fflush(stdout);
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                printf("MB1_DOWN\n");
                fflush(stdout);
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                printf("MB1_UP\n");
                fflush(stdout);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
