
#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#include "stdbool.h"

#define BUFFER_SIZE 1024
#define UNDEFINED 1
#define HORNERPRIME 31


// #define EOF -1

/*
 64 bitlik key ile o keye ait site değeri hash tablosunda kullanılır.
*/
typedef struct KeyValuePair {
    unsigned long long int key;
    char *value;
} KeyValuePair;


/* 
 Key Value değerlerini tutacak hash tablosudur.
 örneğin
 1 - (15,"a.com")
 2 - (15,"b.com")
 ... şeklinde
*/
typedef struct HashTable {
    KeyValuePair **table; // bu TABLO DİZİSİ bu ilk durum kontrolü için bu şekilde aslında 
    int size;

} HashTable;

/*
geleneksel string liste hali kullanımı
*/
typedef struct String {
    char a;
    struct String * nextC;
} String;


/*
 Reading ilk okuma sırasında veriyi listeleyerek tüm terimlerin sayısını tutar.
 Bundan sonra okuma bu veri uyapısı üzerinden yapılır
 diğer bir faydası ise ilk başta bulunması istenen ayrık tterim sayısını elde etmek için kullanılacaktır.
 toplam terim sayısı set veri yapısının kullancağı hash tablosunun boyutu olacaktır.
*/
typedef struct Term {
    char * termName;
    struct Term *next;
} Term;

typedef struct ReadStruct {
    char * websiteName ; 
    Term * terms;
    struct ReadStruct * next;
} ReadStruct;


typedef struct Reading {
    ReadStruct * website;
    int totalTermCount; // bu tutulmayabilir ama set kullanılacaksa faydallı olabilir.
} Reading;


void appendString(String* string, char i ) {
    if(string->a == UNDEFINED) {
        string->a = i;
        return;
    }
    String *tmp = string;
    
    while(tmp->nextC != NULL) {
        tmp = tmp->nextC;
    }
    
    
    tmp->nextC = (String*) malloc(sizeof(String*));
    tmp->nextC->nextC = NULL;
    tmp->nextC->a = i;
}

String * initString() {
    String * string = (String* ) malloc(sizeof(String));
    string->a=UNDEFINED;
    string->nextC = NULL;
    // printf("deneme");

    return string;
}

int str2CharArr(char ** charArrPtr, String * string) {
    
    int size = 2, i = 0;
    if(string->a == UNDEFINED){
        printf("Exception : 1 tane kayit olmali.");
        return 0;
    }
    String * tmp = string;
    while(tmp->nextC != NULL) {
        size++;
        tmp = tmp->nextC;
    }
    
    if(tmp!=string){
        size++;
        
    }
    else {
        (*charArrPtr) = (char*) malloc(sizeof(char) * size);    
        (*charArrPtr)[0] = string->a;
        (*charArrPtr)[1] = '\0';
        return 1; 
    }
    
    *charArrPtr = (char*) malloc(sizeof(char) * size);

    tmp = string;
    while(tmp->nextC != NULL) {
        (*charArrPtr)[i] = tmp->a;
        tmp = tmp->nextC;
        i++;
    }

    (*charArrPtr)[i] = tmp->a;
    (*charArrPtr)[i+1] = '\0';
    return size;
}

long long int hornerMethod(char * charArr) {
    int size,i;
    for(size = 0 ; charArr[size] != '\0' ; size++ ) {    }
    unsigned long long int key=0;
    unsigned long int  multiplier = HORNERPRIME;
    for( i = size-1 ; ~i ; i--) {
        key= key + multiplier * (unsigned long long int)charArr[i];
        multiplier = multiplier * HORNERPRIME;
    }

    return key;
}


void linearProbing(HashTable * hashTable, unsigned long long int  key, char *value) {
    int i = 0;
    bool bucketFound = false;
    int idx = (key + i) % hashTable->size;
    while(!bucketFound) {
        if ( hashTable->table[(key + i) % hashTable->size] == NULL )  {
            hashTable->table[(key + i) % hashTable->size] = (KeyValuePair*) malloc(sizeof(KeyValuePair));
            hashTable->table[(key + i) % hashTable->size]->key =  key;
            hashTable->table[(key + i) % hashTable->size]->value  = value;

            
            bucketFound = true;
        } else if (!strcmp(hashTable->table[(key + i) % hashTable->size]->value, value) ) {
            // set oluşturma mantığı olarak kullanılacak aynı key value pair girildiyse girme.
            bucketFound = true;
        }
        else {
            i++;
        }


    }
}

