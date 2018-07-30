
/********************第一题原始代码*********************/
typedef struct tag_unimportant {
    char *name;   	并没有给指针分配空间
    int num;
} A;
void func(A *a)		应该传递二维指针
{
    a = (A *)malloc(sizeof(A));
    strcpy(a->name, "Xiyou Linux Group"); 会报错,非法的地址访问
    a->num = 20180728;
}
int main(int argc, char *argv[])
{
    A *a;
    func(a);
    printf("%s %d\n", a->name, a->num); 	垃圾值，函数调用产生						的值被系统释放
    return 0;
}

/*********************第三题原始代码*********************/
struct strange {
    char a, b, c, d, e, zero;
};

int main(int argc, char *argv[])
{
    struct strange xp[3];
    int characters[6] = "abcde";
    for (int i = 0; i < 6; i++) {
        *((char *)(xp + 0) + i) = characters[i];
    }

    strcpy((char *)&xp[1], (char *)characters);
    memcpy(&xp[2], characters, sizeof(struct strange));
    printf("%zu\n", strlen((char *)characters));

    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);	
    }
    return 0;
}
