#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
int get_number(int low, int high);/// 生成指定范围的随机奇数或2
bool check_P_isPrime(int num); /// 判断是否为素数
int get_P_Number(int low, int high);///生成随机素数p和q
int get_n_number(int *p ,int *q );///生成n且n小于128时重新生成p，q和n
int getGCD(int num1, int num2);///判断两个数互素
int get_e_number( int ein);///取随机数0<e<φ(n)使(e，φ(n))=1
int get_d_number(int n , int e ,int ein );///生成满足 d*e mod φ(n)=1的随机数d
int* switch_m(const char* str);///将明文M转换成ASCII存于数组m中
int* E_D_key( const int* ascii, int length, int e_or_d_key, int n_key);///加密或解密
char* switch_M(const int* m1 , int length);///将解码明文m1从ASCII转换成明文M

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));///随机种子
    int p ;///保密
    int q ;///保密
    int n = get_n_number(&p , &q );///公开,生成p，q，n
    int ein = (p-1) * (q-1);///保密
    int e = get_e_number( ein );///公开
    int d = get_d_number( n , e , ein );///保密
    wprintf(L"公钥对（%d，%d）\n", e,n);
    wprintf(L"私钥对（%d，%d）\n", d,n);
    wprintf(L"A向B发送信息M\n");
    char* M = (char*) malloc(1000 * sizeof(char));///发送明文
    scanf("%s", M);

    int* m = switch_m(M);
    wprintf(L"明文M的转换ASCII码明文m为\n");
    for (int i = 0; i < strlen(M); ++i) {
        printf("%d", m[i]);
        if (i < strlen(M) - 1) {
            printf(",");
        }
    }

    int* c=E_D_key(m, (int)strlen(M), e, n);
    wprintf(L"\n加密密文c为\n");
    for (int i = 0; i < strlen(M); ++i) {
        printf("%d", c[i]);
        if (i < strlen(M) - 1) {
            printf(",");
        }
    }

    int* m1=E_D_key(c, (int)strlen(M), d, n);
    wprintf(L"\n解密明文m1为\n");
    for (int i = 0; i < strlen(M); ++i) {
        printf("%d", m1[i]);
        if (i < strlen(M) - 1) {
            printf(",");
        }
    }

    char* M1 = switch_M( m1 ,(int)strlen(M));
    wprintf(L"\nASCII码明文m1的转换明文M1为\n");
    for (int i = 0; i <strlen(M1) ; ++i) {
        printf("%c", M1[i]);
    }
    printf("\n");

    return 0;
}

int get_number(int low, int high) {

    int random = 2+ rand() % ((high - low) / 2 + 1);
    if (random == 2) {
        return 2; //如果 random等于2，直接返回2
    }
    int result = 2 * random + low;
    if (result % 2 == 0) {
        // 如果结果是偶数，加1使其变成奇数
        result++;
    }
    return result;
}

bool check_P_isPrime(int num) {
    if (num < 2) {
        return false;
    }
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int get_P_Number(int low, int high) {
    int number;
    do {
        number = get_number(low, high);
    } while (!check_P_isPrime(number));
    return number;
}

int get_n_number(int* p ,int* q ) {
    int number = 0;
    while (number < 128) {
        do {
            *p = get_P_Number(2, 200);
            *q = get_P_Number(2, 200);
        } while (*p == *q);
        number = *p * *q;
    }
    return number;
}

int getGCD(int num1, int num2) {
    int a = num1;
    int b = num2;

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    if (a == 1) {
        return 1;// 互素返回1
    } else {
        return 0; // 不互素返回0
    }
}

int get_e_number( int ein){
    int random1;
    do {
        random1 = 2+rand() % ( ein-1 ) ;
    } while (!getGCD( random1, ein));

    return random1;
}

int get_d_number(int n , int e ,int ein ){
    int random2;
    do {
        random2 = 2+rand() % ( n-1 ) ;
    } while ( (random2 * e) % ein != 1 );
    return random2;
}

int* switch_m(const char* str ) {
    int* ascii = (int*)malloc(strlen(str) * sizeof(int));

    if (ascii == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < strlen(str); ++i) {
            ascii[i] = (int)str[i];
    }
    return ascii;
}

int* E_D_key(const int* ascii, int length, int e_or_d_key, int n_key ) {
    int* en = (int*)malloc(e_or_d_key * sizeof(int));
    int* SecCode = (int*)malloc(length * sizeof(int));
    for(int i = 0; i < length; ++i){
        for (int j = 0; j < e_or_d_key; ++j) {
            en[j] = ascii[i];
        }
        for (int k = 1; k < e_or_d_key; ++k) {
            en[0] = (en[0] * en[k]) % n_key;
        }
        SecCode[i]=en[0];
    }
    return SecCode;
}

char* switch_M(const int* m1 , int length) {
    char *M_number = (char *) malloc((length+1) * sizeof(char));

    if (M_number == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < length; ++i) {
        M_number[i] = (char)m1[i];
    }
    M_number[length] = '\0';
    return M_number;
}
