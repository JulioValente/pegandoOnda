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
	printf(" autores: J�lio C�sar e Mateus Rocha. \t\t\t\t\t turma: 4323");
	printf("\n*================================================================================================================*");
	printf("\n\n");

	printf(" reduz ou aumenta o volume do �udio, valores para volume com valor positivo aumenta e negativo diminui");
	nl(2);
	printf(" corta o arquvo do ponto que o usu�rio selecionar at� o tempo de dura��o escolhido");
	nl(1);
	printf(" =======================================================================================================");

	nl(2);

}

/*cabe�alho do arquivo wave*/
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

/*estrutura para as informa��es do arquivo*/
typedef struct{
	uint32_t SubChunk2Size;
	char *ListTypeID;	//as informa��es do arquivo escritas em formato de string
}list_t;

int tabul (int t){ //fun��o para tabula��es
	int i;

	for(i=0; i<t; i++){
		printf("\t");
	}
}

int nl (int y){ //fun��o para novas linhas

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

void recebeList(void *dest, char *list, int *p, int bytes){ //l� uma certa quantidade de bytes do LIST na posi��o do ponteiro e atualiza o ponteiro
    memcpy(dest, (void *)(list + *p), bytes);
    *p+=bytes;
}

void escreveList(list_t list){ //escreve as informa��es do LIST
    int textPointer = 0;    //posi��o atual de leitura do list
    uint32_t textSize;      //tamanho do texto sucessor do id
    char id[] = "    \0";   //id
    char *text;             //string do texto sucessor do id

    recebeList((void *)id, list.ListTypeID, &textPointer, 4);   //l� o id
    if(!strcmp(id, "INFO")){
        while(textPointer < list.SubChunk2Size){
            recebeList((void *)id, list.ListTypeID, &textPointer, 4);           //l� o info id
            recebeList((void *)&textSize, list.ListTypeID, &textPointer, 4);    //l� o tamanho do texto

            text = (char *)malloc(sizeof(char)*textSize);                       //aloca mem�ria para a string do texto
            recebeList((void *)text, list.ListTypeID, &textPointer, textSize);  //recebe o texto

            /*imprime as informa��es*/
            if(!strcmp(id, "IART")){
                printf(" Nome do artista: %s", text);
            }else if(!strcmp(id, "ICRD")){
                printf(" Data de cria��o: %s", text);
            }else if(!strcmp(id, "IGNR")){
                printf(" G�nero: %s", text);
            }else if(!strcmp(id, "INAM")){
                printf(" T�tulo: %s", text);
            }else if(!strcmp(id, "ICMT")){
                printf(" Coment�rioS: %s", text);
            }else if(!strcmp(id, "ICOP")){
                printf(" Copyright: %s", text);
            }else if(!strcmp(id, "ITRK")){
                printf(" N�mero da faixa: %s", text);
            }else if(!strcmp(id, "IPRD")){
                printf(" Album: %s", text);
            }else if(!strcmp(id, "ISFT")){
                printf(" Software: %s", text);
            }else if(!strcmp(id, "IARL")){
                printf(" Local do arquivo: %s", text);
            }else if(!strcmp(id, "ICMS")){
                printf(" Comiss�o: %s", text);
            }else if(!strcmp(id, "IENG")){
                printf(" Engenheiro: %s", text);
            }else if(!strcmp(id, "IKEY")){
                printf(" Palavras chaves: %s", text);
            }else if(!strcmp(id, "IMED")){
                printf(" M�dia: %s", text);
            }else if(!strcmp(id, "ISBJ")){
                printf(" Descri��o do arquivo: %s", text);
            }else if(!strcmp(id, "ITCH")){
                printf(" T�cnico: %s", text);
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

	/*declarando vari�veis*/
    FILE *fpRd, *fpWt; 		  //fpRd = arquivo lido, fpWt = arquivo criado e modificado
    char nome[300], cut[300]; //nome = vari�vel de entrada, cut = onde estar� o "cut_"
    char id[5] = "    \0"; 	  //vetor para pegar o ID do sub-chunk lido ap�s o sub-chunk fmt
    int temList = 0;          //vari�vel que verifica se o arquivo tem o sub-chunk LIST
    int i; 					  //contador
    char *p; 				  //ponteiro para a vari�vel tempor�ria
    char temp[5] = "    \0";  //vari�vel tempor�ria
    int segundosComeco;		  //ponto onde come�a o corte
    int segundosDuracao;	  //quanto tempo dura o corte
    int numBytesPular;		  //define a quantidade de bytes que sr�o lidos
    int numSamplesLer;		  //define a quantidade de samples que sr�o lidas
	float volume;			  //vari�vel para ajustar os valore de volume
	int novoData;             //data vezes o volume

    cabecalho_t wavCab;		  //vari�vel para o cabe�alho
    data_t wavDat;			  //vari�vel para os valores do data
    list_t info;			  //vari�vel para as informa��es do arquivo
    int16_t data;	 		  //bytes que ser�o ajustados na mudan�a de volume
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
			printf(" n�o foi poss�vel abrir o arquivo! Arquivo n�o � do formato .wav ou n�o existe");
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

	/*coletando as informa��es do arquivo*/
	if(!strcmp(id, "data")){ //quando � encontrado o sub-chunk data ap�s o sub-chunk fmt
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd); //le o tamanho do data

		fseek (fpRd, wavDat.SubChunk2Size, SEEK_CUR); //percorre o sub-chunk data

		for(p=temp; *p!='\0'; p++){
        	fread((void *)p, 1, 1, fpRd); //l� quatro bytes e os coloca na vari�vel tempor�ria
        	*p = tolower(*p);
		}

		if(!strcmp(temp, "list")){ //quando � encontrado o sub-chunk LIST ap�s o sub-chunk data
			temList = 1;
			fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd); 					   //quantidade de bytes das informa��es
			info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size); 						   //definindo o tamanho do ListTypeID
			fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);								   //lendo as informa��es
			fseek (fpRd, -(wavDat.SubChunk2Size+info.SubChunk2Size+sizeof(info.SubChunk2Size)), SEEK_CUR); //voltando para o come�o do arquivo
		}else{
			fseek (fpRd, -4, SEEK_CUR); //voltando os 4 bytes lidos por temp caso n�o � encontrado o sub-chunk LIST
		}
	}else if(!strcmp(id, "list")){ //quando � encontrado o sub-chunk LIST ap�s o sub-chunk fmt
	    temList = 1;
		fread((void *)&info.SubChunk2Size, sizeof(info.SubChunk2Size), 1, fpRd);    //quantidade de bytes das informa��es
		info.ListTypeID = (char *) malloc(sizeof(char)*info.SubChunk2Size);         //definindo o tamanho do ListTypeID
		fread((void *)info.ListTypeID, info.SubChunk2Size, 1, fpRd);                //lendo as informa��es
		fseek(fpRd, 4, SEEK_CUR);                                                   //pula a string "data"
		fread ((void *)&wavDat, sizeof(wavDat), 1, fpRd);                           //le o tamanho do data
	}

	/*mostrando o cabe�alho*/
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

    /*mostrando as informa��es do arquivo de �udio*/
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
	/*coletando as altera��es a ser feitas no arquivo de �udio*/

	printf(" digite o quanto deseja diminuir ou aumentar o �udio (negativo para baixar e positivo para aumentar): ");
    scanf("%f", &volume);

	if(volume > 0){
		if(volume > 2){
			volume = 2;
			nl(2);
			printf(" volume reduzido para 2 para n�o causar extremo desconforto!!!");
			nl(2);
		}else{
			volume = volume;
		}
	}else if(volume < 0){
		volume = (-1)*volume;
		volume = 1/volume;
	}

    nl(1);

    printf(" digite em quantos segundos quer o come�o do corte: ");
    scanf("%d", &segundosComeco);

	printf(" digite em quantos segundos quer de dura��o do corte: ");
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

	/*escrevendo o cabe�alho e os dados no novo arquivo*/
    fwrite((void *)&wavCab, sizeof(wavCab), 1, fpWt);	//escrevendo o cabe�alho
    fwrite((void *)"data", 4, 1, fpWt); 				//colocando a string data e marcando o come�o do arquivo
	fwrite((void *)&wavDat, sizeof(wavDat), 1, fpWt);	//escrevendo os dados

	/*cortando o �udio*/
    fseek(fpRd, numBytesPular, SEEK_CUR);

	/*alterando o volume*/
	for(i=0; i<numSamplesLer; i++){
		fread ((void *)&data, sizeof(int16_t), 1, fpRd);

		//verifica se o data vezes o volume ultrapassa os valores m�ximos do data
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

	/*colocando as informa��es no arquivo novo*/
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
