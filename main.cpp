#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

# This project implement process control, such as process creation and cancellation, process state transition;

# This is the status of process
#define Running 0
#define Ready 1
#define Blocked 2

char *Running_process;

RCB R1=init_RCB(1);
RCB R2=init_RCB(2);
RCB R3=init_RCB(3);
RCB R4=init_RCB(4);

//具有3个优先级的就绪队列RL初始化
ReadyList_head RL=init_ReadyList();
BlockList_head BL=init_BlockList();

void Init_PCBResource(PCB self)
{
    self->Resources=(Resource_head)malloc(sizeof(resource_node));
    self->Resources->resource_next=NULL;
    Resource_head m=self->Resources,p=NULL;
    for(int i=1;i<=4;i++)
    {
        p=(Resource_head)malloc(sizeof(resource_node));
        p->RID=i;
        p->number=0;
        p->resource_next=m->resource_next;
        m->resource_next=p;
    }
    
}

PCB create_init()
{
    PCB init=(PCB)malloc(sizeof(pcbnode));
    init->Priority=0;
    init->Status.Type=Running;
    init->PID="init";
    Init_PCBResource(init);
    init->Creation_Tree.Parent=(PCB)malloc(sizeof(pcbnode));
    init->Creation_Tree.Children=(PCB)malloc(sizeof(pcbnode));
    init->Creation_Tree.Children->Children_next=NULL;
    init->Children_next=NULL;
    init->List_next=RL->Priority_0->List_next;
    RL->Priority_0->List_next=init;
    Running_process="init";
    return init;
}

PCB Get_PCB(char * pid)
{
    PCB q0=NULL,q1=NULL,q2=NULL,qb=NULL;
    //优先级为2的队列的头节点
    q2=RL->Priority_2;
    //优先级为1的队列的头节点
    q1=RL->Priority_1;
    //优先级为0的队列的头节点
    q0=RL->Priority_0;
    qb=BL->block;
    
    //若优先级为2的队列不为空
    if(q2->List_next)
    {
        while(q2->List_next)
        {
            //找到当前运行的程序
            q2=q2->List_next;
            if(q2 && strcmp(q2->PID,pid)==0)
            {
                return q2;
            }
            
        }
    }
    //若优先级为1的队列不为空
    if(q1->List_next)
    {
        while(q1->List_next)
        {
            //找到当前运行的程序
            q1=q1->List_next;
            if(q1 && strcmp(q1->PID,pid)==0)
            {
                return q1;
            }
            
        }
    }
    //若优先级为0的队列不为空
    if(q0->List_next)
    {
        while(q0->List_next)
        {
            //找到当前运行的程序
            q0=q0->List_next;
            if(q0 && strcmp(q0->PID,pid)==0)
            {
                return q0;
            }
            
        }
    }
    //若优先级为0的队列不为空
    if(qb->List_next)
    {
        while(qb->List_next)
        {
            //找到当前运行的程序
            qb=qb->List_next;
            if(qb && strcmp(qb->PID,pid)==0)
            {
                return qb;
            }
            
        }
    }
    return NULL;
}

PCB Get_Current_PCB()
{
    PCB self=NULL;
    if(RL->Priority_2->List_next)
    {
        for(self=RL->Priority_2->List_next;self->List_next;self=self->List_next);
        return self;
    }
    if(RL->Priority_1->List_next)
    {
        for(self=RL->Priority_1->List_next;self->List_next;self=self->List_next);
        return self;
    }
    else
    {
        for(self=RL->Priority_0->List_next;self->List_next;self=self->List_next);
        return self;
    }
}

RCB Get_RCB(int rid)
{
    if(rid==1) return R1;
    else if(rid==2) return R2;
    else if(rid==3) return R3;
    else return R4;
}

