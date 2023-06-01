/* Opossum Massage Simulator GBA eReader source code */

#include <stdint.h>
#include <stddef.h>

#include "gbalib.h"

#include "title.h"
#include "game.h"
#include "oldman.h"
#include "oldman2.h"
#include "bar.h"
#include "white.h"

uint8_t quit;
uint32_t key;
uint8_t game_state;
uint8_t story_text;
uint8_t start_game;
int8_t hiscore[5];
int8_t score[5];
int16_t carre_tension;
uint16_t wait;

void switch_game(uint8_t m);

const char story_text_stored[3][4][25] =
{
	{
		{"2024 AD"},
		{"TRUMP IS NOW PRESIDENT."},
		{"HE WILL BAN ALIENS FROM"},
		{"MURICA."},
	},
	{
		{"THIS OFFENDED ALIENS"},
		{"AFTER MUCH DEBATE,"},
		{"THE ALIEN ASSEMBLY"},
		{"DECIDED TO INVADE EARTH"},
	},
	{
		{"OPOSSUMS ARE OUR ONLY"},
		{"HOPE LEFT AGAINST ALIENS"},
		{"PLEASE CHIN,SAVE OUR GOD"},
		{"FOR GREAT JUSTICE!"},
	}
};

void DrawStoryText()
{
	Draw_Text(story_text_stored[story_text][0], 0, 0, 15, 0);
	Draw_Text(story_text_stored[story_text][1], 0, 16, 15, 0);
	Draw_Text(story_text_stored[story_text][2], 0, 32, 15, 0);
	Draw_Text(story_text_stored[story_text][3], 0, 48, 15, 0);	
}


void gameplay();

int main(void) 
{
	quit = 0;
	
	Init_GBA(DCNT_MODE4);

	hiscore[0] = 48;
	hiscore[1] = 48;
	hiscore[2] = 48;
	hiscore[3] = 48;
	hiscore[4] = '\0';

	switch_game(0);
	
	for(;;)
	{
		updateButtons();
		wait++;
		
		switch(game_state)
		{
			default:
				if (wasButtonPressed(BUTTON_A))
				{
					switch_game(1);
				}

#ifdef EREADER
				if (isButtonPressed(BUTTON_B) )
				{
					SystemCall(0);
				}
#endif
			break;
			case 1:
				if (wasButtonPressed(BUTTON_A))
				{
					story_text++;
					if (story_text > 2)
					{
						switch_game(2);
					}
					else
					{
						memset16(fb, 0x0000, 240*160 / 2);
						DrawStoryText();
					}
				}
			break;
			case 2:
				gameplay();
				vid_vsync();
			break;
			case 3:
				
				if (wasButtonPressed(BUTTON_A) && wait > 20)
				{
					switch_game(0);
				}
				vid_vsync();
				vid_vsync();
				vid_vsync();
			break;
		}
		
		vid_vsync();
	}

	return 2; // Exit to menu
}

static inline short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}

#define add_score(add) \
	score[3] += add; \
	if (score[3] > 57) { \
		score[2]+= 1; \
		score[3] = 48; } \
	if (score[2] > 57) { \
		score[1]+= 1; \
		score[2] = 48; } \
	if (score[1] > 57) { \
		score[0]+= 1; \
		score[1] = 48; } \


void gameplay()
{
	static unsigned char animation_oldman = 0;
	static unsigned char time_oldman = 0;
	unsigned char animation_oldman_frames[3] = {0, 1, 126};
	static unsigned char carre_time = 0;
	static unsigned char carre_reserve = 0;
	static unsigned char carre_time_reserve = 0;
	static unsigned char start_time = 0;
	static unsigned char possumstate = 0;
	unsigned char temp_random;

	
	switch (start_game)
	{
		default:
			Draw_Text_Center("READY?", 32, 1, 0);
			start_time++;
			if (start_time > 60)
			{
				start_game = 1;
				start_time = 0;
				LZ77UnCompVram(gameBitmap, fb);
				// DMAFastCopy((void*)gameBitmap, (void*)fb, sizeof(gameBitmap)/4, DMA_32NOW);
				drawImage(oldmanBitmap, 64, 64, 88, 79);
				drawImage(barBitmap, 120, 16, 60, 0);
				possumstate = 0;
			}

		break;	
		case 1:
			add_score(1);
			
			carre_time++;
			carre_time_reserve++;
			time_oldman++;
			
			possumstate = 0;
			

			if (score[0] > 1)
			{
				possumstate = 3;
				if (score[0] > 2)
				{
					possumstate = 4;
				}
			}
			if (score[1] > 5)
			{
				possumstate = 1;
			}
			
			if (carre_tension < 1 || carre_tension > 103)
			{
				switch_game(3);
			}
			
			if (carre_time_reserve > 0)
			{
				while (carre_reserve > 0) 
				{
					temp_random = rand_a_b(1, 3);
					//temp_random = 1;
					carre_reserve = carre_reserve - 1;
					carre_tension = (carre_tension + temp_random + possumstate);
				}
				carre_time_reserve = 0;
			}
			
			if (carre_time > 0)
			{
				temp_random = 2;
				carre_tension = carre_tension - (temp_random + possumstate);
				carre_time = 0;
			}
			
			if (animation_oldman % 2 == 0) {
				drawImage(oldmanBitmap, 64, 64, 88, 79);
			}
			else
			{
				drawImage(oldman2Bitmap, 64, 51, 89, 79);
			}
			
			drawImage(barBitmap, 120, 16, 60, 0);
			drawImage(whiteBitmap, 8, 8, 60 + 12 + carre_tension, 4);
			
			Draw_Text(score, 0, 0, 1, 0);
			
			if (isButtonPressed(BUTTON_A) )
			{
				if (time_oldman > 2)
				{
					animation_oldman++;
					if (animation_oldman_frames[animation_oldman] == 126) animation_oldman = 0;
					time_oldman = 0;
				}
				carre_reserve = carre_reserve + 3;
			}
		break;
	}

}



