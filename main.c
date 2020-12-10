#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <math.h>
#include <stdint.h>

char abertura(){
	
	printf("*================================================================================================================*");
	printf("\n\t\t\tPegando Onda - editor de arquivo wave");
	nl(2);
	printf(" autores: Júlio César e Mateus Rocha. \t\t\t\t\t turma: 4323");
	printf("\n*================================================================================================================*");
	printf("\n\n");
	
	printf(" reduz ou aumenta o volume do áudio, valores para volume com valor positivo aumenta e negativo diminui");
	nl(2);
	printf(" corta o arquvo do ponto que o usuário selecionar até o tempo de duração escolhido");
	nl(1);
	printf(" =======================================================================================================");
	
	nl(2);
	
}

/*cabeçalho do arquivo wave*/
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

/*estrutura para o tamanho do data*/
typedef struct{
	uint32_t SubChunk2Size;
}data_t;

/*estrutura para as informações do arquivo*/
typedef struct{
	uint32_t SubChunk2Size;
	char *ListTypeID;	//as informações do arquivo escritas em formato de string
	char SubChunk3ID[4];
}list_t;

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

/*escreve as ID's*/
void p4 (char *str){

	int i=4;

	while(i){
		putchar(*str);
		str++;
		i--;
	}

}

/*programa principal*/
int main(){

	/*declarando variáveis*/
    FILE *fpRd, *fpWt; 		  //fpRd = arquivo lido, fpWt = arquivo criado e modificado
    char nome[300], cut[300]; //nome = variável de entrada, cut = onde estará o "cut_"
    char id[5] = "    \0"; 	  //vetor para pegar as ID's
    int i; 					  //contador
    char *p; 				  //ponteiro para a variável temporária
    char temp[5] = "    \0";  //variável temporária
    int segundosComeco;		  //ponto onde começa o corte
    int segundosDuracao;	  //quanto tempo dura o corte
    int numBytesPular;		  //define a quantidade de bytes que srão lidos
    int numSamplesLer;		  //define a quantidade de samples que srão lidas
	float volume;			  //variável para ajustar os valore de volume
	
    cabecalho_t wavCab;		  //variável para o cabeçalho
    data_t wavDat;			  //variável para os valores do data
    list_t info;			  //variável para as informações do arquivo
    int16_t data;	 		  //bytes que serão ajustados na mudança de volume
	/**/
	
    setlocale(LC_ALL, "");

    strcpy(cut, "cut_");
    
    abertura();

	/*coletando o nome e abrindo o arquivo para leitura*/
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

	/*data format*/

	for(p=id; *p!='\0'; p++){
        fread((void *)p, 1, 1, fpRd);
	}

	/*data format*/
	
	/*coletando as informações do arquivo*/
	if(!strcmp(id, "data")){
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd); //lendo os dados do arquivo
		
		fseek (fpRd, wavDat.SubChunk2Size, SEEK_CUR); //percorrendo o arquivo	
		
		for(p=temp; *p!='\0'; p++){
        	fread((void *)p, 1, 1, fpRd); //lê quatro bytes e os coloca na variável temporária
		}
		
		if(!strcmp(temp, "LIST")){ //quando é encontrado o LIST, ou seja, as informações do arquivo
			strcpy(id, temp);
			fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd); 					   //quantidade de bytes das informações
			info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size); 						   //definindo o tamanho do ListTypeID
			fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);								   //lendo as informações
			fseek (fpRd, -(wavDat.SubChunk2Size+info.SubChunk2Size+sizeof(info.SubChunk2Size)), SEEK_CUR); //voltando para o começo do arquivo
			memcpy(info.SubChunk3ID, "data", 4);														   //trocando o "LIST" por "data" para poder começar o áudio
		}else{
			fseek (fpRd, -4, SEEK_CUR); //voltando ao começo quando não é encontrado o "LIST"
		}
	}else if(!strcmp(id, "LIST")){
		fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd);
		info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size);
		fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);
		fread((void *)&info.SubChunk3ID, sizeof(info.SubChunk3ID), 1, fpRd);
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd);
	}
	
	/*mostrando o cabeçalho*/
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
    
    /*mostrando as informações do arquivo de áudio*/
    if(!strcmp(id, "LIST")){
		printf(" LIST subchunk2 size: %d", info.SubChunk2Size);
		nl(1);
		printf(" list type id: ");
		p4(info.ListTypeID);
		nl(1);
		for(i=4; i<info.SubChunk2Size; i++){
			printf("%c", *(info.ListTypeID+i));
		}
		nl(1);
	}

	/*mostrando os dados do arquivo*/
	printf(" subchunk2 ID: ");
	p4(id);
	
    nl(1);

	printf(" subchunk2 Size: %d", wavDat.SubChunk2Size);

    nl(2);
    
	/*coletando as alterações a ser feitas no arquivo de áudio*/
	
	printf(" digite o quanto deseja diminuir ou aumentar o áudio: ");
    scanf("%f", &volume);
	
	if(volume > 0){
		if(volume > 1.5){
			volume = 1.5;
			nl(2);
			printf(" volume reduzido para 1,5 para não causar extremo desconforto!!!");
			nl(2);
		}else{
			volume = volume;
		}
	}else if(volume < 0){
		volume = (-1)*volume;
		volume = 1/volume;
	}
	
    nl(1);

    printf(" digite em quantos segundos quer o começo do corte: ");
    scanf("%d", &segundosComeco);

	printf(" digite em quantos segundos quer de duração do corte: ");
	scanf("%d", &segundosDuracao);

	nl(1);

	printf(" aperte uma tecla para finalizar o programa");

	/*calculando o tempo do corte*/
    numBytesPular = segundosComeco*wavCab.SampleRate*wavCab.BitsPerSample/8;
    numSamplesLer = segundosDuracao*wavCab.SampleRate;

    wavDat.SubChunk2Size = numSamplesLer * (wavCab.BitsPerSample/8);
    wavCab.ChunkSize = wavDat.SubChunk2Size + 36;

	/*abrindo o novo arquivo com o prefixo "cut_"*/
    strcat(cut, nome);

    fpWt = fopen(cut, "wb");

	/*escrevendo o cabeçalho e os dados no novo arquivo*/
    fwrite((void *)&wavCab, sizeof(wavCab), 1, fpWt);	//escrevendo o cabeçalho
    fwrite((void *)"data", 4, 1, fpWt); 				//colocando a string data e marcando o começo do arquivo
	fwrite((void *)&wavDat, sizeof(wavDat), 1, fpWt);	//escrevendo os dados
	
	/*cortando o áudio*/
    fseek(fpRd, numBytesPular, SEEK_CUR);

	/*alterando o volume*/
	for(i=0; i<numSamplesLer; i++){
		fread ((void *)&data, sizeof(int16_t), 1, fpRd);
    	data = data*volume;
		fwrite ((void *)&data, sizeof(int16_t), 1, fpWt);	
	}

	/*colocando as informações no arquivo novo*/
	if(!strcmp(id, "LIST")){
		fwrite ((void *)id, 4, 1, fpWt);
		fwrite((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpWt);
		fwrite((void *)info.ListTypeID, info.SubChunk2Size, 1, fpWt);
	}

	/*fechando os dois arquivos*/
	fclose(fpRd);
    fclose(fpWt);

	/*reproduzindo o novo arquivo*/
	PlaySound(TEXT(cut), NULL, SND_FILENAME | SND_ASYNC);

	/*fim do programa*/
    getch();
    return 0;
}
