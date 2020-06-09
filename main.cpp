#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <algorithm>
#include<malloc.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#include <queue>
#include <vector>
#include "io.h"
#include "math.h"
#include "time.h"
#define TRUE 1
#define FALSE 0
#define MAXSIZE 100 /* 存储空间初始分配量 */

typedef int Status; /* Status是函数的类型,其值是函数结果状态代码，如OK等 */

using namespace std;
typedef struct BiTNode
{
    int data;//数据
    int bf;//深度差
    struct BiTNode* lchild, *rchild;
} BiTNode, *BiTree;
//增加结点平衡操作

void L_Rotate(BiTree *T)//左旋转
{
    BiTree R = (*T)->rchild;
    (*T)->rchild = R->lchild;
    R->lchild = *T;
    *T = R;
}

void R_Rotate(BiTree *T)//右旋转
{
    BiTree L = (*T)->lchild;
    (*T)->lchild = L->rchild;
    L->rchild = *T;
    *T = L;
}
#define LH +1
#define EH 0
#define RH -1
//T 的左边高，不平衡，使其平衡，右旋转，右旋转前先检查L->bf，
//如果为RH，L要先进行左旋转，使T->lchild->bf和T->bf一致
/*  对以指针T所指结点为根的二叉树作左平衡旋转处理 */
void LeftBalance(BiTree *T)
{
    BiTree L, Lr;
    L = (*T)->lchild; /*  L指向T的左子树根结点 */
    switch (L->bf)
    {
        /*  检查T的左子树的平衡度，并作相应平衡处理 */
    case LH: /*  新结点插入在T的左孩子的左子树上，要作单右旋处理 */
        (*T)->bf = L->bf = EH;
        R_Rotate(T);
        break;
    case RH: /*  新结点插入在T的左孩子的右子树上，要作双旋处理 */
        Lr = L->rchild; /*  Lr指向T的左孩子的右子树根 */
        switch (Lr->bf)
        {
            /*  修改T及其左孩子的平衡因子 */
        case LH:
            (*T)->bf = RH;
            L->bf = EH;
            break;
        case EH:
            (*T)->bf = L->bf = EH;
            break;
        case RH:
            (*T)->bf = EH;
            L->bf = LH;
            break;
        }
        Lr->bf = EH;
        L_Rotate(&(*T)->lchild); /*  对T的左子树作左旋平衡处理 */
        R_Rotate(T); /*  对T作右旋平衡处理 */
    }
}

/*  对以指针T所指结点为根的二叉树作右平衡旋转处理， */
void RightBalance(BiTree *T)
{
    BiTree R, Rl;
    R = (*T)->rchild; /*  R指向T的右子树根结点 */
    switch (R->bf)
    {
        /*  检查T的右子树的平衡度，并作相应平衡处理 */
    case RH: /*  新结点插入在T的右孩子的右子树上，要作单左旋处理 */
        (*T)->bf = R->bf = EH;
        L_Rotate(T);
        break;
    case LH: /*  新结点插入在T的右孩子的左子树上，要作双旋处理 */
        Rl = R->lchild; /*  Rl指向T的右孩子的左子树根 */
        switch (Rl->bf)
        {
            /*  修改T及其右孩子的平衡因子 */
        case RH:
            (*T)->bf = LH;
            R->bf = EH;
            break;
        case EH:
            (*T)->bf = R->bf = EH;
            break;
        case LH:
            (*T)->bf = EH;
            R->bf = RH;
            break;
        }
        Rl->bf = EH;
        R_Rotate(&(*T)->rchild); /*  对T的右子树作右旋平衡处理 */
        L_Rotate(T); /*  对T作左旋平衡处理 */
    }
}

/*  若在平衡的二叉排序树T中不存在和e有相同关键字的结点，则插入一个 */
/*  数据元素为e的新结点，并返回1，否则返回0。若因插入而使二叉排序树 */
/*  失去平衡，则作平衡旋转处理，taller反映T长高与否。 */

