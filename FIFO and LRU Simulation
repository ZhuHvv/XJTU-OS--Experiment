#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue> //队列

using namespace std;
int PP;			// 物理块数
int Length;		// 页面序列长度
int Lack = 0;	// 缺页数
int *Page;		// 页面序列数组
int *Block;		// 内存中的物理块
int *next_dist; // 用于记录当前内存块所存储的页到下一次使用的距离
int *bef_dist;	// 用于记录当前内存块所存储的页到上一次使用的距离
void menu();	// 生命目录生成函数
void input()	// 输入
{
	printf("Please Input the Length of the Sequence of Pages:");
	scanf("%d", &Length);
	Page = (int *)malloc(sizeof(int) * Length);
	printf("1.Generate the Sequence Randomly\n2.Input the Sequence Manually\nPlease Choose the Number:");
	int choice;
	scanf("%d", &choice);
	switch (choice)
	{
	case 1://随机产生页面序列
		printf("Please Input the Maximum Page Number:");
		int maximum;
		scanf("%d", &maximum);
		for (int i = 0; i < Length; i++)
			Page[i] = rand() % (maximum + 1);
		break;
	case 2://手动输入页面序列
		printf("Please Input the Sequence of Pages:");
		for (int i = 0; i < Length; i++)
			scanf("%d", &Page[i]);
		break;
	default:
		printf("Please Choose:\n1.Generate the Sequence Randomly\n2.Input the Sequence Manually\n");
	}
	printf("Please Input the Amount of Blocks:");
	scanf("%d", &PP);
	Block = (int *)malloc(sizeof(int) * PP); // 指针变量分配空间
	next_dist = (int *)malloc(sizeof(int) * PP);
	bef_dist = (int *)malloc(sizeof(int) * PP);
	memset(Block, -1, sizeof(int) * PP); // memset是在一段内存块中填充给定的值，对指针初始化
	menu();
}

void ShowBlock() // 显示现有内存中的物理块
{
	for (int i = 0; i < PP; i++) // 遍历现有内存
	{
		if (Block[i] >= 0) // 逐个输出
			printf("%d\t", Block[i]);
	}
}

int is_in(int *block, int page) // 找一下传入的页面是否在内存中
{
	for (int i = 0; i < PP; i++) // 遍历现有内存
	{
		if (page == block[i]) // 找到
		{
			return i;
		}
	}
	return -1; // 内存中没有，返回-1
}

int Next_Time(int start, int target, int *page) // 统计内存中的某个页面，离下一次出现的距离
{												// i表示从页面序列里的第几个页面开始搜索，j是内存中的某个页面，page是页面序列
	int distance = 0, i = start;
	while (i < Length && target != page[i]) // 在页面序列中逐个搜索并且不匹配时
	{
		distance++; // 距离数增加
		i++;		// 下一个页面
	}
	if (i >= Length) // target页面后续没再出现
		return distance + 1;
	else // 找到下一次target页面
		return distance;
}

int Last_Time(int start, int target, int *page) // 统计内存中的某个页面，离上一次出现的距离
{												// i表示从页面序列里的第几个页面开始搜索，j是内存中的某个页面，a是页面序列
	int distance = 0, i = start;
	while (i >= 0 && target != page[i]) // 在页面序列中逐个向前搜索并且不匹配时
	{
		distance++; // 距离数增加
		i--;		// 下一个页面
	}
	if (i < 0) // target页面之前没出现
		return distance + 1;
	else // 找到上一次target页面
		return distance;
}

