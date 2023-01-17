#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define false 0
#define true 1

#define LINE_SIZE 500
#define COLOR_SZ 50


// renkleri çevirmek için çıktı girdi
typedef enum color {
    RED , PINK , ORANGE , YELLOW , PURPLE , GREEN , BLUE , BROWN 
} color;

typedef int bool;

// vektörü sağa döndürür
void rotateRight(int ** map, int size, int row) {
    int * vector = map[row];
    int tmp = vector[size-1];

    for(int i = 0 ; i < size ; i++){
        int tmp2 = vector[i]; 
        vector[i] = tmp;
        tmp = tmp2;
    }

}

// vectörü yazdırır
void printRow(int ** map,int size, int row){
    int * vector = map[row];
    for(int i = 0 ; i < size ; i++){
        printf("%d ,",  vector[i]);
    }
    printf("\n");
}


// rengi outputta çevirmek için
char* colorDecode(int color) {
    char * colorName = (char*) malloc(sizeof(COLOR_SZ));
    if(color == RED) {
        colorName = "Red";
        return colorName;
    } else if (color == PINK) {
        colorName = "Pink";
        return colorName;
    } else if (color == PINK) {
        colorName = "Red";
        return colorName;
    } else if (color == ORANGE) {
        colorName = "Orange";
        return colorName;
    } else if (color == YELLOW) {
        colorName = "Yellow";
        return colorName;
    } else if (color == GREEN) {
        colorName = "Green";
        return colorName;
    } else if (color == BLUE) {
        colorName = "Blue";
        return colorName;
    } else if (color == BROWN) {
        colorName = "Brown";
        return colorName;
    } else{
        return "ERROR";
    }
}

// rengi inputta .çevirmek için
int colorEncode(char * color) {
    if(!strcmp(color, "Red")) {
        return RED;
    } else if (!strcmp(color, "Pink")) {
        return PINK;
    } else if (!strcmp(color, "Orange")) {
        return ORANGE;
    } else if (!strcmp(color, "Yellow")) {
        return YELLOW;
    } else if (!strcmp(color, "Green")) {
        return GREEN;
    } else if (!strcmp(color, "Blue")) {
        return BLUE;
    } else if (!strcmp(color, "Brown")) {
        return BROWN;
    } 
}

// yukarıdaki satırdaki kesişimlere bakmak için
bool checkUpwardIntersection(int ** map, int size, int row, bool debug) 
{
    if(row==0) {
        return false;
    }
    int * vector = map[row];
    for(int i = 0 ; i < size ; i++){
        if(debug) {
            printf("column %d \n", i );
        }
        for(int j = row-1 ; j > -1 ; j--) {
            if (debug) {
                printf("%d %d\n", map[j][i], vector[i] );
            }

            if(map[j][i] == vector[i]) {
               return true; 
            }
        }
    }
    return false;

}

// haritaları yazdırır
void printMap(int **map,int size, int directionIdx)
{
    int i, j;
    printf("\n");
    for ( i = 0 ; i < size ; i++ ) {
        if ( directionIdx == i ) {
            printf("-->");
        }
        for ( j = 0 ; j < size ; j++ ){
            printf("%d ," , map[i][j]) ;
        }
        printf("\n");
   }
}

// en son haritayı yazdırır
void printMapDecoded(int **map,int size, int directionIdx)
{
    int i, j;
    printf("\n");
    for ( i = 0 ; i < size ; i++ ) {
        if ( directionIdx == i ) {
            printf("-->");
        }
        for ( j = 0 ; j < size ; j++ ){
            char * color = colorDecode(map[i][j]);
            printf("%s ," , color) ;
        }
        printf("\n");
   }
}

// state space durumunu yazdırır. 3 3 3 gibi dökümanda bahsedildi.3'3 durum
void printStateSpaceVector(int* vector, int size)
{
    int i ;
    printf("states :");
    for ( i = 0 ; i < size ; i++ ) {
        printf("%d ,", vector[i] );
    }
    printf("\n") ;
}

