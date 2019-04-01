/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: pepe
 *
 * Created on 2019. március 21., 20:51
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

#include<stdio.h>
 
void create(int [], int);
void down_adjust(int [], int, int);
void heapsort(int[], int );
 
int main(int argc, char** argv) 
{
	int heap[30],n,i,last,temp;
	printf("Enter no. of elements:");
	scanf("%d",&n);
	printf("\nEnter elements:");
	for(i=1;i<=n;i++)
		scanf("%d",&heap[i]);
	
	//create a heap
        
	heapsort(heap, n);
 
	//print sorted data
	printf("\nArray after sorting:\n");
	for(i=1;i<=n;i++)
		printf("%d ",heap[i]);
        return (EXIT_SUCCESS);
}
 
void create(int heap[], int n)
{
	for(int i=n/2;i>=1;i--)
		down_adjust(heap,n,i);
}
 
void down_adjust(int heap[],int n,int i)
{
	int j,temp,flag=1;
	
	while(2 * i <= n && flag==1)
	{
		j=2*i;	//j points to left child
		if(j + 1 <= n && heap[j+1] > heap[j])
			j=j+1;
		if(heap[i] > heap[j])
			flag=0;
		else
		{
			temp=heap[i];
			heap[i]=heap[j];
			heap[j]=temp;
			i=j;
		}
	}
}

void heapsort(int heap[], int n)
{  
    int last; 
    int temp;
    create(heap,n);
    
    //sorting
    while(heap[0] > 1)
    {
        //swap heap[1] and heap[last]
        last=n;
        temp=heap[0];
        heap[0]=heap[last];
        heap[last]=temp;
        n--;
        down_adjust(heap,n,1);
    }
}