void switch_game(uint8_t m)
{
	game_state = m;
	story_text = 0;
	start_game = 0;
	carre_tension = 52;
	
	switch(game_state)
	{
		case 0:
		
			DMAFastCopy((void*)titlePal, (void*)MEM_BG_PALETTE, sizeof(titlePal)/4, DMA_32NOW);
			memset16(fb, 0x0101, 240*160 / 2);
			#ifdef CARTRIDGE
			LZ77UnCompVram(titleBitmap, fb);
			#else
			//DMAFastCopy((void*)titleBitmap, (void*)fb, sizeof(titleBitmap)/4, DMA_32NOW);
			
			#if 1
			memset16(fb, 0x0202, (240 * 4) / 2);
			memset16(fb + ((240 * 4) / 2), 0x0F0F, (240 * 80) / 2);
			memset16(fb + ((240 * 4) / 2) + ((240 * 80) / 2) , 0x0202, (240 * 4) / 2);
			
			Draw_Text_Center("OPOSSUM MASSAGE", 32, 13, 15);
			Draw_Text_Center("SIMULATOR", 48, 1, 15);
			#endif
			
			#endif
			Draw_Text_Center("PUSH START TO PLAY", 120, 15, 0);
			
			Draw_Text("HISCORE ", 0, 152, 15, 0);
			Draw_Text(hiscore, 64, 152, 15, 0);
		break;
		case 1:
			DMAFastCopy((void*)titlePal, (void*)MEM_BG_PALETTE, sizeof(titlePal)/4, DMA_32NOW);
			memset16(fb, 0x0000, 240*160 / 2);
			DrawStoryText();
		break;
		case 2:
			DMAFastCopy((void*)gamePal, (void*)MEM_BG_PALETTE, sizeof(gamePal)/4, DMA_32NOW);
			memset16(fb, 0x0101, 240*160 / 2);
			
			LZ77UnCompVram(gameBitmap, fb);
			//DMAFastCopy((void*)gameBitmap, (void*)fb, sizeof(gameBitmap)/4, DMA_32NOW);
			
			score[0] = 48;
			score[1] = 48;
			score[2] = 48;
			score[3] = 48;
			score[4] = '\0';
			
			drawImage(oldmanBitmap, 64, 64, 88, 79);
			drawImage(barBitmap, 120, 16, 60, 0);
			drawImage(whiteBitmap, 8, 8, 60 + 12 + carre_tension, 4);
		break;
		case 3:
			DMAFastCopy((void*)gamePal, (void*)MEM_BG_PALETTE, sizeof(gamePal)/4, DMA_32NOW);
			memset16(fb, 0x0000, 240*160 / 2);
			drawImage(oldmanBitmap, 64, 64, 88, 79);
			Draw_Text_Center("GAME OVER", 32, 1, 0);
			Draw_Text_Center("SCORE IS", 48, 1, 0);
			Draw_Text_Center(score, 56, 1, 0);
			
			//Canonical approach, thanks calcmaniac
			int8_t diff = score[0] - hiscore[0];
			if (diff == 0)
				diff = score[1] - hiscore[1];
			if (diff == 0)
				diff = score[2] - hiscore[2];
			if (diff == 0)
				diff = score[3] - hiscore[3];
			if (diff == 0)
				diff = score[4] - hiscore[4];
				
			if (diff > 0)
			{
				hiscore[0] = score[0];
				hiscore[1] = score[1];
				hiscore[2] = score[2];
				hiscore[3] = score[3];
				hiscore[4] = score[4];
				hiscore[5] = '\0';
			}
		break;
	}
}
