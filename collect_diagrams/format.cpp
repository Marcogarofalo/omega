#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


int main(){
   
    FILE *fp = fopen("all_diagrams_3pi_quark_contractions.txt", "r");
    FILE *fo = fopen("all_diagrams_formatted.txt", "w+");

    if (fp == NULL)
    {
        printf("Error: could not open file ");
        return 1;
    }

    // read one character at a time and
    // display it to the output
    char ch;
    while ((ch = fgetc(fp)) != EOF){
        if (ch =='+'|| ch=='-')
            fprintf(fo,"\n");
        if (ch =='\n')
            continue;
        fprintf(fo,"%c",ch);
    }
    fclose(fo);

    // close the file
    fclose(fp);

    return 0;
}