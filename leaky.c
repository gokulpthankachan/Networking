#include<stdio.h>

void main()
{
    int incoming, outgoing, buck_size, n, store=0;
    printf("Enter bucket size : ");
    scanf("%d", &buck_size);
    printf("Enter outgoing rate : ");
    scanf("%d", &outgoing);
    printf("Enter the number of inputs :");
    scanf("%d", &n);

    while(n!=0)
    {
        printf("\nEnter the incoming size : ");
        scanf("%d", &incoming);
        if(incoming<=(buck_size-store))
        {
            store+=incoming;
            printf("\nBucket buffer size %d out of %d", store, buck_size);
        }
        else
        {
            printf("\nDropped %d number of packets",incoming-(buck_size-store));
            printf("\nBucket buffer size %d out of %d", store, buck_size);
            store=buck_size;
        }
        store-=outgoing;
        printf("\nAfter outgoing, %d packets left out of %d in buffer\n", store, buck_size);
        n--;
    }
}
