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
#define MAXSIZE 100 /* �洢�ռ��ʼ������ */

typedef int Status; /* Status�Ǻ���������,��ֵ�Ǻ������״̬���룬��OK�� */

using namespace std;
typedef struct BiTNode
{
    int data;//����
    int bf;//��Ȳ�
    struct BiTNode* lchild, *rchild;
} BiTNode, *BiTree;
//���ӽ��ƽ�����

void L_Rotate(BiTree *T)//����ת
{
    BiTree R = (*T)->rchild;
    (*T)->rchild = R->lchild;
    R->lchild = *T;
    *T = R;
}

void R_Rotate(BiTree *T)//����ת
{
    BiTree L = (*T)->lchild;
    (*T)->lchild = L->rchild;
    L->rchild = *T;
    *T = L;
}
#define LH +1
#define EH 0
#define RH -1
//T ����߸ߣ���ƽ�⣬ʹ��ƽ�⣬����ת������תǰ�ȼ��L->bf��
//���ΪRH��LҪ�Ƚ�������ת��ʹT->lchild->bf��T->bfһ��
/*  ����ָ��T��ָ���Ϊ���Ķ���������ƽ����ת���� */
void LeftBalance(BiTree *T)
{
    BiTree L, Lr;
    L = (*T)->lchild; /*  Lָ��T������������� */
    switch (L->bf)
    {
        /*  ���T����������ƽ��ȣ�������Ӧƽ�⴦�� */
    case LH: /*  �½�������T�����ӵ��������ϣ�Ҫ������������ */
        (*T)->bf = L->bf = EH;
        R_Rotate(T);
        break;
    case RH: /*  �½�������T�����ӵ��������ϣ�Ҫ��˫������ */
        Lr = L->rchild; /*  Lrָ��T�����ӵ��������� */
        switch (Lr->bf)
        {
            /*  �޸�T�������ӵ�ƽ������ */
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
        L_Rotate(&(*T)->lchild); /*  ��T��������������ƽ�⴦�� */
        R_Rotate(T); /*  ��T������ƽ�⴦�� */
    }
}

/*  ����ָ��T��ָ���Ϊ���Ķ���������ƽ����ת���� */
void RightBalance(BiTree *T)
{
    BiTree R, Rl;
    R = (*T)->rchild; /*  Rָ��T������������� */
    switch (R->bf)
    {
        /*  ���T����������ƽ��ȣ�������Ӧƽ�⴦�� */
    case RH: /*  �½�������T���Һ��ӵ��������ϣ�Ҫ������������ */
        (*T)->bf = R->bf = EH;
        L_Rotate(T);
        break;
    case LH: /*  �½�������T���Һ��ӵ��������ϣ�Ҫ��˫������ */
        Rl = R->lchild; /*  Rlָ��T���Һ��ӵ��������� */
        switch (Rl->bf)
        {
            /*  �޸�T�����Һ��ӵ�ƽ������ */
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
        R_Rotate(&(*T)->rchild); /*  ��T��������������ƽ�⴦�� */
        L_Rotate(T); /*  ��T������ƽ�⴦�� */
    }
}

/*  ����ƽ��Ķ���������T�в����ں�e����ͬ�ؼ��ֵĽ�㣬�����һ�� */
/*  ����Ԫ��Ϊe���½�㣬������1�����򷵻�0����������ʹ���������� */
/*  ʧȥƽ�⣬����ƽ����ת����taller��ӳT������� */

Status InsertAVL(BiTree *T, int e, Status *taller)
{
    if (!*T)  //TΪ��
    {
        /*  �����½�㣬�������ߡ�����tallerΪTRUE */
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
            /*  �����Ѵ��ں�e����ͬ�ؼ��ֵĽ�����ٲ��� */
            printf ("�����ѱ����롣");
            *taller = FALSE;
            return FALSE;
        }
        if (e < (*T)->data)
        {
            /*  Ӧ������T���������н������� */
            if (!InsertAVL(&(*T)->lchild, e, taller)) /*  δ���� */
                return FALSE;
            if (*taller) /*   �Ѳ��뵽T�����������������������ߡ� */
                switch ((*T)->bf) /*  ���T��ƽ��� */
                {
                case LH: /*  ԭ�����������������ߣ���Ҫ����ƽ�⴦�� */
                    LeftBalance(T);
                    *taller = FALSE;
                    break;
                case EH: /*  ԭ�����������ȸߣ��������������߶�ʹ������ */
                    (*T)->bf = LH;
                    *taller = TRUE;
                    break;
                case RH: /*  ԭ�����������������ߣ������������ȸ� */
                    (*T)->bf = EH;
                    *taller = FALSE;
                    break;
                }
        }
        else
        {
            /*  Ӧ������T���������н������� */
            if (!InsertAVL(&(*T)->rchild, e, taller)) /*  δ���� */
                return FALSE;
            if (*taller) /*  �Ѳ��뵽T���������������������ߡ� */
                switch ((*T)->bf) /*  ���T��ƽ��� */
                {
                case LH: /*  ԭ�����������������ߣ������������ȸ� */
                    (*T)->bf = EH;
                    *taller = FALSE;
                    break;
                case EH: /*  ԭ�����������ȸߣ��������������߶�ʹ������  */
                    (*T)->bf = RH;
                    *taller = TRUE;
                    break;
                case RH: /*  ԭ�����������������ߣ���Ҫ����ƽ�⴦�� */
                    RightBalance(T);
                    *taller = FALSE;
                    break;
                }
        }
    }
    return TRUE;
}
/*�ݹ��㷨*/
void PreOrderTraverse( BiTree T)
{
    // ǰ�����
    if (T)
    {
        printf ("%d ",T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
} // PreOrderTraverse
void InOrderTraverse( BiTree T )
{
    // �������
    if (T)
    {
        InOrderTraverse(T->lchild);
        printf ("%d ",T->data);
        InOrderTraverse(T->rchild);
    }
} // InOrderTraverse
void PostOrderTraverse( BiTree T)
{
    // �������
    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf ("%d ",T->data);
    }
} // PostOrderTraverse
/*�ݹ��㷨*/

/*�ǵݹ��㷨*/
void PreorderTraverse(BiTree T)//�������
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

void InorderTraverse(BiTree T)//�������
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
void PostorderTraverse(BiTree T)//�������
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
            //�����ǰ�ڵ�û�����Һ��ӣ����������ӻ��к��ӣ����Ѿ������������
            //��ֱ������ýڵ㣬�����ջ��������Ϊ��һ�����ʵĽڵ�
            printf("%d ", q->data);
            S.pop();
            p = q;
        }
        else
        {
            //��������������������,�����Һ�������������ջ
            if(q->rchild != NULL)
                S.push(q->rchild);
            if(q->lchild != NULL)
                S.push(q->lchild);
        }
    }
}
/*�ǵݹ��㷨*/
int BiTreeDepth(BiTree T)  //���
{
    int i,j;
    if(!T)
        return 0;
    if(T->lchild)
        i=BiTreeDepth(T->lchild); // ���������
    else
        i=0;
    if(T->rchild)
        j=BiTreeDepth(T->rchild);  // ���������
    else
        j=0;
    return i>j?i+1:j+1;
}

