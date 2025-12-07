/* Main function for RaspberryPi Tetris */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "definitions.h"

void board_initialize(TetrisBoard* ctx);
void tetris_initialize(TetrisContext* ctx, int hold);
void block_step(TetrisContext* ctx);
void set_rect_xy_(SDL_FRect* r, short x, short y);
void move_left(TetrisContext* ctx);
void move_right(TetrisContext* ctx);
void move_down(TetrisContext* ctx);
void rotate_shape(TetrisContext* ctx);
bool detect_left_right_collision(int x);
bool detect_rotate_left_collision(int x);
bool detect_rotate_right_collision(int x);
bool detect_bottom_collision(int x, int y, TetrisBoard* b_ctx);
void lock_shape(TetrisContext* t_ctx, TetrisBoard* b_ctx);
void clear_rows(TetrisBoard* b_ctx);
void hard_drop(TetrisContext* t_ctx, TetrisBoard* b_ctx);
void hold_piece(TetrisContext* t_ctx, TetrisBoard* b_ctx);

static TTF_Font* font = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFontIO(SDL_IOFromConstMem(tiny_ttf, tiny_ttf_len), true, 60.0f);

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }

    *appstate = as;

    if (!SDL_CreateWindowAndRenderer("RaspberryPiTetris", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &as->window, &as->renderer)) {
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(as->renderer, SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    tetris_initialize(&as->tetris_ctx, 7);
    board_initialize(&as->board_ctx);

    SDL_Surface* text;

    // create the score
    text = TTF_RenderText_Solid(font, "000", 0, color);
    if (text) {
        as->texture = SDL_CreateTextureFromSurface(as->renderer, text);
        SDL_DestroySurface(text);
    }
    if (!as->texture) {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->last_step = SDL_GetTicks();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* as = (AppState*)appstate;
    const Uint64 now = SDL_GetTicks();
    TetrisContext* t_ctx = &as->tetris_ctx;
    TetrisBoard* b_ctx = &as->board_ctx;
    int x, y;

    // Each tetris piece is made of 4 rectangles
    SDL_FRect tetrisR[4];

    // Held tetris piece
    SDL_FRect tetrisHoldR[4];
    
    // Tetris board
    SDL_FRect board[200];

    if (b_ctx->gameOver == 0) {
        // constantly check if we hit the bottom
        for (int8_t i = 0; i < 4; i++) {
            x = t_ctx->shape[i * 2] + t_ctx->xpos;
            y = t_ctx->shape[i * 2 + 1] + t_ctx->ypos + 1;
            if (detect_bottom_collision(x, y, b_ctx)) {
                lock_shape(t_ctx, b_ctx);
                clear_rows(b_ctx);
                tetris_initialize(t_ctx, 7);
                if (y <= 5) {
                    b_ctx->gameOver = 1;
                }
            }
        }

        // run game logic if we're at or past the time to run it.
        // if we're _really_ behind the time to run it, run it
        // several times.
        while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
            block_step(t_ctx);
            as->last_step += STEP_RATE_IN_MILLISECONDS;
        }


        SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(as->renderer);

        // Create Tetris board
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 20; j++) {
                board[i * 10 + j].w = board[i * 10 + j].h = BLOCK_SIZE_IN_PIXELS;
                set_rect_xy_(&board[i * 10 + j], i + 7, j + 3);
                SDL_SetRenderDrawColor(as->renderer, b_ctx->grid[i][j][0], b_ctx->grid[i][j][1], b_ctx->grid[i][j][2], SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(as->renderer, &board[i * 10 + j]);
            }
        }

        // set position and render all rectangles
        SDL_SetRenderDrawColor(as->renderer, t_ctx->color[0], t_ctx->color[1], t_ctx->color[2], SDL_ALPHA_OPAQUE);
        for (int i = 0; i < 4; i++) {
            tetrisR[i].w = tetrisR[i].h = BLOCK_SIZE_IN_PIXELS;
            set_rect_xy_(&tetrisR[i], t_ctx->xpos + t_ctx->shape[2 * i], t_ctx->ypos + t_ctx->shape[2 * i + 1]);
            SDL_RenderFillRect(as->renderer, &tetrisR[i]);
        }

        // Create the tetris board grid using white lines
        SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        // Vertical lines
        for (int i = 7; i < 18; i++) {
            SDL_RenderLine(as->renderer, BLOCK_SIZE_IN_PIXELS * i, BLOCK_SIZE_IN_PIXELS * 3, BLOCK_SIZE_IN_PIXELS * i, BLOCK_SIZE_IN_PIXELS * 23);
        }

        // Horizontal lines
        for (int i = 3; i < 24; i++) {
            SDL_RenderLine(as->renderer, BLOCK_SIZE_IN_PIXELS * 7, BLOCK_SIZE_IN_PIXELS * i, BLOCK_SIZE_IN_PIXELS * 17, BLOCK_SIZE_IN_PIXELS * i);
        }

        // Draw currently held piece
        if (b_ctx->currentHold != 7) {
            SDL_SetRenderDrawColor(as->renderer, TetrisColors[b_ctx->currentHold][0], TetrisColors[b_ctx->currentHold][1], TetrisColors[b_ctx->currentHold][2], SDL_ALPHA_OPAQUE);
            for (int i = 0; i < 4; i++) {
                tetrisHoldR[i].w = tetrisHoldR[i].h = BLOCK_SIZE_IN_PIXELS;
                set_rect_xy_(&tetrisHoldR[i], SHAPES[b_ctx->currentHold][2 * i] + (GAME_HEIGHT - 3), SHAPES[b_ctx->currentHold][2 * i + 1] + 2);
                SDL_RenderFillRect(as->renderer, &tetrisHoldR[i]);
            }
        }

        SDL_FRect dst;

        // update the score
        char str[5];
        snprintf(str, sizeof(str), "%0*d", 4, b_ctx->score);
        SDL_Surface* text = TTF_RenderText_Solid(font, str, 0, color);
        if (text) {
            as->texture = SDL_CreateTextureFromSurface(as->renderer, text);
            SDL_DestroySurface(text);
        }
        if (!as->texture) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // center text
        dst.x = GAME_WIDTH / 3 * BLOCK_SIZE_IN_PIXELS;
        dst.y = BLOCK_SIZE_IN_PIXELS;
        dst.h = BLOCK_SIZE_IN_PIXELS * 2;
        dst.w = BLOCK_SIZE_IN_PIXELS * 5;

        // write the score
        SDL_RenderTexture(as->renderer, as->texture, NULL, &dst);
        SDL_DestroySurface(text);
        SDL_RenderPresent(as->renderer);
    }
    else {
        SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 0);
        SDL_RenderClear(as->renderer);

        SDL_FRect gameOver;

        // show the final score
        char gameOverStr[11] = "Game Over!";
        SDL_Surface* gameOverText = TTF_RenderText_Solid(font, gameOverStr, 0, color);
        if (gameOverText) {
            as->texture = SDL_CreateTextureFromSurface(as->renderer, gameOverText);
            SDL_DestroySurface(gameOverText);
        }
        if (!as->texture) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // center text
        gameOver.x = (GAME_WIDTH / 2 - 11) * BLOCK_SIZE_IN_PIXELS;
        gameOver.y = BLOCK_SIZE_IN_PIXELS * 3;
        gameOver.h = BLOCK_SIZE_IN_PIXELS * 8;
        gameOver.w = BLOCK_SIZE_IN_PIXELS * 22;

        // write the score
        SDL_RenderTexture(as->renderer, as->texture, NULL, &gameOver);
        SDL_DestroySurface(gameOverText);

        SDL_FRect dst;

        // show the final score
        char str[5];
        snprintf(str, sizeof(str), "%0*d", 4, b_ctx->score);
        SDL_Surface* text = TTF_RenderText_Solid(font, str, 0, color);
        if (text) {
            as->texture = SDL_CreateTextureFromSurface(as->renderer, text);
            SDL_DestroySurface(text);
        }
        if (!as->texture) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // center text
        dst.x = (GAME_WIDTH / 2 - 7) * BLOCK_SIZE_IN_PIXELS;
        dst.y = BLOCK_SIZE_IN_PIXELS * 12;
        dst.h = BLOCK_SIZE_IN_PIXELS * 6;
        dst.w = BLOCK_SIZE_IN_PIXELS * 15;

        // write the score
        SDL_RenderTexture(as->renderer, as->texture, NULL, &dst);
        SDL_DestroySurface(text);
        SDL_RenderPresent(as->renderer);
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_key_event_(TetrisContext* t_ctx, TetrisBoard* b_ctx, SDL_Scancode key_code) {
    switch (key_code) {
        /* Quit. */
        // @todo pause game
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
        /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        tetris_initialize(t_ctx, 7);
        if (b_ctx->gameOver == 1) {
            board_initialize(b_ctx);
        }
        break;
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_L:
        move_right(t_ctx);
        break;
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_I:
        rotate_shape(t_ctx);
        break;
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_J:
        move_left(t_ctx);
        break;
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_K:
        move_down(t_ctx);
        break;
    case SDL_SCANCODE_SPACE:
        hard_drop(t_ctx, b_ctx);
        break;
    case SDL_SCANCODE_LSHIFT:
        hold_piece(t_ctx, b_ctx);
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    TetrisContext* t_ctx = &((AppState*)appstate)->tetris_ctx;
    TetrisBoard* b_ctx = &((AppState*)appstate)->board_ctx;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event_(t_ctx, b_ctx, event->key.scancode);
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState* as = (AppState*)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}

void board_initialize(TetrisBoard* ctx) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 3; k++) {
                ctx->grid[i][j][k] = 40;
            }
            ctx->grid[i][j][3] = 0;
        }
    }
    ctx->score = 0;
    ctx->currentHold = 7;
    ctx->gameOver = 0;
}

