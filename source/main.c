#include <string.h>
#include <stdio.h>
#include <switch.h>

#include "translations.h"
#include "mount.h"


//Functions Prototypes
void confirmButton(); 
void intTextbox(int c);
void setFile();
void setItem();
void getData();
int power(int x, int y);


long int rupeeValue;

char itemName[50][40];
long int quantItems[50];
long int newQuantItems[50];
int numberOfItems;


int selector = 0;
int isSlotSelected = 0;
int currentItem = 0;
int slot = 0;

long int rupees;

int rupID[7]      = {0x00e0a0, 0x00e110, 0x00e110, 0x00e678, 0x00e730, 0x00eaf8, 0x00eaf8};
int itemsID[7]    = {0x052828, 0x0528d8, 0x0528c0, 0x053890, 0x05fa48, 0x060408, 0x060408};
int itemsQuant[7] = {0x063340, 0x0633f0, 0x0633d8, 0x064550, 0x070730, 0x0711c8, 0x0711c8};
int header[7]     = {0x24e2,   0x24ee,   0x2588,   0x29c0,   0x3ef8,   0x471a,   0x471b};
char versionArray[7][5]  = {"1.0",  "1.1",  "1.2",  "1.3", "1.3.3",  "v1.4",  "v1.5"};


int maxArrows = 999;
int version;



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
        		selector = 0;
        		setItem();
        	}
        	
        }

        if(kDown & KEY_R){
        	if(currentItem != 0){
        		currentItem++;
        		selector = 0;
        		setItem();
        	}
        	
        }

        if (kDown & KEY_PLUS){
        	fclose(fp);
    		unmountSaveData();
    		fsdevUnmountDevice("save");
        	break;
        } 

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}


int getDigit(int num, int n){
	
	int r, t1, t2;
 
    t1 = power(10, n+1);
    r = num % t1;
 
    if (n > 0)
    {
        t2 = power(10, n);
        r = r / t2;
    }
 
    return r;
}


void confirmButton(){
	if(isSlotSelected == 0 && currentItem == 0){
		currentItem = 1;
		isSlotSelected = 1;
		setFile();
	
	}

	else{
		consoleClear();
		if(rupeeValue != rupees){
			printf("Set Rupees = %d\n", rupeeValue);
			fseek(fp,rupID[version],SEEK_SET);
			fwrite(&rupeeValue, sizeof(long int), 1, fp);
		}

		for(int x = 0; x < numberOfItems; x++){
			if(newQuantItems[x] != quantItems[x]){
				if((strcmp(translate(itemName[x]), "Arrow") == 0) || (strcmp(translate(itemName[x]), "Fire Arrow") == 0) || (strcmp(translate(itemName[x]), "Ice Arrow") == 0) || (strcmp(translate(itemName[x]), "Shock Arrow") == 0) || (strcmp(translate(itemName[x]), "Ancient Arrow") == 0) || (strcmp(translate(itemName[x]), "Bomb Arrow") == 0)){
					if(newQuantItems[x] > 999){
						newQuantItems[x] = 999;
					}
				}
				printf("Set %s = %d\n", translate(itemName[x]), newQuantItems[x]);
				fseek(fp, itemsQuant[version] + (8 * x),SEEK_SET);
				fwrite(&newQuantItems[x], sizeof(int), 1, fp);
			}
		}
		fclose(fp);
    	unmountSaveData();
		printf("Press + to exit");
	}
}