Status InsertAVL(BiTree *T, int e, Status *taller)
{
    if (!*T)  //T为空
    {
        /*  插入新结点，树“长高”，置taller为TRUE */
        *T = (BiTree)malloc(sizeof(BiTNode));
        (*T)->data = e;
        (*T)->lchild = (*T)->rchild = NULL;
        (*T)->bf = EH;
        *taller = TRUE;
    }
    else
    {
        if (e == (*T)->data)
        {
            /*  树中已存在和e有相同关键字的结点则不再插入 */
            printf ("该数已被插入。");
            *taller = FALSE;
            return FALSE;
        }
        if (e < (*T)->data)
        {
            /*  应继续在T的左子树中进行搜索 */
            if (!InsertAVL(&(*T)->lchild, e, taller)) /*  未插入 */
                return FALSE;
            if (*taller) /*   已插入到T的左子树中且左子树“长高” */
                switch ((*T)->bf) /*  检查T的平衡度 */
                {
                case LH: /*  原本左子树比右子树高，需要作左平衡处理 */
                    LeftBalance(T);
                    *taller = FALSE;
                    break;
                case EH: /*  原本左、右子树等高，现因左子树增高而使树增高 */
                    (*T)->bf = LH;
                    *taller = TRUE;
                    break;
                case RH: /*  原本右子树比左子树高，现左、右子树等高 */
                    (*T)->bf = EH;
                    *taller = FALSE;
                    break;
                }
        }
        else
        {
            /*  应继续在T的右子树中进行搜索 */
            if (!InsertAVL(&(*T)->rchild, e, taller)) /*  未插入 */
                return FALSE;
            if (*taller) /*  已插入到T的右子树且右子树“长高” */
                switch ((*T)->bf) /*  检查T的平衡度 */
                {
                case LH: /*  原本左子树比右子树高，现左、右子树等高 */
                    (*T)->bf = EH;
                    *taller = FALSE;
                    break;
                case EH: /*  原本左、右子树等高，现因右子树增高而使树增高  */
                    (*T)->bf = RH;
                    *taller = TRUE;
                    break;
                case RH: /*  原本右子树比左子树高，需要作右平衡处理 */
                    RightBalance(T);
                    *taller = FALSE;
                    break;
                }
        }
    }
    return TRUE;
}
/*递归算法*/
void PreOrderTraverse( BiTree T)
{
    // 前序遍历
    if (T)
    {
        printf ("%d ",T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
} // PreOrderTraverse
void InOrderTraverse( BiTree T )
{
    // 中序遍历
    if (T)
    {
        InOrderTraverse(T->lchild);
        printf ("%d ",T->data);
        InOrderTraverse(T->rchild);
    }
} // InOrderTraverse
void PostOrderTraverse( BiTree T)
{
    // 后序遍历
    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf ("%d ",T->data);
    }
} // PostOrderTraverse
/*递归算法*/

/*非递归算法*/
void PreorderTraverse(BiTree T)//先序遍历
{
    stack<BiTree>S;
    BiTree p,q;
    p=T;
    q=new BiTNode;
    while (p||!S.empty())
    {
        if (p)
        {
            S.push(p);
            printf("%d ",p->data);
            p=p->lchild;
        }
        else
        {
            q=S.top();
            S.pop();
            p=q->rchild;
        }
    }
}

void InorderTraverse(BiTree T)//中序遍历
{
    stack<BiTree>S;
    BiTree p,q;
    p=T;
    q=new BiTNode;
    while (p||!S.empty())
    {
        if (p)
        {
            S.push(p);
            p=p->lchild;
        }
        else
        {
            q=S.top();
            S.pop();
            printf("%d ",q->data);
            p=q->rchild;
        }
    }
}
void PostorderTraverse(BiTree T)//后序遍历
{
    stack<BiTree>S;
    BiTree p,q;
    p=T;
    q=new BiTNode;
    S.push(p);
    while (!S.empty())
    {
        q=S.top();
        if((q->lchild==NULL && q->rchild==NULL) ||(p!=NULL && (q->lchild==p || q->rchild==p)))
        {
            //如果当前节点没有左右孩子，或者有左孩子或有孩子，但已经被访问输出，
            //则直接输出该节点，将其出栈，将其设为上一个访问的节点
            printf("%d ", q->data);
            S.pop();
            p = q;
        }
        else
        {
            //如果不满足上面两种情况,则将其右孩子左孩子依次入栈
            if(q->rchild != NULL)
                S.push(q->rchild);
            if(q->lchild != NULL)
                S.push(q->lchild);
        }
    }
}
/*非递归算法*/
int BiTreeDepth(BiTree T)  //深度
{
    int i,j;
    if(!T)
        return 0;
    if(T->lchild)
        i=BiTreeDepth(T->lchild); // 左子树深度
    else
        i=0;
    if(T->rchild)
        j=BiTreeDepth(T->rchild);  // 右子树深度
    else
        j=0;
    return i>j?i+1:j+1;
}

void print_by_level_3(BiTree T)//层次遍历
{
    queue<BiTree> q;
    q.push(T);
    while (!q.empty())//不为空
    {
        T=q.front();
        if (T->lchild)
        {
            q.push(T->lchild);
        }
        if (T->rchild)
        {
            q.push(T->rchild);
        }
        printf ("%d ",T->data);
        q.pop();
    }
}
int Treeleaf(BiTree T,int &n)  //叶子节点数
{
    if(T)//非空树
    {
        if(!T->lchild&&!T->rchild)//当结点左右儿子为空时为叶子结点
            n++;//数量增加
        Treeleaf(T->lchild,n);
        Treeleaf(T->rchild,n);
    }
}

void ReverseLeftRightChild(BiTree T)// 交换左右子树
{
    // 如果是叶子节点，则递归结束
    if (T == NULL)
    {
        return;
    }
    swap((T)->lchild, (T)->rchild); // 直接交换指针；
    ReverseLeftRightChild(&(*T->lchild));
    ReverseLeftRightChild(&(*T->rchild));
}

int Searchdata(BiTree T,int e)//查找关键字
{
    if (T==NULL) return FALSE;
    if (T->data>e) Searchdata(T->lchild,e);
    else if (T->data<e) Searchdata(T->rchild,e);
    else if (T->data==e) return TRUE;
}

//删除平衡操作
void LeftBalance_div(BiTree &T,bool &shorter)//删除结点时左平衡旋转操作
{
    if(T->bf == LH)//删除前左子树比右子树高，树变矮后恢复平衡
    {
        T->bf = EH;
        shorter = true;
    }
    else if(T->bf == EH)//删除前左右子树平衡，现在右子树比左子树高
    {
        T->bf = RH;
        shorter = false;
    }
    else//*T的右子树高
    {
        BiTree T1 = T->rchild;
        if(T1->bf == EH)//T1结点的左右子树等高，删除结点后*T的bf为-2，进行左旋操作，RR型
        {
            T1->bf = LH;
            T->bf = RH;
            L_Rotate(&T);
            shorter = false;
        }
        else if(T1->bf == RH)//删除结点前右边比左边高，需要左旋操作，RR型
        {
            T1->bf = T->bf = EH;
            L_Rotate(&T);
            shorter = true;
        }
        else//T1的左子树高，双旋操作，LR型
        {
            BiTree T2 = T1->lchild;
            T1->lchild = T2->rchild;
            T2->rchild = T1;
            T->rchild = T2->lchild;
            T2->lchild = T;
            if(T2->bf == EH)
                T->bf = T1->bf =EH;
            else if(T2->bf == LH)
            {
                T->bf = RH;
                T1->bf = EH;
            }
            else
            {
                T->bf = EH;
                T1->bf = LH;
            }
            T2->bf = EH;
            T = T2;
            shorter = true;
        }
    }

}

void RightBalance_div(BiTree &T,bool &shorter)//删除结点时右平衡旋转操作
{
    if(T->bf == RH)//*T的右子树比左子树高，现平衡
    {
        T->bf = EH;
        shorter = true;
    }
    else if(T->bf == EH)
    {
        T->bf = LH;
        shorter = false;
    }
    else
    {
        BiTree T1 = T->lchild;
        if(T1->bf == LH)
        {
            T1->bf = T->bf = EH;
            R_Rotate(&T);
            shorter = true;
        }
        else if(T1->bf == EH)
        {
            T1->bf = LH;
            T->bf = RH;
            R_Rotate(&T);
            shorter = true;
        }
        else
        {
            BiTree T2 = T1->rchild;
            T1->rchild = T2->lchild;
            T2->lchild = T1;
            T->lchild = T2->rchild;
            T2->rchild = T;
            if(T2->bf == LH)
            {
                T->bf = EH;
                T1->bf = RH;
            }
            else if(T2->bf == EH)
            {
                T->bf = T1->bf = EH;
            }
            else
            {
                T->bf = LH;
                T1->bf = EH;
            }
            T2->bf = EH;
            T = T2;
            shorter = true;
        }
    }
}
void Delete(BiTree T,BiTree &t,bool &shorter)//当删除结点左右子树都不为空的删除操作
{
    if(!t->rchild)
    {
        T->data = t->data;
        T = t;
        t = t->lchild;
        free(T);
        shorter = true;
    }
    else
    {
        Delete(T,t->rchild,shorter);
        if(shorter)
            RightBalance_div(t,shorter);
    }
}

bool DeleteNode(BiTree &T, int key, bool &shorter)//删除结点
{
    bool f;
    if(!T)
    {
        printf("不存在相关关键字\n");
        return false;
    }
    else
    {
        if(key > T->data)
        {
            f=DeleteNode(T->rchild,key,shorter);
            if(shorter)//删除成功
                RightBalance_div(T,shorter);
            if(f)
                return true;
            else//删除失败
                return false;
        }
        else if(key < T->data)
        {
            f=DeleteNode(T->lchild,key,shorter);
            if(shorter)//删除成功
                LeftBalance_div(T,shorter);
            if(f)
                return true;
            else//删除失败
                return false;
        }
        else
        {
            BiTree t = T;
            if(T->lchild == NULL)//左子树为空
            {
                T = T->rchild;
                free(T);
                shorter = true;
            }
            else if(T->rchild == NULL)//右子树为空
            {
                T = T->lchild;
                free(t);
                shorter = true;
            }
            else//左右子树不为空
            {
                Delete(t,t->lchild,shorter);//左子树的最右节点放到那里去
                if(shorter)
                    LeftBalance_div(t,shorter);
                T = t;
            }
            return true;
        }
    }
}

int main()
{
    int i,x;
    //InsertAVL插入
    BiTree T = NULL;
    while (1)
    {
        Status taller;
        bool shorter=false;
        printf  ("[0]退出\n[1]插入数据\n[2]删除结点\n[3]先序遍历（递归）\n[4]中序遍历（递归）\n[5]后序遍历（递归）\n[6]先序遍历（非递归）\n");
        printf ("[7]中序遍历（非递归）\n[8]后序遍历（非递归）\n[9]层次遍历\n[10]查找关键字\n[11]交换左右子树\n[12]树的深度\n[13]叶子结点数\n");
        printf ("请输入您的操作：");
        scanf ("%d",&i);
        if (i==1)
        {
            printf ("请输入您要加入的数字：");
            scanf ("%d",&x);
            if (InsertAVL(&T,x,&taller)) printf ("加入成功\n");
            system("pause");
            system("cls");
        }
        else if (i==2)
        {
            printf ("请输入您想要删除的数字：");
            scanf ("%d",&x);
            if (DeleteNode(T,x,shorter)) printf ("删除成功\n");
            system("pause");
            system("cls");
        }
        else if (i==3)
        {
            PreOrderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==4)
        {
            InOrderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==5)
        {
            PostOrderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==6)
        {
            PreorderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==7)
        {
            InorderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==8)
        {
            PostorderTraverse(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==9)
        {
            print_by_level_3(T);
            printf ("\n");
            system("pause");
            system("cls");
        }
        else if (i==10)
        {
            printf ("请输入您要查找的数字：");
            scanf ("%d",&x);
            if (Searchdata(T,x))
                printf ("查找成功\n");
            else printf ("查找失败\n");
            system("pause");
            system("cls");
        }
        else if (i==11)
        {
            ReverseLeftRightChild(T);
            printf ("交换成功\n");
            //else printf ("交换失败\n");
            system("pause");
            system("cls");
        }
        else if (i==12)
        {
            printf ("树的深度为%d\n",BiTreeDepth(T));
            system("pause");
            system("cls");
        }
        else if (i==13)
        {
            int n=0;
            Treeleaf(T,n);
            printf ("树的叶子结点数为%d\n",n);
            system("pause");
            system("cls");
        }
        else if (i==0)
            return 0;
        else
        {
            printf ("请输入正确编号\n");
            system("pause");
            system("cls");
        }
    }
    return 0;
}
