#include <stdio.h>
#include <SDL2/sdl.h>
#include <math.h>
#include <assert.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define PLAYER_COLOR (SDL_Color) { .r=150, .g=50, .b=50, .a=255 }
#define BALL_COLOR   (SDL_Color) { .r=50, .g=150, .b=50, .a=255 }

const int player_length    = 250;
const int player_velocity  = 10;
const int player_wall_dist = 50;
float ball_velocity_x      = 0.1;
float ball_velocity_y      = 0.1;
int score1 = 0, score2 = 0;

typedef struct Entity {
  SDL_Color color;
  float     x, y;
  int       thickness;
} Entity;

typedef enum EntityType {
  ENTITY_BALL,
  ENTITY_PLAYER
}EntityType;

void RenderEntity(SDL_Renderer *renderer, Entity e, EntityType type) {
  SDL_SetRenderDrawColor(renderer, e.color.r, e.color.g, e.color.b, e.color.a);
  SDL_Rect rect = (SDL_Rect) {
    .x=e.x-(e.thickness*0.5), .y=e.y,
    .w=e.thickness,
    .h=type == ENTITY_PLAYER ? player_length : e.thickness
  };
  SDL_RenderFillRect(renderer, &rect);
}

void GameUpdate(Entity *Ball, Entity *Player1, Entity *Player2) {
  if(fabs(Ball->x - Player1->x) < 5 && Ball->y >= Player1->y && Ball->y <= Player1->y + player_length) {
    //TODO: give score to Player1
    score1++;
    ball_velocity_x = ball_velocity_x * -1;
    ball_velocity_y = ball_velocity_y * -1;
  }
  if(fabs(Ball->x - Player2->x) < 5  && Ball->y >= Player2->y && Ball->y <= Player2->y + player_length) {
    //TODO: give score to Player2
    score2++;
    ball_velocity_x = ball_velocity_x * -1;
    ball_velocity_y = ball_velocity_y * -1;
  }
  if(Ball->x >= WINDOW_WIDTH) {
    //TODO: decrease score of Player2
    score2--;
    ball_velocity_x = -1*ball_velocity_x;
  }
  if(Ball->x <= 0) {
    //TODO: decrease score of Player1
    score1--;
    ball_velocity_x = -1*ball_velocity_x;
  }
  Ball->x -= ball_velocity_x;
  if(Ball->y >= WINDOW_HEIGHT || Ball->y <= 0) ball_velocity_y = -1*ball_velocity_y;
  Ball->y -= ball_velocity_y;
}

void ShowScores(SDL_Window *parentwin) {
  char msg[128];
  sprintf(msg, "player1: %d - player2: %d, %s", score1, score2, score1 == score2 ? "tie" : score1 > score2 ? "player1 has won!" : "player2 has won!");
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Results!", msg, parentwin);
}

int main(int argc, char *argv[]) {
  Entity Player1=(Entity) {.color=PLAYER_COLOR, .x=player_wall_dist, .y=20, .thickness=15}, Player2=(Entity) {.color=PLAYER_COLOR, .x=WINDOW_WIDTH-player_wall_dist, .y=20, .thickness=15}, Ball=(Entity) {.color=BALL_COLOR, .x=WINDOW_WIDTH/2, .y=WINDOW_HEIGHT/2, .thickness=10};
  assert(player_wall_dist >= Ball.thickness);
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  SDL_Window *window = SDL_CreateWindow("ping pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  int quit = 0;
  SDL_Event event;
  while(quit == 0) {
    SDL_PollEvent(&event);
    switch(event.type) {
      case SDL_QUIT: {
        ShowScores(window);
        quit = 1;
        break;
      }
      case SDL_KEYDOWN: {
        if(event.key.keysym.sym == SDLK_DOWN) {
          Player2.y += player_velocity;
        }else if(event.key.keysym.sym == SDLK_UP) {
          Player2.y -= player_velocity;
        }else if(event.key.keysym.sym == 's') {
          Player1.y += player_velocity;
        }else if(event.key.keysym.sym == 'w') {
          Player1.y -= player_velocity;
        }else if(event.key.keysym.sym == 'r') {
          ShowScores(window);
          Player1 = (Entity) {.color=PLAYER_COLOR, .x=player_wall_dist, .y=20, .thickness=15};
          Player2 = (Entity) {.color=PLAYER_COLOR, .x=WINDOW_WIDTH-player_wall_dist, .y=20, .thickness=15};
          Ball    = (Entity) {.color=BALL_COLOR, .x=WINDOW_WIDTH/2, .y=WINDOW_HEIGHT/2, .thickness=10};
        }
        break;
      }
    }

    GameUpdate(&Ball, &Player1, &Player2);

    SDL_SetRenderDrawColor(renderer, (Uint8) 0, (Uint8)0, (Uint8)0, (Uint8)255);
    SDL_RenderClear(renderer);

    RenderEntity(renderer, Player1, ENTITY_PLAYER);
    RenderEntity(renderer, Player2, ENTITY_PLAYER);
    RenderEntity(renderer, Ball, ENTITY_BALL);

    SDL_RenderPresent(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