//insert from the beginning
void Insert_RL(PCB p)
{
    if(p->Priority==0)
    {
        p->List_next=RL->Priority_0->List_next;
        RL->Priority_0->List_next=p;
    }
    else if(p->Priority==1)
    {
        p->List_next=RL->Priority_1->List_next;
        RL->Priority_1->List_next=p;
    }
    else if(p->Priority==2)
    {
        p->List_next=RL->Priority_2->List_next;
        RL->Priority_2->List_next=p;
    }
}

//remove the last
void Remove_RL(PCB p)
{
    PCB pre=NULL,rear=NULL;
    if(p->Priority==0)
    {
        pre=RL->Priority_0;
        if(pre->List_next)
        {
            rear=pre->List_next;
            while(rear->List_next)
            {
                rear=rear->List_next;
                pre=pre->List_next;
            }
            pre->List_next=NULL;
        }
        
    }
    else if(p->Priority==1)
    {
        pre=RL->Priority_1;
        if(pre->List_next)
        {
            rear=pre->List_next;
            while(rear->List_next)
            {
                rear=rear->List_next;
                pre=pre->List_next;
            }
            pre->List_next=NULL;
        }
    }
    else if(p->Priority==2)
    {
        pre=RL->Priority_2;
        if(pre->List_next)
        {
            rear=pre->List_next;
            while(rear->List_next)
            {
                rear=rear->List_next;
                pre=pre->List_next;
            }
            pre->List_next=NULL;
        }
    }
}

//insert from the beginning
void Insert_BL(PCB p)
{
    p->List_next=BL->block->List_next;
    BL->block->List_next=p;
}

void Remove_BL(PCB p)
{
    PCB pre=NULL,rear=NULL;
    pre=BL->block;
    rear=BL->block->List_next;
    while(rear)
    {
        if(strcmp(rear->PID,p->PID)==0)
        {
            pre->List_next=rear->List_next;
            break;
        }
        rear=rear->List_next;
        pre=pre->List_next;
    }
}


void Insert_Resource(PCB self,RCB r,int number)
{
    Resource_head p=self->Resources->resource_next;
    while(p)
    {
        if(p->RID==r->RID)
        {
            p->number=p->number+number;
            break;
        }
        else p=p->resource_next;
    }
    
}

void preempt(PCB self, PCB p)
{
    if(self)
    {
        if(self->Status.Type!=Blocked)
        {
            self->Status.Type=Ready;
            if(p->Priority>self->Priority)
            {
                Remove_RL(self);
                Insert_RL(self);
            }
        }
    }
    
    p->Status.Type=Running;
    Running_process=p->PID;
    
}

void Scheduler(PCB self)
{
    PCB p=NULL;
    p=Get_Current_PCB();
    if(self==NULL)
        preempt(self, p);
    else if (self->Priority < p->Priority ||self->Status.Type != Running)
        //self当前操作的进程，p首部的进程，p抢占self
        preempt(self, p);
}



void Remove_Resource(PCB self,int rid,int number)
{
    if(number!=0)
    {
        RCB r=Get_RCB(rid);
        //将资源r从当前进程占用的资源列表里移除，并且资源r的可用数量从u变为u+n
        Resource_head p=self->Resources->resource_next;
        while(p!=NULL)
        {
            if(p->RID==r->RID)
            {
                p->number -= number;
                break;
            }
            else p=p->resource_next;
        }
        r->Status.u=r->Status.u+number;
    }
}

void Insert_RCB_WL(RCB r, PCB self,int number)
{
    waitinglist_node * p=(waitinglist_node*)malloc(sizeof(waitinglist_node));
    if(p!=NULL)
    {
        p->PID=self->PID;
        p->number=number;
        p->waiting_next=r->Waiting_List->waiting_next;
        r->Waiting_List->waiting_next=p;
    }
}