void tetris_initialize(TetrisContext* ctx, int hold) {
    SDL_zeroa(ctx->color);
    SDL_zeroa(ctx->shape);
    ctx->xpos = 12;
    ctx->ypos = 4;
    if (hold == 7) {
        ctx->currentShape = rand() % 7;
    }
    else {
        ctx->currentShape = hold;
    }
    for (int i = 0; i < 8; i++) {
        ctx->shape[i] = SHAPES[ctx->currentShape][i];
    }
    for (int i = 0; i < 3; i++) {
        ctx->color[i] = TetrisColors[ctx->currentShape][i];
    }

}

void block_step(TetrisContext* ctx) {
    ++ctx->ypos;
    return;
}

void set_rect_xy_(SDL_FRect* r, short x, short y) {
    r->x = (float)(x * BLOCK_SIZE_IN_PIXELS);
    r->y = (float)(y * BLOCK_SIZE_IN_PIXELS);
}

void move_left(TetrisContext* ctx) {
    int x;
    for (int i = 0; i < 4; i++) {
        x = ctx->shape[i * 2] + ctx->xpos - 1;

        if (detect_left_right_collision(x)) {
            return;
        }
    }
    --ctx->xpos;
}

void move_right(TetrisContext* ctx) {
    int x;
    for (int i = 0; i < 4; i++) {
        x = ctx->shape[i * 2] + ctx->xpos + 1;

        if (detect_left_right_collision(x)) {
            return;
        }
    }
    ++ctx->xpos;
}

