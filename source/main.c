#include <string.h>
#include <stdio.h>
#include <switch.h>



//Functions Prototypes
void confirmButton(); 
void intTextbox(int c);
void setFile();
void setItem();



int newValues[6] = {0,0,0,0,0,0};
int arrowValues[6][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};


char arrowName[6][13] = {"Arrow", "Fire Arrow", "Ice Arrow", "Shock Arrow", "Bomb Arrow", "Ancient Arrow"};


int selector = 0;
int isSlotSelected = 0;
int currentItem = 0;
int slot = 0;

long int rupees;
long int arrowCount[6] = {0,0,0,0,0,0};

int rupID = 0x00eaf8;
int arrowsID = 0x071258;


long int new_rupees;
long int newArrows;


FILE *fp;


int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(NULL);

   	printf("Legend of Zelda BOTW Save Editor WIP \n\n");
   	printf("Use L/R to select an Item, press A to commit Changes\n");
	printf("Enter your save slot (Use DPad UP/Down to Increase/Decrease) \n");
	printf("0");
	
    while(appletMainLoop())
    {

        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);


        if (kDown & KEY_DUP) intTextbox(0);
        if (kDown & KEY_DDOWN) intTextbox(1);
        if (kDown & KEY_DRIGHT) intTextbox(2);
        if (kDown & KEY_DLEFT) intTextbox(3);
        if (kDown & KEY_A) confirmButton();

        if(kDown & KEY_L){
        	if(currentItem != 0){
        		currentItem--;
        		setItem();
        	}
        	
        }

        if(kDown & KEY_R){
        	if(currentItem != 0){
        		currentItem++;
        		setItem();
        	}
        	
        }

        if (kDown & KEY_PLUS){
        	fclose(fp);
        	break;
        } 

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}


void intTextbox(int c){

	switch(currentItem){
		case 0:
			printf("\r");
			printf("%d", slot);
			switch(c){
				case 0:
					slot++;
					if(slot == 6)
						slot = 0;
					break;
				case 1:
					slot--;
					if(slot == -1)
						slot = 5;
					break;	
				
			}
			printf("\r");
			printf("%d", slot);

			break;
		case 1:

			printf("\r");
			int x = 0;
			for(x = 0; x < 6; x++){
				printf("%d", newValues[x]);
			}	
			
			switch(c){
				case 0:
					newValues[selector]++;
					if(newValues[selector] == 10)
						newValues[selector] = 0;
					break;
				case 1:
					newValues[selector]--;
					if(newValues[selector] == -1)
						newValues[selector] = 9;
					break;	
				case 2:
					selector++;
					if(selector == 6)
						selector = 0;
					break;
				case 3:
					selector--;
					if(selector == -1)
						selector = 5;
					break;
		
			}
			printf("\r");
			for(x = 0; x < 6; x++){
				printf("%d", newValues[x]);
			}	

			break;
	}


	if(currentItem > 1)
	{
		printf("\r");
		int x = 0;
		for(x = 0; x < 3; x++){
			printf("%d", arrowValues[currentItem-2][x]);
		}	
		
		switch(c){
			case 0:
				arrowValues[currentItem-2][selector]++;
				if(arrowValues[currentItem-2][selector] == 10)
					arrowValues[currentItem-2][selector] = 0;
				break;
			case 1:
				arrowValues[currentItem-2][selector]--;
				if(arrowValues[currentItem-2][selector] == -1)
					arrowValues[currentItem-2][selector] = 9;
				break;	
			case 2:
				selector++;
				if(selector == 3)
					selector = 0;
				break;
			case 3:
				selector--;
				if(selector == -1)
					selector = 2;
				break;
	
		}
		printf("\r");
		for(x = 0; x < 3; x++){
			printf("%d", arrowValues[currentItem-2][x]);
		}	

	}

}


void confirmButton(){

	if(isSlotSelected == 0 && currentItem == 0){
		currentItem = 1;
		isSlotSelected = 1;
		setFile();
	
	}
	else{

		consoleClear();

		int isWritten = 0;
		for(int x = 0; x < 6; x++){
			if(newValues[x] != 0){
				isWritten = 1;
				break;
			}
		}

		if(isWritten == 1){
			new_rupees = (newValues[0] * 100000) + (newValues[1] * 10000) + (newValues[2] * 1000) + (newValues[3] * 100) + (newValues[4] * 10) + newValues[5];	
			fseek(fp,rupID,SEEK_SET);
			fwrite(&new_rupees, sizeof(long int), 1, fp);
			printf("%u Rupees set!\n", new_rupees);
		}


		isWritten = 0;

		for(int x = 0; x < 6; x++){
			for(int y = 0; y < 3; y++){
				if(arrowValues[x][y] != 0){
					isWritten = 1;
					break;
				}
			}

			if(isWritten == 1){
				newArrows = (arrowValues[x][0] * 100) + (arrowValues[x][1] * 10) + arrowValues[x][2];
				fseek(fp,arrowsID + (x * 8),SEEK_SET);
				fwrite(&newArrows, sizeof(int), 1, fp);
				printf("%u %s set!\n", newArrows, arrowName[x]);
			}
		}

	}


	selector = 0;
		
	
}


void setItem(){

	consoleClear();

	if(currentItem > 7) currentItem = 7;
	if(currentItem < 1) currentItem = 1;


	switch(currentItem){

		case 1:
			fseek(fp,rupID,SEEK_SET);
			fread(&rupees, sizeof(long int), 1, fp);
			printf("Current Ruppes: %u \n\n", rupees);
			printf("Enter your new ruppes quant. (Use DPad UP/Down to Increase/Decrease)\n");
			for(int x = 0; x < 6; x++){
				printf("%d", newValues[x]);
			}	
			break;

	}


	if(currentItem > 1){

		fseek(fp, arrowsID + ((currentItem - 2) * 8), SEEK_SET);
		fread(&arrowCount[currentItem-2], sizeof(long int), 1, fp);
		printf("Current %s: %u \n\n", arrowName[currentItem-2] , arrowCount[currentItem-2]);
		printf("Enter your new arrows quant. (Use DPad UP/Down to Increase/Decrease) \n");
		for(int x = 0; x < 3; x++){
			printf("%d", arrowValues[currentItem-2][x]);
		}	
		
	}

	selector = 0;
	
}


void setFile(){


	char file_name[256];
	char header[] = "Checkpoint/saves/0x01007EF00011E000 The Legend of Zelda  Breath of the Wild/botw/";
	char footer[] = "/game_data.sav";

	snprintf(file_name, sizeof file_name, "%s%d%s", header, slot, footer);
	
	fp = fopen(file_name,"r+b");
	
	if( fp == NULL )
	{
		perror("Error while opening the file.\n");
		printf("Press + to exit!");
	}


	else{
		setItem();	
	}
	
}