void Remove_RCB_WL(RCB r,PCB self)
{
    
    Waitinglist_head pre=r->Waiting_List;
    if(pre->waiting_next!=NULL)
    {
        Waitinglist_head rear=pre->waiting_next;
        while(rear)
        {
            if(rear->PID==self->PID)
            {
                pre->waiting_next=rear->waiting_next;
                rear=rear->waiting_next;
                if(!rear)
                    break;
            }
            rear=rear->waiting_next;
            pre=pre->waiting_next;
        }
    }
    
}

PCB Create(PCB self,char *pid,int Priority)
// initialization parameters可以为进程的ID和优先级，优先级：初始进程0、用户进程1和系统进程2。
{
    //create PCB data structure
    PCB p=(PCB)malloc(sizeof(pcbnode));
    PCB q0=NULL,q1=NULL,q2=NULL;
    //优先级为2的队列的头节点
    q2=RL->Priority_2;
    //优先级为1的队列的头节点
    q1=RL->Priority_1;
    //优先级为0的队列的头节点
    q0=RL->Priority_0;
    
    //初始化pid
    p->PID=(char*)malloc(sizeof(char));
    strcpy(p->PID, pid);
    //初始化Resource队列
    Init_PCBResource(p);
    //初始化Status.Type
    p->Status.Type=Ready;
    //初始化优先级
    p->Priority=Priority;
    //初始化children
    p->Creation_Tree.Parent=(PCB)malloc(sizeof(pcbnode));
    p->Creation_Tree.Children=(PCB)malloc(sizeof(pcbnode));
    p->Creation_Tree.Children->Children_next=NULL;
    
    //link PCB to creation tree /*连接父亲节点和兄弟节点，当前进程为 父亲节点，父亲节点中的子节点为兄弟节点*/
    if(q2->List_next)//若优先级为2的队列不为空
    {
        while(q2->List_next)
            //找到当前运行的程序
            q2=q2->List_next;
        //将新建进程的parent指向正在运行的进程
        p->Creation_Tree.Parent=q2;
        //把新建进程加入到正在运行的进程的Children链表
        p->Children_next=q2->Creation_Tree.Children->Children_next;
        q2->Creation_Tree.Children->Children_next=p;
    }
    //若优先级为1的队列不为空
    else if(q1->List_next)
    {
        while(q1->List_next)
            //找到当前运行的程序
            q1=q1->List_next;
        p->Creation_Tree.Parent=q1;
        p->Children_next=q1->Creation_Tree.Children->Children_next;
        q1->Creation_Tree.Children->Children_next=p;
    }
    //若优先级为0的队列不为空
    else if(q0->List_next)
    {
        while(q0->List_next)
            q0=q0->List_next;
        p->Creation_Tree.Parent=q0;
        p->Children_next=q0->Creation_Tree.Children->Children_next;
        q0->Creation_Tree.Children->Children_next=p;
    }
    
    //插入就绪相应优先级队列的尾部
    Insert_RL(p);
    Scheduler(self);
    return p;
}

void Request(PCB self,int rid, int n)
{
    RCB r=NULL;
    r = Get_RCB(rid);
    // u为r->Status.u, 即可用资源数量
    if (r->Status.u >= n)
    {
        //self 为当前请求资源的进程PCB，insert以后n 个r为self进程占有的资源, 参PCB结构图
        r->Status.u=r->Status.u-n;
        Insert_Resource(self, r, n);
    }
    else
    {
        if (n>r->Status.k)
            //k为资源r的总数，申请量超过总数时，将打印错误信息并退出
            exit(1);
        //只要u<n, 就不分配，进程阻塞
        self->Status.Type = Blocked;
        //point to block list 注意：此时block list 是n个 r
        self->Status.List = r;
        // remove self from the ready list，因为运行进程位于绪队列首部，所以此时将它从就绪队列移除
        Remove_RL(self);
        Insert_BL(self);
        // 将进程self插入到资源r的等待队列尾部
        Insert_RCB_WL(r,self,n);
        Scheduler(self);
    }
}

