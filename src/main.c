#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int window_size[2] = { 1920, 1200 };

struct winsize terminal_window;

int main(int argc, char* argv[])
{
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_window) == -1)
    {
        perror("ioctl");
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "piylt",
        window_size[0],
        window_size[1],
        SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );

    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (!SDL_SetWindowOpacity(window, 1.0f))
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not set window opacity: %s\n", SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    float xf;
    float yf;
    
    bool mb_down = false;

    bool run = true;
    SDL_Event event;

    while (run)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) run = false;
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                xf = event.motion.x / window_size[0];
                yf = event.motion.y / window_size[1];
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) mb_down = true;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) mb_down = false;
        }

        if (mb_down)
        {
            for (int i = 0; i < terminal_window.ws_row * yf; i++)
            {
                for (int j = 0; j < terminal_window.ws_col * xf; j++) putchar(' ');
                if ((i + 1) >= terminal_window.ws_row * yf) putchar('.');
                putchar('\n');
            };

            printf("\033[2J\033[H");
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
