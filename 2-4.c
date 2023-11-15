
#include <stdio.h>
#include <stdlib.h>
// 常量定义
#define PROCESS_NAME_LEN 32   // 进程名长度
#define MIN_SLICE 10          // 最小碎片的大小
#define DEFAULT_MEM_SIZE 1024 // 内存大小
#define DEFAULT_MEM_START 0   // 起始位置
// 内存分配算法
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size = DEFAULT_MEM_SIZE; // 内存大小
int totalmem = DEFAULT_MEM_SIZE; // 初始总内存大小
int ma_algorithm = MA_FF;        // 当前分配算法
static int pid = 0;              // 初始 pid
int flag = 0;                    // 设置内存大小标志
// 描述每一个空闲块的数据结构
typedef struct free_block_type
{
    int size;
    int start_addr;
    struct free_block_type *next;
} free_block_type;
// 指向内存中空闲块链表的首指针
free_block_type *free_block_head = NULL;

// 每个进程分配到的内存块的描述
typedef struct allocated_block
{
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
} allocated_block;
// 进程分配内存块链表的首指针
allocated_block *allocated_block_head = NULL;

// 初始化空闲块，默认为一块，可以指定大小及起始地址
free_block_type *init_free_block(int mem_size)
{
    free_block_type *fb;
    fb = (free_block_type *)malloc(sizeof(free_block_type));
    if (fb == NULL)
    {
        printf("No mem\n");
        return NULL;
    }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}

