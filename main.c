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

void recebeList(void *dest, char *list, int *p, int bytes){ //lê uma certa quantidade de bytes do LIST na posição do ponteiro e atualiza o ponteiro
    memcpy(dest, (void *)(list + *p), bytes);
    *p+=bytes;
}

void escreveList(list_t list){ //escreve as informações do LIST
    int textPointer = 0;    //posição atual de leitura do list
    uint32_t textSize;      //tamanho do texto sucessor do id
    char id[] = "    \0";   //id
    char *text;             //string do texto sucessor do id

    recebeList((void *)id, list.ListTypeID, &textPointer, 4);   //lê o id
    if(!strcmp(id, "INFO")){
        while(textPointer < list.SubChunk2Size){
            recebeList((void *)id, list.ListTypeID, &textPointer, 4);           //lê o info id
            recebeList((void *)&textSize, list.ListTypeID, &textPointer, 4);    //lê o tamanho do texto

            text = (char *)malloc(sizeof(char)*textSize);                       //aloca memória para a string do texto
            recebeList((void *)text, list.ListTypeID, &textPointer, textSize);  //recebe o texto

            /*imprime as informações*/
            if(!strcmp(id, "IART")){
                printf(" Nome do artista: %s", text);
            }else if(!strcmp(id, "ICRD")){
                printf(" Data de criação: %s", text);
            }else if(!strcmp(id, "IGNR")){
                printf(" Gênero: %s", text);
            }else if(!strcmp(id, "INAM")){
                printf(" Título: %s", text);
            }else if(!strcmp(id, "ICMT")){
                printf(" ComentárioS: %s", text);
            }else if(!strcmp(id, "ICOP")){
                printf(" Copyright: %s", text);
            }else if(!strcmp(id, "ITRK")){
                printf(" Número da faixa: %s", text);
            }else if(!strcmp(id, "IPRD")){
                printf(" Album: %s", text);
            }else if(!strcmp(id, "ISFT")){
                printf(" Software: %s", text);
            }else if(!strcmp(id, "IARL")){
                printf(" Local do arquivo: %s", text);
            }else if(!strcmp(id, "ICMS")){
                printf(" Comissão: %s", text);
            }else if(!strcmp(id, "IENG")){
                printf(" Engenheiro: %s", text);
            }else if(!strcmp(id, "IKEY")){
                printf(" Palavras chaves: %s", text);
            }else if(!strcmp(id, "IMED")){
                printf(" Mídia: %s", text);
            }else if(!strcmp(id, "ISBJ")){
                printf(" Descrição do arquivo: %s", text);
            }else if(!strcmp(id, "ITCH")){
                printf(" Técnico: %s", text);
            }else if(!strcmp(id, "ISRC")){
                printf(" Fonte: %s", text);
            }else if(!strcmp(id, "ISRF")){
                printf(" Formato da fonte: %s", text);
            }else{
                printf(" %s: %s", id, text);
            }

            nl(1);
        }
    }else{
		for(; textPointer<list.SubChunk2Size; textPointer++){
			printf("%c", *(list.ListTypeID+textPointer));
		}
    }
}

/*programa principal*/
int main(){

	/*declarando variáveis*/
    FILE *fpRd, *fpWt; 		  //fpRd = arquivo lido, fpWt = arquivo criado e modificado
    char nome[300], cut[300]; //nome = variável de entrada, cut = onde estará o "cut_"
    char id[5] = "    \0"; 	  //vetor para pegar o ID do sub-chunk lido após o sub-chunk fmt
    int temList = 0;          //variável que verifica se o arquivo tem o sub-chunk LIST
    int i; 					  //contador
    char *p; 				  //ponteiro para a variável temporária
    char temp[5] = "    \0";  //variável temporária
    int segundosComeco;		  //ponto onde começa o corte
    int segundosDuracao;	  //quanto tempo dura o corte
    int numBytesPular;		  //define a quantidade de bytes que srão lidos
    int numSamplesLer;		  //define a quantidade de samples que srão lidas
	float volume;			  //variável para ajustar os valore de volume
	int novoData;             //data vezes o volume

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
        *p = tolower(*p);
	}

	/*data format*/

	/*coletando as informações do arquivo*/
	if(!strcmp(id, "data")){ //quando é encontrado o sub-chunk data após o sub-chunk fmt
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd); //le o tamanho do data

		fseek (fpRd, wavDat.SubChunk2Size, SEEK_CUR); //percorre o sub-chunk data

		for(p=temp; *p!='\0'; p++){
        	fread((void *)p, 1, 1, fpRd); //lê quatro bytes e os coloca na variável temporária
        	*p = tolower(*p);
		}

		if(!strcmp(temp, "list")){ //quando é encontrado o sub-chunk LIST após o sub-chunk data
			temList = 1;
			fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd); 					   //quantidade de bytes das informações
			info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size); 						   //definindo o tamanho do ListTypeID
			fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);								   //lendo as informações
			fseek (fpRd, -(wavDat.SubChunk2Size+info.SubChunk2Size+sizeof(info.SubChunk2Size)), SEEK_CUR); //voltando para o começo do arquivo
		}else{
			fseek (fpRd, -4, SEEK_CUR); //voltando os 4 bytes lidos por temp caso não é encontrado o sub-chunk LIST
		}
	}else if(!strcmp(id, "list")){ //quando é encontrado o sub-chunk LIST após o sub-chunk fmt
	    temList = 1;
		fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd);    //quantidade de bytes das informações
		info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size);         //definindo o tamanho do ListTypeID
		fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);                //lendo as informações
		fseek(fpRd, 4, SEEK_CUR);                                                   //pula a string "data"
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd);                           //le o tamanho do data
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
    if(temList){
		printf(" LIST subchunk2 size: %d", info.SubChunk2Size);
		nl(1);
		printf(" list type id: ");
		p4(info.ListTypeID);

		/*nl(1);
		for(i=4; i<info.SubChunk2Size; i++){
			printf("%c", *(info.ListTypeID+i));
		}
		nl(2);*/

		nl(2);
		escreveList(info);
	}

    nl(1);

	/*mostrando os dados do arquivo*/

	printf(" DATA subchunk2 Size: %d", wavDat.SubChunk2Size);

    nl(2);
	/*coletando as alterações a ser feitas no arquivo de áudio*/

	printf(" digite o quanto deseja diminuir ou aumentar o áudio (negativo para baixar e positivo para aumentar): ");
    scanf("%f", &volume);

	if(volume > 0){
		if(volume > 2){
			volume = 2;
			nl(2);
			printf(" volume reduzido para 2 para não causar extremo desconforto!!!");
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

		//verifica se o data vezes o volume ultrapassa os valores máximos do data
		novoData = data*volume;
		if(novoData > 0x7FFF){
            data = 0x7FFF;
		}else if (novoData < -0x8000){
            data = -0x8000;
		}else{
            data = novoData;
		}
		fwrite ((void *)&data, sizeof(int16_t), 1, fpWt);
	}

	/*colocando as informações no arquivo novo*/
	if(temList){
		fwrite ((void *)"LIST", 4, 1, fpWt);
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
