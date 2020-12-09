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
		char SubChunk2Id[4];
		uint32_t SubChunk2Size;
}cabecalho_t;

typedef struct{
	uint32_t SubChunk2Size;
	char SubChunk2Id[4];
	int met;
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
    
    cabecalho_t wavCab, wavDat;
    int16_t data;
    
    setlocale(LC_ALL, "");
    
    do{

		printf(" escreva o nome do arquivo que desja abrir: ");    	
		scanf(" %s", &nome);
		
		nl(1);
    
    	strcat(nome, ".wav");
    
    	fpRd = fopen(nome, "rb");
    	
    	if(!fpRd){
			printf(" não foi possível abrir o arquivo! Arquivo não é do formato .wav ou não existe");
			nl(2);
		}else{
			strcat(cut, nome);
    	
    		fpWt = fopen(cut, "wb");
		}
			
	}while(!fpRd);
	
	fread ((void *)&wavCab, sizeof(wavCab), 1, fpRd);
	
	printf(" ID : ");
    p4(wavCab.ChunkID);
    
    nl(1);
	
	printf(" chunk size: %d", wavCab.ChunkSize);
	
	nl(1);
    
    printf(" format: ");
    p4(wavCab.Format);
    
    nl(1);
    
    printf(" subchunk1 ID: ");
    p4(wavCab.SubChunkID);
    
    nl(1);
    
    printf(" subchunk1 Size: %d", wavCab.SubChunkSize);
    
    nl(1);
    
    printf(" Audio Format: %d", wavCab.AudioFormat);
    
    nl(1);
    
    printf(" num chanels: %d", wavCab.NumChannels);
    
    nl(1);
    
    printf(" Sample Rate: %d", wavCab.SampleRate);
    
    nl(1);
    
    printf(" byte rate: %d", wavCab.ByteRate);
    
    nl(1);
    
    printf(" block align: %d", wavCab.BlockAlign);
    
    nl(1);
    
    printf(" bits per sample: %d", wavCab.BytesPerSample);
    
    nl(2);
    
    fwrite ((void *)&wavCab, sizeof(wavCab), 1, fpWt);

	for(i=0; i<wavDat.SubChunk2Size/2; i++){
		fread ((void *)&data, sizeof(int16_t), 1, fpRd);
			data = data/10;
		fwrite ((void *)&data, sizeof(int16_t), 1, fpWt);	
	}
    
	printf(" subchunk2 ID: ");
    p4(wavCab.SubChunk2Id);
    
    nl(1);
	
	printf(" subchunk2 Size: %d", wavCab.SubChunk2Size);
    
    nl(2);
    	
    fclose(fpRd);
    fclose(fpWt);
    
	//PlaySound(TEXT("megalovania.wav"), NULL, SND_ASYNC);
    //PlaySound("C:\Users\Marcelo\Desktop\os trabaio\zucco\pegandoOnda-master\megalovania.wav", NULL, SND_SYNC);

	printf(" aperta uma tecla aí jão");

    getch();
    return 0;
}