void hashTablePushBack(HashTable * hashTable, char* key, char* value) {
    unsigned long long int key_ = hornerMethod(key);

     linearProbing(hashTable, key_, value);
    
    // if( !state ) {
    //     printf("tablo dolmuştur.");
    // }
    // else {
    //     printf("ekleme başarılı.");
    // }

}

typedef struct Sieves
{
    int * primes;
    int size;
} Sieves;


Sieves * SieveOfEratosthenes(int n)
{
   
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    bool prime[n + 1];
    memset(prime, true, sizeof(prime));
 
    for (int p = 2; p * p <= n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }
 
    // Print all prime numbers
    int size = 0,j=0, p;
    for (p = 2; p <= n; p++){
        if (prime[p])
          size++;

    }
    Sieves * sieves = (Sieves*) malloc(sizeof(Sieves));

    int * sievesArr = (int*) malloc(sizeof(prime) * size);
    
    for (p = 2; p <= n; p++){
        if (prime[p]) {
          sievesArr[j] = p;
          j++;
        }

    }
    
    sieves->primes = sievesArr;
    sieves->size = size;
    return sieves;
}


int binarySearch(Sieves* primes, int left, int right, int n)
{
    if (left<=right)
    {
        int mid = (left + right)/2;
 
        // base condition is, if we are reaching at left
        // corner or right corner of primes[] array then
        // return that corner element because before or
        // after that we don't have any prime number in
        // primes array
        if (mid == 0 || mid == primes->size-1)
            return primes->primes[mid];

 
        // now if n is itself a prime so it will be present
        // in primes array and here we have to find nearest
        // prime less than n so we will return primes[mid-1]
        if (primes->primes[mid] == n)
            return primes->primes[mid-1];
 
        // now if primes[mid]<n and primes[mid+1]>n that
        // mean we reached at nearest prime
        if (primes->primes[mid] < n && primes->primes[mid+1] > n)
            return primes->primes[mid];
        if (n < primes->primes[mid])
            return binarySearch( primes, left, mid-1, n);
        else
            return binarySearch( primes, mid+1, right, n);
    }
    return 0;
}

int findClosestPrime(int number) {
    Sieves* sieves = SieveOfEratosthenes(number);

    int closestPrime = binarySearch(sieves, 0, sieves->size-1, number );

    return closestPrime;
}

HashTable * initHashTable(int elementCount, float loadFactor) {
    int i ;
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));

    int tableSize = (elementCount)*(1/loadFactor);
    tableSize = findClosestPrime(tableSize);
    hashTable->size = tableSize;
    hashTable->table = (KeyValuePair**) malloc(sizeof(KeyValuePair*)* tableSize);
    

    for( i = 0 ; i < hashTable->size ; i++) {
        hashTable->table[i] = NULL;
    }
    
    return hashTable;
}



Reading * openReadingMaterial(char * file) {
    FILE* fptr = fopen(file, "r");
    char buffer[BUFFER_SIZE];
    
    Reading * reading = (Reading*) malloc(sizeof(Reading));
    reading->website = (ReadStruct*) malloc(sizeof(ReadStruct));
    ReadStruct * cursor = reading->website;
    reading->totalTermCount=0;
    while (fgets(buffer, sizeof(buffer), fptr)) {
        // char * x;
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[len-1] = 0;

        char * line = (char*) malloc(sizeof(char) * (len-1));
        strcpy(line, buffer );

        cursor->websiteName = line; 

        
        cursor->terms = (Term*) malloc(sizeof(Term));
        Term * termCursor = cursor->terms;
        char a = 0;
        // appendString(string, a);
        while ( a != '\n' && a != '\r' && a!=EOF) {
            String * string = initString();
            
            a = fgetc(fptr);
            while( a != ' ' && a!='\n' && a!=EOF) {
                appendString(string, a);
                a = fgetc(fptr);
            }

            char** charArrPtr = (char**) malloc(sizeof(char**));
            int size = str2CharArr(charArrPtr,string);

            if(a == '\n' || a==EOF) {
                (*charArrPtr)[size-1] = '\0';
                termCursor->next = NULL;
                termCursor->termName = *charArrPtr;
                
            }
            else {
                termCursor->termName = (char*) malloc(sizeof(char)* size);
                termCursor->termName = *charArrPtr;
                
                termCursor->next = (Term*) malloc(sizeof(Term));
                termCursor = termCursor->next;
            }
            reading->totalTermCount++;
            printf("%s ", *charArrPtr);
            
            
        }
        
        if(a == EOF) {
            cursor->next = NULL;    
        }
        else {
            cursor->next = (ReadStruct*) malloc(sizeof(ReadStruct));
            cursor = cursor->next;

        }

        printf("%s  \n",line);

    }
    return reading;
}

