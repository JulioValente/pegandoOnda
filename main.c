#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
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
		uint16_t BitsPerSample;
}cabecalho_t;

typedef struct{
	char SubChunk2Id[4];
	uint32_t SubChunk2Size;
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
    char nome[300], cut[300];
    int i;
    int segundosComeco;
    int segundosDuracao;
    int numBytesPular;
    int numSamplesLer;

    strcpy(cut, "cut_");

    cabecalho_t wavCab;
    wave_t wavDat;
    int16_t data, volume;

    setlocale(LC_ALL, "");

    do{

		printf(" escreva o nome do arquivo que desja abrir: ");
		scanf("%s", &nome);

		nl(1);

    	strcat(nome, ".wav");

    	fpRd = fopen(nome, "rb");

    	if(!fpRd){
			printf(" não foi possível abrir o arquivo! Arquivo não é do formato .wav ou não existe");
			nl(2);
		}
	}while(!fpRd);

	fread ((void *)&wavCab, sizeof(wavCab), 1, fpRd);
	fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd);

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

    printf(" bits per sample: %d", wavCab.BitsPerSample);

    nl(2);

	printf(" subchunk2 ID: ");
    p4(wavDat.SubChunk2Id);

    nl(1);

	printf(" subchunk2 Size: %d", wavDat.SubChunk2Size);

    nl(2);

	printf(" digite o quanto deseja diminuir ou aumentar o áudio: ");
    scanf("%d", &volume);

    nl(1);

    printf(" digite em quantos segundos quer o começo do corte: ");
    scanf("%d", &segundosComeco);

	printf(" digite em quantos segundos quer de duração do corte: ");
	scanf("%d", &segundosDuracao);

	nl(1);

	printf(" aperta uma tecla aí jão");

    numBytesPular = segundosComeco*wavCab.SampleRate*wavCab.BitsPerSample/8;
    numSamplesLer = segundosDuracao*wavCab.SampleRate;

    wavDat.SubChunk2Size = numSamplesLer * (wavCab.BitsPerSample/8);
    wavCab.ChunkSize = wavDat.SubChunk2Size + 36;

    strcat(cut, nome);

    fpWt = fopen(cut, "wb");

    fwrite((void *)&wavCab, sizeof(wavCab), 1, fpWt);
	fwrite((void *)&wavDat, sizeof(wavDat), 1, fpWt);

    fseek(fpRd, numBytesPular, SEEK_CUR);

	for(i=0; i<numSamplesLer; i++){
		fread ((void *)&data, sizeof(int16_t), 1, fpRd);
        data = data/volume;
		fwrite ((void *)&data, sizeof(int16_t), 1, fpWt);
	}

	fclose(fpRd);
    fclose(fpWt);

	PlaySound(TEXT(cut), NULL, SND_FILENAME | SND_ASYNC);

    getch();
    return 0;
}