void Release(PCB self,int rid, int n)
{
    RCB r=NULL;
    r = Get_RCB(rid);
    //从self的占用资源队列中删除，且已经把占用资源数恢复
    Remove_Resource(self, rid, n);
    //找到队首的等待进程
    while (r->Waiting_List->waiting_next != NULL )
    {
        Waitinglist_head p=r->Waiting_List;
        
        while(NULL!=p->waiting_next)
            p=p->waiting_next;
        int req_num=p->number;
        
        if(r->Status.u>=req_num)
        {
            PCB q;
            q=Get_PCB(p->PID);
            q->Status.Type = Ready;
            q->Status.List = RL;
            //插入资源r到进程q所占用的资源中
            Insert_Resource(q, r, req_num);
            //从block list中移除q
            Remove_BL(q);
            // 插入q到就绪队列
            Insert_RL(q);
            
            //可用资源数量减少
            r->Status.u = r->Status.u - req_num;
            // 从资源r的阻塞队列中移除刚刚被恢复的资源
            Waitinglist_head pre=r->Waiting_List,rear=pre;
            while(rear->waiting_next!=NULL)
            {
                rear=rear->waiting_next;
                if(rear->waiting_next!=NULL)
                    pre=pre->waiting_next;
            }
            pre->waiting_next=NULL;
            free(rear);
            rear=NULL;
        }
        
        else break;
        
    }
    //基于优先级的抢占式调度策略，因此当有进程获得资源时，需要查看当前的优先级情况并进行调度
    Scheduler(self);
    
}

void Release_destroy(PCB self,int rid, int n)
{
    RCB r=NULL;
    r = Get_RCB(rid);
    //从self的占用资源队列中删除，且已经把占用资源数恢复
    Remove_Resource(self, rid, n);
    //找到队首的等待进程
    while (r->Waiting_List->waiting_next != NULL )
    {
        Waitinglist_head p=r->Waiting_List;
        
        while(NULL!=p->waiting_next)
            p=p->waiting_next;
        int req_num=p->number;
        
        if(r->Status.u>=req_num)
        {
            PCB q;
            q=Get_PCB(p->PID);
            q->Status.Type = Ready;
            q->Status.List = RL;
            //插入资源r到进程q所占用的资源中
            Insert_Resource(q, r, req_num);
            //从block list中移除q
            Remove_BL(q);
             // 插入q到就绪队列
            Insert_RL(q);
            
            //可用资源数量减少
            r->Status.u = r->Status.u - req_num;
            // 从资源r的阻塞队列中移除刚刚被恢复的资源
            Waitinglist_head pre=r->Waiting_List,rear=pre;
            while(rear->waiting_next!=NULL)
            {
                rear=rear->waiting_next;
                if(rear->waiting_next!=NULL)
                    pre=pre->waiting_next;
            }
            pre->waiting_next=NULL;
            free(rear);
            rear=NULL;
        }
        
        else break;
        
    }
    
}

void Time_out(PCB self)
{
    PCB q=NULL;
    q=Get_PCB(Running_process);
    // remove from head? yes
    Remove_RL(q);
    q->Status.Type = Ready;
    // insert into tail? yes
    Insert_RL(q);
    Scheduler(self);
}

