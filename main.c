#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(){

    PlaySound(TEXT("megalovania.wav"), NULL, SND_ASYNC);

    getch();
    return 0;
}
