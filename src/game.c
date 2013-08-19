/* game.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"
# include "SDL2/SDL_ttf.h"

# include "structs.h"

void keybpause (uint *keyp);
void music (uint room[],Mix_Music *bso[],uint *changeflag,int flag);
void changescreen (struct hero *jean,uint room[], int *changeflag);
void events (struct hero *jean,uint stagedata[][22][32],uint room[],uint counter[],struct enem *enemies,Mix_Chunk *fx[]);
void control (struct hero *jean,uint *keyp);
void counters (uint counter[]);
void animation (uint stagedata[][22][32],int room[],int counter[]);

void game(SDL_Window *screen,uint *state,uint *grapset) {

	/* Renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	/* Sounds */
	Mix_Music *bso[8];
	Mix_Chunk *fx[7];

	/* Init Font */
	TTF_Font *font = TTF_OpenFont("../fonts/VeniceClassic.ttf", 17);
	TTF_SetFontHinting(font, TTF_HINTING_NORMAL);

	uint stagedata[25][22][32];
	int enemydata[25][7][15];
	uint room[2] = {5,5}; /* Room, previous room */
	uint exit = 0;
	int coord[2] = {0,0}; /* Coord X and Y */
	uint changeflag = 1; /* Screen change */
	uint counter[3] = {0,0,0}; /* Counters */
	uint changetiles=*grapset;
	uint i = 0;
	float proyec[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Enemiess shoots */
	uint keyp = 0;
	uint parchment = 0;
	uint n = 0;

	/* Loading PNG */
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"../graphics/tiles.png");
	SDL_Texture *yparchment = IMG_LoadTexture(renderer,"../graphics/parchment.png");

	/* Loading musics */
	loadingmusic(bso,fx);

	/* Load data */
	loaddata(stagedata,enemydata);

	/* Init struct */
	struct enem enemies = {
		.type = {0,0,0,0,0,0,0},
		.x = {0,0,0,0,0,0,0},
		.y = {0,0,0,0,0,0,0},
		.direction = {0,0,0,0,0,0,0},
		.tilex = {0,0,0,0,0,0,0},
		.tiley = {0,0,0,0,0,0,0},
		.animation = {0,0,0,0,0,0,0},
		.limleft = {0,0,0,0,0,0,0},
		.limright = {0,0,0,0,0,0,0},
		.speed = {0,0,0,0,0,0,0},
		.fire = {0,0,0,0,0,0,0},
		.adjustx1 = {0,0,0,0,0,0,0},
		.adjustx2 = {0,0,0,0,0,0,0},
		.adjusty1 = {0,0,0,0,0,0,0},
		.adjusty2 = {0,0,0,0,0,0,0}
	};

	struct hero jean = {
		.x = 72,
		.y = 136,
		.direction = 1,
		.jump = 0,
		.height = 0,
		.animation = 0,
		.gravity = 1.9,
		.points = {0,0,0,0,0,0,0,0},
		.ground = 0,
		.collision = {0,0,0,0},
		.ducking = 0,
		.checkpoint = {5,72,136,5},
		.state = {9,0},
		.flags = {0,0,0,0,0,0,0},
		.death = 0,
		.push = {0,0,0,0},
		.temp = 0
	};

	/* Game loop */
	while (exit != 1) {

		/* Manage counters */
		counters(counter);

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		/* Animation of fire and water */
		animation(stagedata,room,counter);

		/* Draw screen */
		drawscreen(renderer,stagedata,tiles,room,counter,changeflag,fx,changetiles);

		/* Draw statusbar */
		if (room[0] != 4)
			statusbar(renderer,tiles,room,jean.state[0],jean.state[1],font,changetiles);

		/* Draw Jean */
		if (jean.flags[6] < 8)
			drawjean (renderer,tiles,&jean,counter,fx,changetiles);

		/* Enemies */
		if (enemies.type[0] > 0) {
			if (room[0] != 4)
				movenemies (&enemies,stagedata,counter,proyec,jean,fx);
			if ((room[0] == 5) || (room[0] == 6))
				crusaders (&enemies,renderer,tiles,counter,room,changetiles);
			if (room[0] == 10)
				dragon (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if (room[0] == 11)
				fireball (&enemies,renderer,tiles,counter,jean,stagedata,changetiles);
			if (room[0] == 14)
				plants (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if (room[0] == 9)
				drawrope (enemies,renderer,tiles,changetiles);
			if (room[0] == 18)
				death (&enemies,renderer,tiles,counter,proyec,stagedata,fx,changetiles);
			if ((room[0] == 24) && (enemies.type[0] == 18))
				satan (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if ((room[0] == 24) && (jean.flags[6] == 5))
				crusaders (&enemies,renderer,tiles,counter,room,changetiles);
			drawenemies (&enemies,renderer,tiles,fx,changetiles);
		}

		/* Shoots */
		if ((proyec[0] > 0) && ((room[0] == 17) || (room[0] == 20) || (room[0] == 21) || (room[0] == 22)))
		  drawshoots (proyec,tiles,renderer,&enemies,changetiles);

		/* Jean management */
		if (jean.death == 0) {
			if (jean.flags[6] < 5) {
					if (jean.temp == 0)
						control(&jean, &keyp);
					if (jean.temp == 1)
						jean.temp = 0;
					collisions (&jean,stagedata,room);
					movejean (&jean,fx);
			}
			if (room[0] != 4) {
				touchobj (&jean,stagedata,room,parchment,&changeflag,&enemies,proyec,fx);
				contact (&jean,enemies,proyec,room);
			}
			events (&jean,stagedata,room,counter,&enemies,fx);
		}

		/* Jean death */
		if (jean.death == 98) {
			if (room[0] != 4) {
				room[0] = jean.checkpoint[0];
				jean.x = jean.checkpoint[1];
				jean.y = jean.checkpoint[2];
				jean.jump = 0;
				jean.height = 0;
				jean.push[0] = 0;
				jean.push[1] = 0;
				jean.push[2] = 0;
				jean.push[3] = 0;
				changeflag = 2;
				jean.state[0] --;
				jean.death = 0;
				jean.temp = 1;
				music (room,bso,&changeflag,jean.flags[6]);
				Mix_ResumeMusic ();
			}
			else {
				jean.death = 0;
				jean.flags[6] = 8;
			}
		}
		/* Using flag 6 as counter, to make time */
		if (jean.flags[6] > 7)
			jean.flags[6] ++;
		/* Reaching 15, jump to ending sequence */
		if (jean.flags[6] == 15) {
			*state = 4;
			exit = 1;
		}
		changescreen (&jean,room,&changeflag);
		if (changeflag > 0) {
			searchenemies (room,&enemies,&changeflag,enemydata);
			music (room,bso,&changeflag,jean.flags[6]);
			for (n=0; n<24; n++) { /* Reset enemyshoots */
			  proyec[n] = 0;
			}
			counter[0] = 0;
			changeflag = 0;
		}
		/* Parchments */
		if (parchment > 0)
			showparchment (renderer,parchment,yparchment);
		if (jean.flags[6] == 3)
			redparchment (renderer,&jean);
		if (jean.flags[6] == 6)
			blueparchment (renderer,&jean);

		/* Flip ! */
		SDL_RenderPresent(renderer);

		if (parchment > 0) {
			Mix_PlayChannel(-1, fx[2], 0);
			Mix_PauseMusic ();
			/* Waiting a key */
			while (keyp == 0)
				keybpause (&keyp);
			jean.push[2] = 0;
			jean.push[3] = 0;
			keyp = 0;
			Mix_ResumeMusic ();
			parchment = 0;
		}
		if (jean.flags[6] == 4) {
			Mix_PlayChannel(-1, fx[2], 0);
			sleep(5);
			jean.flags[6] = 5;
			jean.direction = 0;
			music (room,bso,&changeflag,jean.flags[6]);
		}
		if (jean.flags[6] == 6) {
			sleep(5);
			jean.death = 0;
			changeflag = 1;
			room[0] = 4;
			jean.flags[6] = 7;
			jean.x = 125;
			jean.y = 115;
			jean.jump = 1;
		}

		if (jean.state[0] == 0) {
			Mix_HaltMusic();
			/* Mix_FreeMusic(sonido); */
			*state = 3;
			exit = 1;
		}

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyTexture(yparchment);
	SDL_DestroyRenderer(renderer);
	for (i=0;i<8;i++)
		Mix_FreeMusic(bso[i]);
	for (i=0;i<7;i++)
		Mix_FreeChunk(fx[i]);
	TTF_CloseFont(font);

}

void animation (uint stagedata[][22][32],int room[],int counter[]) {

	int i = 0;
	int j = 0;

	uint data = 0;

	for (j=0; j<=21; j++) {
		for (i=0; i<=31; i++) {

			data = stagedata[room[0]][j][i];

			/* Fire animation */
			if ((data == 59) || (data == 60)) {
				if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
					if (data == 59)
						data = 60;
					else
						data = 59;
				}
			}

			/* Water animation */
			if ((data > 500) && (data < 533)) {
				if (data < 532)
					data ++;
				else
					data = 501;
			}

			stagedata[room[0]][j][i] = data;

		}
	}
}

void counters (uint counter[]) {

	if (counter[0] < 29)
		counter[0] ++;
	else
		counter[0] = 0;

	if (counter[1] < 90)
		counter[1] ++;
	else
		counter[1] = 0;

	if (counter[2] < 8)
		counter[2] ++;
	else
		counter[2] = 0;

}

void control (struct hero *jean,uint *keyp) {

	SDL_Event event;

	if (*keyp > 0)
		*keyp = 0;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
	   	exit(0);
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP) {
				if ((jean->push[0] == 0) && (jean->jump == 0) && (jean->ducking == 0))
					jean->jump = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				if ((jean->push[1] == 0) && (jean->jump == 0)) {
					jean->push[1] = 1;
					jean->ducking = 1;
				}
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				if (jean->push[2] == 0) {
					jean->push[2] = 1;
					jean->push[3] = 0;
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				if (jean->push[3] == 0) {
					jean->push[3] = 1;
					jean->push[2] = 0;
				}
			}
			if (event.key.keysym.sym == SDLK_f)
				*keyp = 6;
			if (event.key.keysym.sym == SDLK_c)
				*keyp = 9;
	   	if (event.key.keysym.sym == SDLK_ESCAPE)
      		exit(0);
		}

		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_UP)
				jean->push[0] = 0;
			if (event.key.keysym.sym == SDLK_DOWN) {
				jean->push[1] = 0;
				jean->ducking = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
				jean->push[2] = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				jean->push[3] = 0;
		}

	}

}