void print_by_level_3(BiTree T)//��α���
{
    queue<BiTree> q;
    q.push(T);
    while (!q.empty())//��Ϊ��
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
int Treeleaf(BiTree T,int &n)  //Ҷ�ӽڵ���
{
    if(T)//�ǿ���
    {
        if(!T->lchild&&!T->rchild)//��������Ҷ���Ϊ��ʱΪҶ�ӽ��
            n++;//��������
        Treeleaf(T->lchild,n);
        Treeleaf(T->rchild,n);
    }
}

void ReverseLeftRightChild(BiTree T)// ������������
{
    // �����Ҷ�ӽڵ㣬��ݹ����
    if (T == NULL)
    {
        return;
    }
    swap((T)->lchild, (T)->rchild); // ֱ�ӽ���ָ�룻
    ReverseLeftRightChild(&(*T->lchild));
    ReverseLeftRightChild(&(*T->rchild));
}

int Searchdata(BiTree T,int e)//���ҹؼ���
{
    if (T==NULL) return FALSE;
    if (T->data>e) Searchdata(T->lchild,e);
    else if (T->data<e) Searchdata(T->rchild,e);
    else if (T->data==e) return TRUE;
}

//ɾ��ƽ�����
void LeftBalance_div(BiTree &T,bool &shorter)//ɾ�����ʱ��ƽ����ת����
{
    if(T->bf == LH)//ɾ��ǰ���������������ߣ����䰫��ָ�ƽ��
    {
        T->bf = EH;
        shorter = true;
    }
    else if(T->bf == EH)//ɾ��ǰ��������ƽ�⣬��������������������
    {
        T->bf = RH;
        shorter = false;
    }
    else//*T����������
    {
        BiTree T1 = T->rchild;
        if(T1->bf == EH)//T1�������������ȸߣ�ɾ������*T��bfΪ-2����������������RR��
        {
            T1->bf = LH;
            T->bf = RH;
            L_Rotate(&T);
            shorter = false;
        }
        else if(T1->bf == RH)//ɾ�����ǰ�ұ߱���߸ߣ���Ҫ����������RR��
        {
            T1->bf = T->bf = EH;
            L_Rotate(&T);
            shorter = true;
        }
        else//T1���������ߣ�˫��������LR��
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

void RightBalance_div(BiTree &T,bool &shorter)//ɾ�����ʱ��ƽ����ת����
{
    if(T->bf == RH)//*T�����������������ߣ���ƽ��
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
void Delete(BiTree T,BiTree &t,bool &shorter)//��ɾ�����������������Ϊ�յ�ɾ������
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

bool DeleteNode(BiTree &T, int key, bool &shorter)//ɾ�����
{
    bool f;
    if(!T)
    {
        printf("��������عؼ���\n");
        return false;
    }
    else
    {
        if(key > T->data)
        {
            f=DeleteNode(T->rchild,key,shorter);
            if(shorter)//ɾ���ɹ�
                RightBalance_div(T,shorter);
            if(f)
                return true;
            else//ɾ��ʧ��
                return false;
        }
        else if(key < T->data)
        {
            f=DeleteNode(T->lchild,key,shorter);
            if(shorter)//ɾ���ɹ�
                LeftBalance_div(T,shorter);
            if(f)
                return true;
            else//ɾ��ʧ��
                return false;
        }
        else
        {
            BiTree t = T;
            if(T->lchild == NULL)//������Ϊ��
            {
                T = T->rchild;
                free(T);
                shorter = true;
            }
            else if(T->rchild == NULL)//������Ϊ��
            {
                T = T->lchild;
                free(t);
                shorter = true;
            }
            else//����������Ϊ��
            {
                Delete(t,t->lchild,shorter);//�����������ҽڵ�ŵ�����ȥ
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
    //InsertAVL����
    BiTree T = NULL;
    while (1)
    {
        Status taller;
        bool shorter=false;
        printf  ("[0]�˳�\n[1]��������\n[2]ɾ�����\n[3]����������ݹ飩\n[4]����������ݹ飩\n[5]����������ݹ飩\n[6]����������ǵݹ飩\n");
        printf ("[7]����������ǵݹ飩\n[8]����������ǵݹ飩\n[9]��α���\n[10]���ҹؼ���\n[11]������������\n[12]�������\n[13]Ҷ�ӽ����\n");
        printf ("���������Ĳ�����");
        scanf ("%d",&i);
        if (i==1)
        {
            printf ("��������Ҫ��������֣�");
            scanf ("%d",&x);
            if (InsertAVL(&T,x,&taller)) printf ("����ɹ�\n");
            system("pause");
            system("cls");
        }
        else if (i==2)
        {
            printf ("����������Ҫɾ�������֣�");
            scanf ("%d",&x);
            if (DeleteNode(T,x,shorter)) printf ("ɾ���ɹ�\n");
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
            printf ("��������Ҫ���ҵ����֣�");
            scanf ("%d",&x);
            if (Searchdata(T,x))
                printf ("���ҳɹ�\n");
            else printf ("����ʧ��\n");
            system("pause");
            system("cls");
        }
        else if (i==11)
        {
            ReverseLeftRightChild(T);
            printf ("�����ɹ�\n");
            //else printf ("����ʧ��\n");
            system("pause");
            system("cls");
        }
        else if (i==12)
        {
            printf ("�������Ϊ%d\n",BiTreeDepth(T));
            system("pause");
            system("cls");
        }
        else if (i==13)
        {
            int n=0;
            Treeleaf(T,n);
            printf ("����Ҷ�ӽ����Ϊ%d\n",n);
            system("pause");
            system("cls");
        }
        else if (i==0)
            return 0;
        else
        {
            printf ("��������ȷ���\n");
            system("pause");
            system("cls");
        }
    }
    return 0;
}
