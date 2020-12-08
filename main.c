#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <locale.h>
#include <math.h>
#include <stdint.h>

typedef struct{
		char ChunkID[4];
		uint32_t ChunkSize;
		char Format[4];
    	char SubChunkID[4];
    	uint32_t SubChunkSize;
		uint16_t AudioFormat;
    	uint16_t NumChannels;
    	uint32_t SampleRate;
    	uint32_t ByteRate;
		uint16_t BlockAlign;
		uint16_t BytesPerSample;
}wavheader_t;

typedef struct{
	uint32_t SubChunk2Size;
	char SubChunk2Id[4];
}wave_t;

int tabul (int t){ //função para tabulações
	int i;
	
	for(i=0; i<t; i++){
		printf("\t");
	}
}

int nl (int y){ //função para novas linhas
	
	int i;
	
	for(i=0; i<y; i++){
		printf("\n");
	}
}

void p4 (char *str){
	
	int i=4;
	
	while(i){
		putchar(*str);
		str++;
		i--;
	}
	
}

int main(){

    FILE *fpRd, *fpWt;
    char nome[300], cut[]={"cut_"};
    int i;
    
    wavheader_t wHead;
    wave_t data;
    
    
    setlocale(LC_ALL, "");
    
    do{

		printf(" escreva o nome do arquivo que desja abrir: ");    	
		scanf(" %s", &nome);
		
		nl(1);
    
    	strcat(nome, ".wav");
    
    	fpRd = fopen(nome, "rb");
    	
    	strcat(cut, nome);
    	
    	fpWt = fopen(cut, "w+b");
    	
    	if(!fpRd){
			printf(" não foi possível abrir o arquivo! Arquivo não é do formato .wav ou não existe");
			nl(2);
		}
			
	}while(!fpRd);
    
	fread ((void *)&wHead, sizeof(wHead), 1, fpRd);
	
	for(i=0; i<data.SubChunk2Size; i++){
		fread ((void *)&data, sizeof(data.SubChunk2Size), 1, fpRd);
		fwrite ((void *)&data, sizeof(data.SubChunk2Size), 1, fpWt);	
	}
    
    printf(" ID : ");
    p4(wHead.ChunkID);
    
    nl(1);
	
	printf(" chunk size: %d", wHead.ChunkSize);
	
	nl(1);
    
    printf(" format: ");
    p4(wHead.Format);
    
    nl(1);
    
    printf(" subchunk1 ID: ");
    p4(wHead.SubChunkID);
    
    nl(1);
    
    printf(" subchunk1 Size: %d", wHead.SubChunkSize);
    
    nl(1);
    
    printf(" Audio Format: %d", wHead.AudioFormat);
    
    nl(1);
    
    printf(" num chanels: %d", wHead.NumChannels);
    
    nl(1);
    
    printf(" Sample Rate: %d", wHead.SampleRate);
    
    nl(1);
    
    printf(" byte rate: %d", wHead.ByteRate);
    
    nl(1);
    
    printf(" block align: %d", wHead.BlockAlign);
    
    nl(1);
    
    printf(" bits per sample: %d", wHead.BytesPerSample);
    
    nl(2);
    
    printf(" subchunk2 ID: ");
    p4(data.SubChunk2Id);
    
    nl(1);
	
	printf(" subchunk2 Size: %d", data.SubChunk2Size);
    
    nl(2);
    
    fclose(fpRd);
    fclose(fpWt);
    
	//PlaySound(TEXT("megalovania.wav"), NULL, SND_ASYNC);
    //PlaySound("C:\Users\Marcelo\Desktop\os trabaio\zucco\pegandoOnda-master\megalovania.wav", NULL, SND_SYNC);

	printf(" aperta uma tecla aí jão");

    getch();
    return 0;
}