void move_down(TetrisContext* ctx) {
    ++ctx->ypos;
}

void rotate_shape(TetrisContext* ctx) {
    int temp[8];
    for (int i = 0; i < 4; i++) {
        temp[i * 2] = -ctx->shape[i * 2 + 1];
        temp[i * 2 + 1] = ctx->shape[i * 2];

        while (detect_rotate_left_collision(temp[i * 2] + ctx->xpos)) {
            ++ctx->xpos;
        }
        while (detect_rotate_right_collision(temp[i * 2] + ctx->xpos)) {
           --ctx->xpos;
        }
    };
    for (int i = 0; i < 8; i++) {
        ctx->shape[i] = temp[i];
    }
}

void hard_drop(TetrisContext* t_ctx, TetrisBoard* b_ctx) {
    int x, y;
    bool cont = true;
    while (cont) {
        ++t_ctx->ypos;
        for (int8_t i = 0; i < 4; i++) {
            x = t_ctx->shape[i * 2] + t_ctx->xpos;
            y = t_ctx->shape[i * 2 + 1] + t_ctx->ypos + 1;
            if(cont) cont = !detect_bottom_collision(x, y, b_ctx);
        }
    }
}

void hold_piece(TetrisContext* t_ctx, TetrisBoard* b_ctx) {
    if (b_ctx->currentHold == 7) {
        b_ctx->currentHold = t_ctx->currentShape;
        tetris_initialize(t_ctx, 7);
    }
    else {
        int temp = b_ctx->currentHold;
        b_ctx->currentHold = t_ctx->currentShape;
        tetris_initialize(t_ctx, temp);    
    }
}

bool detect_left_right_collision(int x) {
    if (x < 7 || x > 16) {
        return true;
    }
    return false;
}

bool detect_rotate_left_collision(int x) {
    if (x < 7) {
        return true;
    }
    return false;
}

bool detect_rotate_right_collision(int x) {
    if (x > 16) {
        return true;
    }
    return false;
}

bool detect_bottom_collision(int x, int y, TetrisBoard* b_ctx) {
    if (y > 22) {
        return true;
    }
    else if (b_ctx->grid[x - 7][y - 3][3]) {
        return true;
    }
    return false;
}

void lock_shape(TetrisContext* t_ctx, TetrisBoard* b_ctx) {
    int x, y;
    for (int i = 0; i < 4; i++) {
        x = t_ctx->shape[i * 2] + t_ctx->xpos;
        y = t_ctx->shape[i * 2 + 1] + t_ctx->ypos;
        for (int j = 0; j < 3; j++) {
            b_ctx->grid[x - 7][y - 3][j] = t_ctx->color[j];
        }
        b_ctx->grid[x - 7][y - 3][3] = 1;
    }
}

void clear_rows(TetrisBoard* b_ctx) {
    bool full_row;
    // if 4 rows cleared at a time, tetris, double points
    int rows_cleared = 0;
    for (int i = 19; i >= 0; i--) {
        full_row = true;
        for (int j = 0; j < 10; j++) {
            if (b_ctx->grid[j][i][3] == 0) {
                full_row = false;
            }
        }
        // move every piece down one row
        if (full_row) {
            rows_cleared++;
            for (int l = 18; l >= 0; l--) {
                for (int j = 0; j < 10; j++) {
                    for (int k = 0; k < 3; k++) {
                        b_ctx->grid[j][l + 1][k] = b_ctx->grid[j][l][k];
                    }
                    b_ctx->grid[j][l + 1][3] = b_ctx->grid[j][l][3];
                }
            }
            i++;
        }
    }
    if (rows_cleared == 4) {
        b_ctx->score = b_ctx->score + 800;
    }
    else {
        b_ctx->score = b_ctx->score + rows_cleared * 100;
    }
}