void intTextbox(int c){

	if(currentItem == 0){
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
	}


	

	else if (currentItem == 1){

		int val = 0;
		int checkDigit = 0;
		
		setItem();
		printf("\r");

		for(int x = 0; x < 6; x++){
			if(x == selector){
				printf("^");
			}
			else{
				printf(" ");
			}
			
		}

		switch(c){
			case 0:
				val = power(10, (5-selector));
				checkDigit = getDigit(rupeeValue, 5-selector);
				if((checkDigit + 1) == 10){
					int newval = -1 * (val * 10);
					rupeeValue += newval;
				}
				rupeeValue += val;
				break;
			case 1:
				val = (-1) * power(10, (5-selector));
				
				checkDigit = getDigit(rupeeValue, 5-selector);
				
				if((checkDigit - 1) == -1){
					int newval = -1 * (val * 10);
					rupeeValue += newval;
				}
				rupeeValue += val;
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
		setItem();

	}



	else if(currentItem > 1){

		int val = 0;
		int checkDigit = 0;
		
		setItem();
		printf("\r");

		for(int x = 0; x < 6; x++){
			if(x == selector){
				printf("^");
			}
			else{
				printf(" ");
			}
			
		}

		switch(c){
			case 0:
				val = power(10, (5-selector));
				checkDigit = getDigit(newQuantItems[currentItem-2], 5-selector);
				if((checkDigit + 1) == 10){
					int newval = -1 * (val * 10);
					newQuantItems[currentItem-2] += newval;
				}
				newQuantItems[currentItem-2] += val;
				break;
			case 1:
				val = (-1) * power(10, (5-selector));
				
				checkDigit = getDigit(newQuantItems[currentItem-2], 5-selector);
				
				if((checkDigit - 1) == -1){
					int newval = -1 * (val * 10);
					newQuantItems[currentItem-2] += newval;
				}
				newQuantItems[currentItem-2] += val;
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
		setItem();

	}
			
	
	
}





int power(int x, int y){

	int powered = x;
	if(y == 0){
		powered = 1;
		return powered;
	}

	int a;

	for(a = 0; a < y-1; a++){
		powered = powered*10;
	}

	return powered;

}


void setItem(){

	consoleClear();

	if(currentItem > numberOfItems) currentItem = numberOfItems;
	if(currentItem < 1) currentItem = 1;

	printf("BOTW Version: %s\n", versionArray[version]);
	printf("(Item no: %d)\n", currentItem);

	if(currentItem == 1){
		
		printf("Current Rupees: %u \n\n", rupees);
		printf("Enter your new rupees quant. (Use DPad UP/Down to Increase/Decrease)\n");
		printf("%06d\n", rupeeValue);	

	}


	if(currentItem > 1){
		printf("Current %s: %u \n\n", translate(itemName[currentItem-2]), quantItems[currentItem-2]);
		printf("Enter your new quant: (Use DPad UP/Down to Increase/Decrease) \n");
		printf("%06d\n", newQuantItems[currentItem-2]);
		
	}


	for(int x = 0; x < 6; x++){
			if(x == selector){
				printf("^");
			}
			else{
				printf(" ");
			}
			
		}

	//selector = 0;
	
}


void getData(){

	int readHeader;
	fread(&readHeader, sizeof(int), 1, fp);
	
	for(version = 0; version<7; version++){
		
		if(readHeader == header[version]){
			break;
		}
		
	}

	fseek(fp,rupID[version],SEEK_SET);
	fread(&rupees, sizeof(long int), 1, fp);
	rupeeValue = rupees;
	int endOfItems = 0;
	
	for(int y = 0; y < 50 ;y++){
		
		int offset = (y * 128);
		for(int x = 0; x < 5; x++){
			char tmpString[5];
			fseek(fp, itemsID[version] + (8 * x) + offset,SEEK_SET);
			fread(&tmpString, sizeof(int), 1, fp);

			if(tmpString[strlen(tmpString) - 1] == 2){
				tmpString[strlen(tmpString) - 1] = 0;
			}

			sprintf(itemName[y] + strlen(itemName[y]),"%s", tmpString);
			
			if(strcmp(itemName[y],"Armo")==0)
				endOfItems = 1;
		}
		if(endOfItems == 1)
			break;

		fseek(fp, itemsQuant[version] + (8 * y),SEEK_SET);
		fread(&quantItems[y], sizeof(int), 1, fp);
		newQuantItems[y] = quantItems[y];
		numberOfItems++;
	}	

}


void setFile(){


	mountSaveData();
	char file_name[75];
	//char file_name[] = "save:/0/game_data.sav";
	char header[] = "save:/";
	char footer[] = "/game_data.sav";

	snprintf(file_name, sizeof file_name, "%s%d%s", header, slot, footer);
	
	fp = fopen(file_name,"r+b");
	
	if( fp == NULL )
	{
		perror("Error while opening the file.\n");
		printf("Press + to exit!");
	}


	else{
		getData();
		setItem();	
	}
	
}





