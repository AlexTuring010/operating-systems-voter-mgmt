#include "./header_files/command.h"

// I use this little function in multiple commands so I decided to make it public here instead of rewriting it all the time

int testNumberParemeter(char* parameter){
    if(parameter == NULL){
        return 0;
    } else{
        int i = 0;
        while(parameter[i] != '\0'){
            if(parameter[i] < 48 || parameter[i] > 57){
                return 0;
            }
            i++;
        }
    }
    return 1;
}