// 显示菜单
void display_menu()
{
    printf("--------------MENU--------------\n");
    printf("1---Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2---Select memory allocation algorithm\n");
    printf("3---New process \n");
    printf("4---Terminate a process \n");
    printf("5---Display memory usage \n");
    printf("0---Exit\n");
    printf("--------------------------------\n");
}
// 设置内存的大小
int set_mem_size()
{
    int size;
    if (flag != 0)
    { // 防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size =");
    fflush(stdin);
    scanf("%d", &size);
    if (size > 0)
    {
        mem_size = size;
        totalmem = size;
        free_block_head->size = mem_size;
    }
    flag = 1;
    return 1;
}
// 按 FF 算法重新整理内存空闲块链表
void rearrange_FF()
{
    free_block_type *fbt;
    fbt = (free_block_type *)malloc(sizeof(free_block_type));
    if (free_block_head != NULL)
    {
        fbt->next = free_block_head;
        free_block_type *lastblock, *p, *fbt1, *fbt2;
        for (lastblock = NULL; lastblock != fbt; lastblock = p) // 按起始地址升序排列
        {
            p = fbt; // p记录的是最后一次交换的排在后面的节点
            for (fbt1 = fbt; fbt1->next->next != lastblock && fbt1->next->next != NULL; fbt1 = fbt1->next)
            {
                if (fbt1->next->start_addr > fbt1->next->next->start_addr)
                {
                    fbt2 = fbt1->next;
                    fbt1->next = fbt2->next;
                    fbt2->next = fbt1->next->next;
                    fbt1->next->next = fbt2;
                    p = fbt2;
                }
            }
        }
        free_block_head = fbt->next;
        free(fbt);
    }
}
// 按 BF 算法重新整理内存空闲块链表
void rearrange_BF()
{
    free_block_type *fbt;
    fbt = (free_block_type *)malloc(sizeof(free_block_type));
    if (free_block_head != NULL)
    {
        fbt->next = free_block_head;
        free_block_type *lastblock, *p, *fbt1, *fbt2;
        for (lastblock = NULL; lastblock != fbt; lastblock = p) // 按内存大小升序排列
        {
            p = fbt; // p记录的是最后一次交换的排在后面的节点
            for (fbt1 = fbt; fbt1->next->next != lastblock && fbt1->next->next != NULL; fbt1 = fbt1->next)
            {
                if (fbt1->next->size > fbt1->next->next->size)
                {
                    fbt2 = fbt1->next;
                    fbt1->next = fbt2->next;
                    fbt2->next = fbt1->next->next;
                    fbt1->next->next = fbt2;
                    p = fbt2;
                }
            }
        }
        free_block_head = fbt->next;
        free(fbt);
    }
}
// 按 WF 算法重新整理内存空闲块链表
void rearrange_WF()
{
    free_block_type *fbt;
    fbt = (free_block_type *)malloc(sizeof(free_block_type));
    if (free_block_head != NULL)
    {
        fbt->next = free_block_head;
        free_block_type *lastblock, *p, *fbt1, *fbt2;
        for (lastblock = NULL; lastblock != fbt; lastblock = p) // 按内存大小降序排列
        {
            p = fbt; // p记录的是最后一次交换的排在后面的节点
            for (fbt1 = fbt; fbt1->next->next != lastblock && fbt1->next->next != NULL; fbt1 = fbt1->next)
            {
                if (fbt1->next->size < fbt1->next->next->size)
                {
                    fbt2 = fbt1->next;
                    fbt1->next = fbt2->next;
                    fbt2->next = fbt1->next->next;
                    fbt1->next->next = fbt2;
                    p = fbt2;
                }
            }
        }
        free_block_head = fbt->next;
        free(fbt);
    }
}
// 按指定的算法整理内存空闲块链表
void rearrange(int algorithm)
{
    switch (algorithm)
    {
    case MA_FF:
        rearrange_FF();
        break;
    case MA_BF:
        rearrange_BF();
        break;
    case MA_WF:
        rearrange_WF();
        break;
    }
}
// 设置当前的分配算法
void set_algorithm()
{
    int algorithm;
    printf("--------Algorithm List--------\n");
    printf("\t1---First Fit\n");
    printf("\t2---Best Fit \n");
    printf("\t3---Worst Fit \n");
    printf("Please Choose the Algorithm:");
    scanf("%d", &algorithm);
    if (algorithm >= 1 && algorithm <= 3)
    {
        ma_algorithm = algorithm;
        // 按指定算法重新排列空闲区链表
        rearrange(ma_algorithm);
    }
    else
    {
        printf("Invalid choice!\n");
    }
}
// 碎片紧缩
void fregment_rearange(int request_size, allocated_block *ab)
{
    free_block_type *fbt1;
    fbt1 = (free_block_type *)malloc(sizeof(free_block_type));
    allocated_block *abtemp1,*abtemp2;
    if (mem_size - request_size >= MIN_SLICE)
    { // 剩余空间足够大，空闲分区只剩一个头节点
        free_block_head->size = mem_size - request_size;
        free_block_head->start_addr = totalmem - mem_size +request_size;
        free_block_head->next = NULL;
        ab->start_addr = totalmem - mem_size; // 修改进程块起始地址
        mem_size -= request_size;
    }
    else if (mem_size-request_size<MIN_SLICE)
    { // 剩余空间小于最小碎片大小，则全部分配，空闲分区清空
        fbt1 = free_block_head;
        free_block_head = NULL;
        ab->start_addr = totalmem - mem_size; // 修改进程块起始地址
        ab->size = mem_size;
        mem_size = 0;
        while (fbt1 != NULL)
        { // 释放其余节点指针
            free_block_type *fbt2 = fbt1;
            fbt1 = fbt1->next;
            free(fbt2);
        }
    }
    abtemp1 = allocated_block_head;
    abtemp2=allocated_block_head->next;
    while (abtemp2 != NULL)
    { // 紧缩，调整已分配的进程块的进程地址，使相邻
        abtemp2->start_addr = abtemp1->start_addr + abtemp1->size;
        abtemp2 = abtemp2->next;
        abtemp1=abtemp1->next;
    }
}
// 分配内存模块
int allocate_mem(allocated_block *ab)
{
    free_block_type *fbt, *pre;
    int request_size = ab->size;
    fbt = pre = free_block_head;
    // 根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    //  1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    //  2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    //  3. 找不到满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配
    //  4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    //  5. 分配成功则返回 1，否则返回-1
    if (mem_size <= 0)
    {
        return -1;
    }
    int flag = 0;                    // 找到置1；否则置0
    while (fbt != NULL && flag == 0) // 顺序访问空闲区链表，找到第一个满足进程大小的分区
    {
        if (fbt->size < request_size)
        {
            if (fbt == pre)
            {
                fbt = fbt->next;
            }
            else
            {
                fbt = fbt->next;
                pre = pre->next;
            }
        }
        else
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        int surplus = fbt->size - request_size; // 用于计算分配后的剩余大小
        if (surplus >= MIN_SLICE)
        { // 剩余空间足够大，分割
            ab->start_addr = fbt->start_addr;
            fbt->size = surplus;
            fbt->start_addr = fbt->start_addr + request_size;
        }
        else
        { // 剩余空间不足，直接分配
            if (pre == fbt)
            {
                ab->start_addr = fbt->start_addr;
                ab->size = fbt->size;
                free_block_head = free_block_head->next;
            }
            else
            {
                ab->start_addr = fbt->start_addr;
                pre->next = fbt->next;
                free(fbt);
            }
            rearrange(ma_algorithm); // 分配后重新排序
            mem_size=0;
        }
        return 1;
    }
    else // 没有找到足够大的分区，但是剩余总空间足够，采用碎片紧缩
    {
        if (mem_size >= request_size)
        {
            fregment_rearange(request_size, ab);
            return 1;
        }
        else
        {
            return -1;
        }
    }
}
// 创建新的进程，主要是获取内存的申请数量
int new_process()
{
    allocated_block *ab;
    int size;
    int ret;
    ab = (allocated_block *)malloc(sizeof(allocated_block));
    if (!ab)
        exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid); // 格式化输出
    ab->pid = pid;
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if (size > 0)
        ab->size = size;
    ret = allocate_mem(ab); // 从空闲区分配内存，ret==1 表示分配 ok
    // 如果此时 allocated_block_head 尚未赋值，则赋值
    if ((ret == 1) && (allocated_block_head == NULL))
    {
        allocated_block_head = ab;
        return 1;
    }
    // 分配成功，将该已分配块的描述插入已分配链表
    else if (ret == 1)
    {
        ab->next = allocated_block_head;
        allocated_block_head = ab;
        return 2;
    }
    else if (ret == -1)
    { // 分配不成功
        printf("Allocation fail\n");
        free(ab);
        return -1;
    }
    return 3;
}
// 寻找对应pid的进程
allocated_block *find_process(int pid)
{
    allocated_block *p;
    p = allocated_block_head;
    while (p) // 遍历链表找pid对应进程
    {
        if (p->pid == pid)
            return p; // 找到则返回struct
        p = p->next;
    }
    printf("\nProcess not found!\n"); // 没有找到则报错并返回NULL
    return NULL;
}
// 将 ab 进程快所占有已分配内存区归还，并进行可能的合并
int free_mem(allocated_block *ab)
{
    int algorithm = ma_algorithm;
    free_block_type *fbt, *pre, *work;
    fbt = (free_block_type *)malloc(sizeof(free_block_type));
    if (!fbt) return -1;
    // 进行可能的合并，基本策略如下
    // 1. 将新释放的结点插入到空闲分区队列末尾
    // 2. 对空闲链表按照地址有序排列
    // 3. 检查并合并相邻的空闲分区
    // 4. 将空闲链表重新按照当前算法排序请自行补充……
    mem_size += ab->size;
    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    fbt->next = NULL;
    rearrange(MA_FF);
    pre = work = free_block_head;
    if(!pre){
        free_block_head=fbt;
    }
    else if ((free_block_head && free_block_head->start_addr > fbt->start_addr))
    { // 直接插表头
        if (fbt->start_addr + fbt->size == free_block_head->start_addr)
        { // 向后合并
            fbt->size += free_block_head->size;
            fbt->next = free_block_head->next;
            free(free_block_head);
            free_block_head=fbt;
        }
        else
        {
            fbt->next = free_block_head;
            free_block_head = fbt;
        }
    }
    else
    {
        while (work && work->start_addr < fbt->start_addr)
        { // 遍历链表，用work找到第一个起始地址比fbt大的，用pre指向这个节点的前一个节点
            if (pre == work)
            {
                work = work->next;
            }
            else
            {
                pre = pre->next;
                work = work->next;
            }
        }
        if (!work)
        { // 在表尾，只有可能与之前的节点合并
            if (fbt->start_addr == pre->start_addr + pre->size)
            { // 向前合并
                pre->size += fbt->size;
                pre->next = fbt->next;
                free(fbt);
            }
            else
            {
                pre->next = fbt;
            }
        }
        else
        { // 在中间，三种皆有可能
            if ((fbt->start_addr + fbt->size == work->start_addr) && (fbt->start_addr == pre->start_addr + pre->size))
            { // 两头都合并
                pre->size = pre->size + fbt->size + work->size;
                pre->next = work->next;
                free(fbt);
                free(work);
            }
            else if (fbt->start_addr + fbt->size == work->start_addr)
            { // 只向后合并
            pre->next=fbt;
                fbt->size += work->size;
                fbt->next=work->next;
                free(work);
            }
            else if (fbt->start_addr == pre->start_addr + pre->size)
            { // 只向前合并
                pre->size += fbt->size;
                free(fbt);
            }
            else
            { // 不合并
                pre->next = fbt;
                fbt->next = work;
            }
        }
    }
    rearrange(algorithm);
    return 1;
}
// 释放 ab 数据结构节点
int dispose(allocated_block *free_ab)
{
    allocated_block *pre, *ab;
    if (free_ab == allocated_block_head)
    { // 如果要释放第一个节点
        allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != free_ab)
    {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}
// 删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点
void kill_process()
{
    allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL)
    {
        free_mem(ab); // 释放 ab 所表示的分配区
        dispose(ab);  // 释放 ab 数据结构节点
    }
}
// 显示当前内存的使用情况，包括空闲区的情况和已经分配的情况
int display_mem_usage()
{
    struct free_block_type *fbt = free_block_head;
    struct allocated_block *ab = allocated_block_head;
    printf("-------------------------------------------------------------\n");
    // 显示空闲区
    printf("Free Memory:\n");
    printf("%20s %20s\n", "start_addr", "size");
    while (fbt != NULL)
    {
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt = fbt->next;
    }

    // 显示已分配区
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "Process Name", "start_addr", "size");
    while (ab != NULL)
    {
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab = ab->next;
    }
    printf("-------------------------------------------------------------\n");
    return 0;
}
void do_exit()
{
    free_block_type *fbt1, *fbt2;
    allocated_block *abtemp1, *abtemp2;
    fbt1 = free_block_head;
    if (fbt1 != NULL)
    {
        fbt2 = fbt1->next;
        for (; fbt2 != NULL; fbt1 = fbt2, fbt2 = fbt2->next)
        {
            free(fbt1);
        }
        free(fbt1);
    }
    abtemp1 = allocated_block_head;
    if (abtemp1 != NULL)
    {
        abtemp2 = abtemp1->next;
        for (; abtemp2 != NULL; abtemp1 = abtemp2, abtemp2 = abtemp2->next)
        {
            free(abtemp1);
        }
        free(abtemp1);
    }
}
int main()
{
   int choice;
	free_block_head = init_free_block(mem_size);

	while (1)
	{
		display_menu();	//显示菜单
		fflush(stdin);	//清空缓冲区，防止误选
        printf("Please Choose:");
		scanf("%d", &choice);	//获取用户输入
		if (choice <= 5 && choice >= 0)
		{
            switch (choice)
            {
            case 1:
                set_mem_size();
                break; // 设置内存大小
            case 2:
                set_algorithm();
                flag = 1;
                break; // 设置算法
            case 3:
                new_process();
                flag = 1;
                break; // 创建新进程
            case 4:
                kill_process();
                flag = 1;
                break; // 删除进程
            case 5:
                display_mem_usage();
                flag = 1;
                break; // 显示内存使用
            case 0:
                do_exit();
                exit(0); // 释放链表并退出
            default:
                break;
            }
        }
        else
        {
            printf("\nInvalid choice, please select again!\n\n");
        }
    }
    return 0;
}