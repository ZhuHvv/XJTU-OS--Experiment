#include<stdlib.h>
#include<stdio.h>
#include<random>\
int AP=0,PP=0;
typedef struct Node {//内存块队列节点
	int PageNumber;//该内存块存放的页序号
}Node;
typedef struct StoredPage {//内存块队列
	Node quere[PP];

}Stored;
int main() {
	printf("请输入想要模拟的算法（1：FIFO	2：LRU）：");
	
	return0;
} 