#ifndef Header_h
#define Header_h

typedef struct resource_node
{
    int RID;
    int number;
    struct resource_node * resource_next;
}resource_node,*Resource_head;

typedef struct waitinglist_node
{
    char * PID;
    int number;
    struct waitinglist_node * waiting_next;
}waitinglist_node,*Waitinglist_head;

typedef struct pcb
{
    char *PID;
    Resource_head Resources;
    struct status
    {
        int Type;
        void * List;
    }Status;
    struct creation_tree
    {
        pcb *Parent;
        pcb *Children;
    }Creation_Tree;
    int Priority;
    pcb *List_next;
    pcb *Children_next;
}pcbnode,*PCB;

typedef struct rcb
{
    int RID;
    struct status
    {
        int k;//Initial number of resource
        int u;//available number of resource currently
    }Status;
    Waitinglist_head Waiting_List;
}rcbnode,*RCB;

typedef struct readylist
{
    PCB Priority_2;
    PCB Priority_1;
    PCB Priority_0;
}readylist,*ReadyList_head;

typedef struct blocklist
{
    PCB block;
}blocklist,*BlockList_head;


RCB init_RCB(int rid)
{
    RCB r=NULL;
    r=(RCB)malloc(sizeof(rcbnode));
    r->RID=rid;
    r->Status.k=rid;
    r->Status.u=rid;
    r->Waiting_List=(Waitinglist_head)malloc(sizeof(waitinglist_node));
    r->Waiting_List->waiting_next=NULL;
    return r;
}

ReadyList_head init_ReadyList()
{
    ReadyList_head l=(ReadyList_head)malloc(sizeof(readylist));
    l->Priority_2=(PCB)malloc(sizeof(pcbnode));
    l->Priority_2->List_next=NULL;
    l->Priority_2->PID=NULL;
    l->Priority_1=(PCB)malloc(sizeof(pcbnode));
    l->Priority_1->List_next=NULL;
    l->Priority_0=(PCB)malloc(sizeof(pcbnode));
    l->Priority_0->List_next=NULL;
    return l;
}

BlockList_head init_BlockList()
{
    BlockList_head l=(BlockList_head)malloc(sizeof(blocklist));
    l->block=(PCB)malloc(sizeof(pcbnode));
    l->block->List_next=NULL;
    return l;
}

#endif /* Header_h */