// Ana algoritma haritanın doğruluğunu döner
bool colorGame(int ** map, int size, bool debug)
{
    int i;
    bool intersection = true;
    int * rotationCount = (int *) malloc(sizeof(int) * size) ;
    for(i = 0 ; i < size ; i++) {
        rotationCount[i] = size; 
    }
    // 
    for ( int i = 0, j = 0 ; i < size && i > -1 ; ) {
        
        if (debug) {
            printf("--------------------------------------\n");
            printf("\n");
            printMap(map, size, i);
            printf("intersection first \n" );
            printRow(map, size, i);
        }
        intersection = checkUpwardIntersection(map,size, i, debug);
        j=0;
        while(intersection && j < rotationCount[i] - 1){
            if(debug) {
                printMap(map, size, i);
                printf("rotate right \n");
            }

            rotateRight(map, size, i);
            if(debug) {
                printf("printing rotated row\n");
                printRow(map, size, i);
            }
            j++;intersection = checkUpwardIntersection(map, size, i, debug);
        }
        if(j==rotationCount[i]-1){
            if (debug) {
                printf("backtracking ...\n" );
            }
            i--;
            rotationCount[i]-=1;
            
            if(i == 0 && rotationCount[i] == 1) {
                return false;
            }
            for (int j = i+1 ; j < size ; j++) {
                rotationCount[j] = size;
            }
            if(debug) {
                printStateSpaceVector(rotationCount,size);
                printf("rotate right :\n" );

            }
            rotateRight(map, size, i);
            if(debug) {
                printf("printing rotated row\n");
                printRow(map, size, i);
            }
        } else {
            i++;
        }
        
    }
    printf("\t result state map\n");
    printMap(map, size, i);
    printf("\n");
    printMapDecoded(map, size, i);
    printf("\n");
    printf("\n");
    return true;
}

// test için rastgele dizi oluştururken kullanılır.
void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

// test oluşturulma fazı
int test(int tableSize, bool debug) {
    int i, j;
    int N = tableSize;
    int ** colorsMap = (int **) malloc(sizeof(int*)*N);
    
    for( i = 0 ; i < N ; i++ ) {
        colorsMap[i] = (int*) malloc(sizeof(int)*N);
    }

    int * vector = (int*) malloc(sizeof(int)*N);
    for(int i = 1 ; i <= N ; i++)
    {
        vector[i-1] = i;
    }

    for(int i = 0 ; i < N ; i++) {
        shuffle(vector, N);
        for( j = 0 ; j < N ; j++) {
            colorsMap[i][j] = vector[j];
        }
    }

    if(debug == false) {
        printf("\t\t normal mode printing\n");
        printf("\tfirst state map\n");
        printMap(colorsMap, N, 0);
        printf("\n\n");

    }     

    return colorGame(colorsMap, N, debug) ;

}

// dısardsan alındı okuma yapmak için stringi verilen character için bölmek için csv gibi
char** strSplit(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

// strSplit generic yapısı çökmesin diye
char ** strSplit_r(char * line, char c) {
    char * block = strdup(line);
    char ** result = strSplit(block, c);
    return result;
}

int main() {
    int i, j;
    char mod[COLOR_SZ];
    int tableSize;
    printf("What is the mode you using?(normal/n -- detail/d)");
    scanf("%s", &mod);
    printf("What is size of game table which you want create?(1<N<10)");
    scanf("%d", &tableSize);

    /*
        Test edilme kodu.
    */
    printf("TESTING IS DOING...");
    bool debug;

    if(strcmp(mod, "normal") == 0 || strcmp(mod, "n") == 0) {
        debug = false;
    }
    if(strcmp(mod, "detay") == 0 || strcmp(mod, "d") == 0) {
        debug = true;
    }

    for( i = 0 ; i < 100 ; i++) {
        if(test(tableSize, debug)) {
            printf("Right Game Map Created.\n");
        } else {
            printf("Wrong Game Map Created.\n");
        }
    }
    printf("TESTING IS DONE...\n Print your matrix\n");
    printf("\n---------------------------------\n");
    printf("example \nRed Green Blue\nGreen Red Blue\nBlue Red Green\n");

    /*
        Input Girdisi verilerek yapılacak çözüm. 
    */
    char str[LINE_SIZE];
    char **colors;

    int ** colorsMap = (int **) malloc(sizeof(int*)*tableSize);
    
    printf("\n---------------------------------\n");
    
    for( i = 0 ; i < tableSize ; i++ ) {
        colorsMap[i] = (int*) malloc(sizeof(int)*tableSize);
    }

    for ( i = 0 ; i < tableSize ; i++) {
        scanf(" %[^\n]",str); 
        colors=strSplit_r(str, ' ');
        for( j = 0 ; j < tableSize ; j++ ) {
            colorsMap[i][j] = colorEncode(colors[j]);

        }
    }
    printf("\n---------------------------------\n");

    if(colorGame(colorsMap,tableSize,debug)) {
        printf("Right Game Map Created.\n");
    } else {
        printf("Wrong Game Map Created.\n");
    }

    scanf("%d",tableSize);
    return 0;
}