void Kill_Children(PCB p)
{
    if(p)
    {
        //遍历其所有的子进程及孙子进程
        if(p->Creation_Tree.Children->Children_next!=NULL)
            Kill_Children(p->Creation_Tree.Children->Children_next);
        if(p->Children_next!=NULL)
            Kill_Children(p->Children_next);
        
        
        //删除它的Creation_tree中的所有进程
        p->Creation_Tree.Children=NULL;
        //删除其父进程中的Creation_tree链表中的它
        PCB parent=p->Creation_Tree.Parent;
        PCB pre=parent->Creation_Tree.Children,rear=pre->Children_next;
        while(rear)
        {
            if(strcmp(rear->PID,p->PID)==0)
            {
                pre->Children_next=rear->Children_next;
                break;
            }
            rear=rear->Children_next;
            pre=pre->Children_next;
        }
        
        //Delete processor in block list
        if(p->Status.Type==Blocked)
        {
            PCB pre=BL->block;
            PCB q=BL->block->List_next;
            for(;q!=NULL;q=q->List_next,pre=pre->List_next)
            {
                if(q->PID==p->PID)
                {
                    pre->List_next=q->List_next;
                    break;
                }
            }
        }
        //Delete processor in ready list(including Running process)
        else
        {
            if(p->Priority==2)
            {
                PCB pre,q;
                for(pre=RL->Priority_2,q=RL->Priority_2->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
                {
                    if(q->PID==p->PID)
                    {
                        pre->List_next=q->List_next;
                        break;
                    }
                }
            }
            
            if(p->Priority==1)
            {
                PCB pre,q;
                for(pre=RL->Priority_1,q=RL->Priority_1->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
                {
                    if(q->PID==p->PID)
                    {
                        pre->List_next=q->List_next;
                        break;
                    }
                }
            }
            
            if(p->Priority==0)
            {
                PCB pre,q;
                for(pre=RL->Priority_0,q=RL->Priority_0->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
                {
                    if(q->PID==p->PID)
                    {
                        pre->List_next=q->List_next;
                        break;
                    }
                }
            }
            
        }
        
    }
    
    //free resources
    //和release调用类似的功能
    Resource_head m=p->Resources->resource_next;
    for(int i=4;i>0;i--)
    {
        RCB r=Get_RCB(i);
        Remove_RCB_WL(r, p);
        if(m->number!=0)
        {
            Release_destroy(p,i,m->number);
        }
        m=m->resource_next;
    }
    
}

void Kill_Self(PCB p)
{
    //删除它的Creation_tree中的所有进程
    p->Creation_Tree.Children=NULL;
    //删除其父进程中的Creation_tree链表中的它
    PCB parent=p->Creation_Tree.Parent;
    PCB pre=parent->Creation_Tree.Children,rear=pre->Children_next;
    while(rear)
    {
        if(strcmp(rear->PID,p->PID)==0)
        {
            pre->Children_next=rear->Children_next;
            break;
        }
        rear=rear->Children_next;
        pre=pre->Children_next;
    }
    
    //Delete processor in block list
    if(p->Status.Type==Blocked)
    {
        PCB pre=BL->block;
        PCB q=BL->block->List_next;
        for(;q!=NULL;q=q->List_next,pre=pre->List_next)
        {
            if(q->PID==p->PID)
            {
                pre->List_next=q->List_next;
                break;
            }
        }
    }
    //Delete processor in ready list(including Running process)
    else
    {
        if(p->Priority==2)
        {
            PCB pre,q;
            for(pre=RL->Priority_2,q=RL->Priority_2->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
            {
                if(q->PID==p->PID)
                {
                    pre->List_next=q->List_next;
                    break;
                }
            }
        }
        
        if(p->Priority==1)
        {
            PCB pre,q;
            for(pre=RL->Priority_1,q=RL->Priority_1->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
            {
                if(q->PID==p->PID)
                {
                    pre->List_next=q->List_next;
                    break;
                }
            }
        }
        
        if(p->Priority==0)
        {
            PCB pre,q;
            for(pre=RL->Priority_0,q=RL->Priority_0->List_next;q!=NULL;q=q->List_next,pre=pre->List_next)
            {
                if(q->PID==p->PID)
                {
                    pre->List_next=q->List_next;
                    break;
                }
            }
        }
    }
    //free resources
    //和release调用类似的功能
    Resource_head m=p->Resources->resource_next;
    for(int i=4;i>0;i--)
    {
        RCB r=Get_RCB(i);
        Remove_RCB_WL(r, p);
        if(m->number!=0)
        {
            Release_destroy(p,i,m->number);
        }
        m=m->resource_next;
    }
}

void Kill_Tree(PCB p)
{
    if(p->Creation_Tree.Children->Children_next!=NULL)
        Kill_Children(p->Creation_Tree.Children->Children_next);
    Kill_Self(p);
}

void Destroy (PCB self,char* pid)
{
    PCB q=NULL;
    q=Get_PCB(pid);
    Kill_Tree(q);
    Scheduler(self);
    //调度其他进程执行
}

char * Get_status(PCB p)
{
    char *status;
    if(p->Status.Type==0)
        status="Running";
    if(p->Status.Type==1)
        status="Ready";
    if(p->Status.Type==2)
        status="Blocked";
    return status;
}

void All_process()
{
    printf("\nHere are all processes and their priority and status:\n");
    //优先级为2的队列的头节点
    PCB q2=RL->Priority_2;
    //优先级为1的队列的头节点
    PCB q1=RL->Priority_1;
    //优先级为0的队列的头节点
    PCB q0=RL->Priority_0;
    PCB qb=BL->block;
    
    if(q2->List_next)//若优先级为2的队列不为空
    {
        q2=q2->List_next;
        while(q2)
        {
            printf("PID:%s\n",q2->PID);
            printf("    Priority:%d\n",q2->Priority);
            printf("    Status:%s\n",Get_status(q2));
            //找到当前运行的程序
            q2=q2->List_next;
        }
    }
    //若优先级为1的队列不为空
    if(q1->List_next)
    {
        q1=q1->List_next;
        while(q1)
        {
            printf("PID:%s\n",q1->PID);
            printf("    Priority:%d\n",q1->Priority);
            printf("    Status:%s\n",Get_status(q1));
            //找到当前运行的程序
            q1=q1->List_next;
        }
    }
    //若优先级为0的队列不为空
    if(q0->List_next)
    {
        q0=q0->List_next;
        while(q0)
        {
            printf("PID:%s\n",q0->PID);
            printf("    Priority:%d\n",q0->Priority);
            printf("    Status:%s\n",Get_status(q0));
            //找到当前运行的程序
            q0=q0->List_next;
        }
    }
    //若优先级为0的队列不为空
    if(qb->List_next)
    {
        qb=qb->List_next;
        while(qb)
        {
            printf("PID:%s\n    Status:%s\n",qb->PID,Get_status(qb));
            //找到当前运行的程序
            qb=qb->List_next;
        }
    }
}


void All_resource(RCB R1,RCB R2,RCB R3,RCB R4)
{
    printf("\nHere are all resources and their priority and status:\n");
    printf("RID:%d\n",R1->RID);
    printf("    |Initial number of resource:%d\n",R1->Status.k);
    printf("    |Available number of resource currently:%d\n",R1->Status.u);
    printf("RID:%d\n",R2->RID);
    printf("    |Initial number of resource:%d\n",R2->Status.k);
    printf("    |Available number of resource currently:%d\n",R2->Status.u);
    printf("RID:%d\n",R3->RID);
    printf("    |Initial number of resource:%d\n",R3->Status.k);
    printf("    |Available number of resource currently:%d\n",R3->Status.u);
    printf("RID:%d\n",R4->RID);
    printf("    |Initial number of resource:%d\n",R4->Status.k);
    printf("    |Available number of resource currently:%d\n",R4->Status.u);
}

void Process_info(char *pid)
{
    PCB p=Get_PCB(pid);
    printf("\nHere is the information of process %s:\n",pid);
    printf("    |PID:%s\n",p->PID);
    //resource:
    printf("    |Occupied Resources:");
    Resource_head r=p->Resources->resource_next;
    int i=0;
    while (r!=NULL)
    {
        if(r->number!=0)
        {
            printf("\n        |RID:R%d",r->RID);
            printf("    |Occupied Number of Resource:%d",r->number);
        }
        r=r->resource_next;
        i++;
    }
    if(i==0)
        printf("None\n");
    //type
    printf("    |Status:%s\n",Get_status(p));
    //parent
    printf("    |Parent:%s\n",p->Creation_Tree.Parent->PID);
    //children
    printf("    |Children:");
    PCB children=p->Creation_Tree.Children->Children_next;
    int j=0;
    while (children!=NULL)
    {
        printf("%s ",children->PID);
        children=children->Children_next;
        j++;
    }
    if(j==0)
        printf("None\n");
    //priority
    printf("    |Priority:%d\n",p->Priority);
}

void Resource_info(char *rid)
{
    RCB r;
    if(strcmp(rid, "1")==0) r=R1;
    if(strcmp(rid, "2")==0) r=R2;
    if(strcmp(rid, "3")==0) r=R3;
    if(strcmp(rid, "4")==0) r=R4;
    printf("\nHere is the information of resource %s:\n",rid);
    printf("    |RID:%s\n",rid);
    //status:
    printf("    |Initial number of resource:%d\n",r->Status.k);
    printf("    |Available number of resource currently:%d\n",r->Status.u);
    //waitinglist
    printf("    |Blocked process:");
    Waitinglist_head w=r->Waiting_List;
    int i=0;
    while (w->waiting_next!=NULL)
    {
        w=w->waiting_next;
        printf("\n        |PID:%s",w->PID);
        printf("    |Requested Number of Resource:%d\n",w->number);
        i++;
    }
    if(i==0)
        printf("None\n");
}


int Test_shell(const char * argv[])
{
    FILE *fpRead=fopen(argv[1],"r");
    if(fpRead==NULL)
    {
        printf("hello!");
        return 0;
    }
    
    
    RL->Priority_2->PID=NULL;
    PCB self;
    //init
    PCB init=create_init();
    RL->Priority_2->PID=NULL;
    self=init;
    //Scheduler(init);
    printf("%s ", Running_process);
    
    
    while(!feof(fpRead))
    {
        char name[10000]={0},resource[10000]={0};
        char s[100]={0};
        char number[100]={0};
        fscanf(fpRead, "%s",s);
        
        if(strcmp(s, "cr")==0)
        {
            fscanf(fpRead, "%s %s",name,number);
            self=Create(self,name,atoi(number));
            printf("%s ", Running_process);
        }
        if(strcmp(s, "de")==0)
        {
            fscanf(fpRead, "%s",name);
            self=NULL;
            Destroy(self,name);
            self=Get_PCB(Running_process);
            printf("%s ", Running_process);
        }
        if(strcmp(s, "req")==0)
        {
            fscanf(fpRead, "%s %s",resource,number);
            int resource_number=resource[1]-'0';
            Request(Get_PCB(Running_process), resource_number,atoi(number));
            self=Get_PCB(Running_process);
            printf("%s ", Running_process);
        }
        if(strcmp(s, "rel")==0)
        {
            fscanf(fpRead, "%s %s",resource,number);
            int resource_number=resource[1]-'0';
            Release(Get_PCB(Running_process), resource_number, atoi(number));
            self=Get_PCB(Running_process);
            printf("%s ", Running_process);
        }
        if(strcmp(s, "to")==0)
        {
            Time_out(self);
            self=Get_PCB(Running_process);
            printf("%s ", Running_process);
        }
        if(strcmp(s, "lp")==0)
            All_process();
        if(strcmp(s, "lr")==0)
            All_resource(R1,R2,R3,R4);
        if(strcmp(s, "pinfo")==0)
        {
            fscanf(fpRead, "%s",name);
            Process_info(name);
        }
        if(strcmp(s, "rinfo")==0)
        {
            fscanf(fpRead, "%s",name);
            Resource_info(name);
        }
    }
    fclose(fpRead);
    return 1;
}

int main(int argc, const char * argv[])
{
    if (argc < 2)
        return 0;
    else
        Test_shell(argv);
    return 0;
}