int getUniqueTermCount(Reading * webScraper) {
    
    ReadStruct * websites = webScraper->website;
    int count = 0;

    bool intersection;
    Term *term, *term2;
 
    while(websites!=NULL) {
        
        for(term = websites->terms; term != NULL ; term = term->next) {
            intersection = false;
            ReadStruct * cursor = websites->next;    
            while (cursor != NULL && intersection == false) {
                
                for(term2 = cursor->terms; term2 != NULL && intersection == false ; term2 = term2->next) {
                    if(strcmp(term->termName, term2->termName) == 0) {
                        intersection = true;
                    }
                }
                cursor = cursor->next;
            }    
            if(intersection == false) {

            count++;
            }
        }
        websites = websites->next;
    }
    return count;
}

void insertAllItemToHashTable(HashTable * hashTable, Reading * websiteScraper) {

    ReadStruct * websites = websiteScraper->website;
    
    Term * term;
    
    while(websites!=NULL) {

        for(term = websites->terms; term != NULL ; term = term->next) {
            hashTablePushBack(hashTable, term->termName, websites->websiteName);

        }
        websites = websites->next;
    } 

}

 void searchHashTable(HashTable * hashTable, char* operand1, char* operator, char* operand2, char* mode ) {
    bool bucketFound = false;
    int i = 0, idx;
    if(!strcmp(mode, "normal\n")) {
        if(!strcmp(operator, "veya")) {
            unsigned long long int key1 = hornerMethod(operand1);
            unsigned long long int key2 = hornerMethod(operand2);
            i = 0;
            idx= (key1 + i) % hashTable->size;
            while(hashTable->table[idx] != NULL) {
                if (hashTable->table[idx]->key == key1) {
                    printf("%s ", hashTable->table[idx]->value);
                    bucketFound = true;
                }
                i++;
                idx= (key1 + i) % hashTable->size;
            }
            i = 0;
            idx= (key2 + i) % hashTable->size;
            while(hashTable->table[idx] != NULL) {
                if (hashTable->table[idx]->key == key1) {
                    printf("%s ", hashTable->table[idx]->value);
                    bucketFound = true;
                }
                i++;
                idx= (key2 + i) % hashTable->size;
            }

            if(!bucketFound) {
                printf("Bu iki kelimenin birlikte geçtiği doküman bulunmamaktadır.\n");
            }
            else {
                printf("\n");
            }
        } else if (!strcmp(operator, "ve"))
        {
            HashTable * set = initHashTable(elementCount, loadFactor);
            

        } else {
            
            unsigned long long int key = hornerMethod(operand1);
            i = 0;
            idx= (key + i) % hashTable->size;
            while(hashTable->table[idx] != NULL) {
                if (hashTable->table[idx]->key == key) {
                    printf("%s ", hashTable->table[idx]->value);
                    bucketFound = true;
                }
                i++;
                idx= (key + i) % hashTable->size;
            }
            printf("\n");
            
        }
        
            
    }
    else if(!strcmp(mode, "detay\n")) {

    } else {
        printf("yanlış modu girdiniz.");
    }
}

int main() {
    
    Reading * websiteScraper = openReadingMaterial("Sample.txt");

    // bu seperate chaining için kullanılabilir.
    int uniqueTermCount = getUniqueTermCount(websiteScraper); 
    
    HashTable* hashTable = initHashTable(websiteScraper->totalTermCount, 0.5);
    
    insertAllItemToHashTable(hashTable, websiteScraper);
    
    char *mode=NULL, *line=NULL;
    int size=0;
    while(true)
    {
        printf("Normal mod mu Detay mod mu? (normal/detay)\n");
        
        getline(&mode, &size, stdin);

        
        // char line[BUFFER_SIZE];
        char operand1[BUFFER_SIZE], operator[BUFFER_SIZE], operand2[BUFFER_SIZE]; 
        int size = BUFFER_SIZE;

        getline(&line, &size, stdin);

        
        sscanf(line,"%s %s %s\n",  &operand1, &operator, &operand2); // %*c to remove trailing \n
       

        // lineSize = getline(&line, &len, stdin);
        // printf("%s %s %s\n", operand1, operator, operand2);        
        searchHashTable(hashTable, operand1, operator, operand2, mode);
        

    }
    



    return 0;
} 