void events (struct hero *jean,uint stagedata[][22][32],uint room[],uint counter[],struct enem *enemies,Mix_Chunk *fx[]) {

	int i = 0;
	int x = 0;
	int y = 0;

	if (room[0] == 4) {
		if (jean->temp < 7) {
			/* Mover a Jean */
			if (counter[1] == 45) {
				switch (jean->direction) {
					case 0: jean->direction = 1;
									break;
					case 1: jean->direction = 0;
									break;
				}
				jean->temp ++;
			}
		}
		else {
			jean->direction = 0;
			jean->death = 1;
		}
	}

	if (room[0] == 7) {
		/* Close door */
		if ((jean->x > 24) && (jean->flags[0] == 0)) {
			stagedata[7][14][0] = 347;
			stagedata[7][15][0] = 348;
			stagedata[7][16][0] = 349;
			stagedata[7][17][0] = 350;
			jean->flags[0] = 1;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}

	if (room[0] == 8) {
		/* Open ground */
		if ((jean->x > 15) && (jean->flags[1] == 1) && (stagedata[8][20][26] == 7)) {
			stagedata[8][20][26] = 38;
			stagedata[8][20][27] = 0;
			stagedata[8][21][26] = 0;
			stagedata[8][21][27] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
		/* Open door */
		if ((jean->x > 211) && (jean->flags[2] == 1) && (stagedata[8][14][31] == 343)) {
			stagedata[8][14][31] = 0;
			stagedata[8][15][31] = 0;
			stagedata[8][16][31] = 0;
			stagedata[8][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}
	if (room[0] == 10) {
		/* Dragon fire kills Jean */
		if (((jean->x > 127) && (jean->x < 144)) && (jean->y < 89)) {
			if ((enemies->speed[0] > 109) && (enemies->speed[0] < 146))
				jean->death = 1;
		}
	}
	if (room[0] == 19) {
		/* Open door */
		if ((jean->y > 16) && (jean->flags[3] == 1) && (stagedata[19][16][0] == 347)) {
			stagedata[19][16][0] = 0;
			stagedata[19][17][0] = 0;
			stagedata[19][18][0] = 0;
			stagedata[19][19][0] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}
	if (room[0] == 20) {
		/* Open door */
		if ((jean->x > 208) && (jean->flags[4] == 1) && (stagedata[20][14][31] == 343)) {
			stagedata[20][14][31] = 0;
			stagedata[20][15][31] = 0;
			stagedata[20][16][31] = 0;
			stagedata[20][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}

	if (room[0] == 24) {
		if ((jean->state[1] == 12) && (jean->x > 8) && (jean->flags[6] == 0)) {
			/* Block entry */
			stagedata[24][16][0] = 99;
			stagedata[24][17][0] = 99;
			stagedata[24][18][0] = 99;
			stagedata[24][19][0] = 99;
			jean->flags[6] = 1;
			/* Mark checkpoint */
			jean->checkpoint[0] = 24;
			jean->checkpoint[1] = jean->x;
			jean->checkpoint[2] = jean->y;
		}
		if ((jean->flags[6] == 1) && (jean->state[1] > 0) && (counter[0] == 0)) {
			/* Putting crosses */
			switch (jean->state[1]) {
				case 1: x=11;
							  y=5;
							  break;
				case 2: x=10;
							  y=5;
							  break;
				case 3: x=8;
							  y=8;
							  break;
				case 4: x=5;
							  y=8;
							  break;
				case 5: x=4;
							  y=8;
							  break;
				case 6: x=2;
							  y=12;
							  break;
				case 7: x=5;
							  y=14;
							  break;
				case 8: x=6;
							  y=14;
							  break;
				case 9: x=9;
							  y=14;
							  break;
				case 10: x=10;
								 y=14;
								 break;
				case 11: x=13;
								 y=13;
								 break;
				case 12: x=15;
								 y=16;
								 break;
			}
			stagedata[24][y][x] = 84;
			jean->state[1] --;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		if ((jean->flags[6] == 1) && (jean->state[1] == 0) && (counter[0] == 29)) {
			/* Draw cup */
			stagedata[24][3][15] = 650;
			jean->flags[6] = 2;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		/* Killed Satan, Smoke appears */
		if (enemies->type[0] == 88) {
			if (enemies->speed[0] < 90)
				enemies->speed[0] ++;
			else {
				enemies->speed[0] = 0;
				enemies->type[i] = 0;
				enemies->x[0] = 0;
				enemies->y[0] = 0;
				enemies->type[0] = 17;
				/* Putting red parchment */
				stagedata[24][14][28] = 339;
				stagedata[24][14][29] = 340;
				stagedata[24][15][28] = 341;
				stagedata[24][15][29] = 342;
			}
		}
	}
}

void music (uint room[],Mix_Music *bso[],uint *changeflag,int flag) {

	if (room[0] == 1) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[0], 0);
	}
	if ((room[0] == 2) && (room[1] == 1)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room[0] == 4) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[2], 0);
	}
	if ((room[0] == 5) && (room[1] != 6)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[7], -1);
	}
	if ((room[0] == 6) && (room[1] == 7)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[7], -1);
	}
	if ((room[0] == 7) && (room[1] == 6)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (((room[0] == 8) && (room[1] == 9)) || ((room[0] == 8) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room[0] == 9) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[3], 0);
	}
	if (((room[0] == 11) && (room[1] == 12)) || ((room[0] == 11) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if (((room[0] == 12) && (room[1] == 11)) || ((room[0] == 12) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if ((room[0] == 13) && (room[1] == 14)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (((room[0] == 14) && (room[1] == 13)) || ((room[0] == 14) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room[0] == 15) && (*changeflag == 2)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room[0] == 16) && (room[1] == 17)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room[0] == 17) && (room[1] == 16)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room[0] == 18) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[5], -1);
	}
	if (((room[0] == 19) && (room[1] == 18)) || ((room[0] == 19) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room[0] == 20) && (room[1] == 21)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room[0] == 21) && (room[1] == 20)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[6], -1);
	}
	if ((room[0] == 23) && (room[1] == 24)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[6], -1);
	}
	if ((room[0] == 24) && (flag != 5)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[5], -1);
	}
	if ((room[0] == 24) && (flag == 5))
		Mix_PlayMusic(bso[7], -1);

 	*changeflag -= 1;

}

void changescreen (struct hero *jean,uint room[], int *changeflag) {

  if ((jean->x < 1) && (room[0] != 5)) {
		room[1] = room[0];
		room[0] -= 1;
		jean->x = 240;
		*changeflag = 1;
  }
  if ((jean->x + 8) > 256) {
		room[1] = room[0];
		room[0] += 1;
		jean->x = 1;
		*changeflag = 1;
  }
	if ((jean->y + 12 < -16) && (jean->jump == 1)) {
		room[1] = room[0];
		room[0] -=5;
		jean->y = 152;
		*changeflag = 1;
	}
	if ((jean->y > 175) && (jean->jump != 1)) {
		room[1] = room[0];
		room[0] +=5;
		jean->y = -16;
		*changeflag = 1;
	}

}

void keybpause (uint *keyp) {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if ((event.key.keysym.sym == SDLK_SPACE) ||
				(event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
					*keyp = 1;
		}
	}

}