int max(int *bef_dist) // 找最近最久没使用的内存块
{
	int k = 0;
	int max = bef_dist[0];
	for (int i = 1; i < PP; i++) // 遍历现有内存
	{
		if (bef_dist[i] > max)
		{
			max = bef_dist[i];
			k = i;
		}
	}
	return k; // 返回最大值的下标
}
void FIFO() // 先进先出
{
	int i = 0, j = 0;  // i用来访问页，j用来访问内存
	int out = -1;	   // 要淘汰的页面
	int Lack = 0;	   // 缺页数
	queue<int> que;	   // 队列初始化
	while (i < Length) // 对页面序列逐个访问
	{
		printf("访问%d\t", Page[i]);
		if (is_in(Block, Page[i]) == -1) // 页面不在内存中
		{
			if (j < PP) // 内存不满
			{
				Block[j] = Page[i]; // 页面直接进入内存
				j++;
				que.push(Page[i]); // 页面入队
			}
			else // 内存满，找页面换出
			{
				for (int k = 0; k < PP; k++) // 遍历现有内存
				{
					if (Block[k] == que.front()) // 找到队列第一个被放入的页面，在内存中的位置，淘汰
					{
						out = Block[k];		// 记录要淘汰的页面
						Block[k] = Page[i]; // 把第一个放入的页面换出去
						que.pop();			// 从队列中移除
						que.push(Page[i]);	// 页面入队
						break;
					}
				}
			}
			ShowBlock();
			if (out == -1) // 没有淘汰页面
			{
				printf("未命中\t");
				printf("\n");
			}
			else
			{
				printf("未命中\t");
				printf("淘汰%d\n", out);
			}
			Lack++; // 统计缺页数
		}
		else // 页面在内存中，就直接显示
		{
			ShowBlock();
			printf("命中\n");
		}
		i++; // 一个页面访问完成，进行下一个页面
	}
	printf("缺页次数:%d\n", Lack);
	printf("缺页率:%.3lf\n", (double)Lack / Length);
	menu();
}
void LRU() // 最近最久未使用
{
	int i = 0, j = 0;  // i用来遍历页面序列，j用来遍历内存块
	int out = -1;	   // 要淘汰的页面
	Lack = 0;		   // 缺页个数
	while (i < Length) // 对页面序列逐个访问
	{
		printf("访问%d\t", Page[i]);
		if (is_in(Block, Page[i]) == -1) // 页面不在内存中
		{
			if (j < PP) // 内存不满
			{
				Block[j] = Page[i]; // 页面直接进入内存
				j++;
			}
			else // 内存满，找页面换出
			{
				for (int k = 0; k < PP; k++) // 遍历现有内存
				{
					bef_dist[k] = Last_Time(i - 1, Block[k], Page); // 统计当前内存与上一次的距离
				}
				int m = max(bef_dist); // 找内存中距离最远的页面(最久未访问)
				out = Block[m];		   // 记录要淘汰的页面
				Block[m] = Page[i];	   // 把距离最远的页面换出去
			}
			ShowBlock();
			if (out == -1) // 内存不满的时候进去的，没有淘汰页面
			{
				printf("未命中\t");
				printf("\n");
			}
			else
			{
				printf("未命中\t");
				printf("淘汰%d\n", out);
			}
			Lack++; // 统计缺页数
		}
		else // 页面在内存中，就直接显示
		{
			ShowBlock();
			printf("命中\n");
		}
		i++; // 一个页面访问完成，进行下一个页面
	}
	printf("缺页次数:%d\n", Lack);
	printf("缺页率:%.3lf\n", (double)Lack / Length);
	menu();
}
void menu()
{
	int choice; // 菜单选择的功能号
	printf("\n-----Function Menu-----\n");
	printf("1:INPUT\n");
	printf("2:FIFO\n");
	printf("3:LRU\n");
	printf("4:QUIT\n");
	printf("-------------------\n");
	printf("Please Choose the Number Above:");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		input();
		break;
	case 2:
		printf("\nFIFO算法:\n");
		FIFO();
		break;
	case 3:
		printf("\nLRU算法:\n");
		LRU();
		break;
	case 4:
		exit(1);
	default:
		input(); // 默认进入输入
	}
}
int main()
{
	input(); // 输入
	return 0;
}
