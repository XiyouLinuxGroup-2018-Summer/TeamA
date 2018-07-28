#include<stdio.h>
#include<string.h>
struct strange {
	char a, b, c, d, e, zero;
};
int main(int argc,char *argv[])
{
	struct strange xp[3];
	int characters[6] ={'a','b','c','d','e'};//将字符对应的ascii值存进数组
for(int i =0; i < 6; i++) {
	*((char*)(xp +0) + i) =characters[i];
}
	strcpy((char*)&xp[1], (char*)characters);
	memcpy(&xp[2], characters,sizeof(struct strange));

	printf("%zu\n", strlen((char*)characters));
for(int i =0; i <3; i++) {
	printf("%c\n", xp[i].e);
	}
}
