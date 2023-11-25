#include <stdio.h>
#include <stdlib.h>
// #include <math.h>

int comeNum(int l, int r, int *a)
{
    int sum = 0;

    for (int i = l; i <= r; i++)
    {
        sum += a[i];
    }

    return sum;
}

int main(void)
{
    int n, q;
    int *a, *l, *r;

    scanf("%d %d", &n, &q);

    a = (int *)malloc(n * sizeof(int));
    l = (int *)malloc(q * sizeof(int));
    r = (int *)malloc(q * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    for (int i = 0; i < q; i++)
    {
        scanf("%d %d", &l[i], &r[i]);
    }

    for (int i = 0; i < q; i++)
    {
        printf("%d\n", comeNum(l[i]-1, r[i]-1, a));
    }

    free(a);
    free(l);
    free(r);

    return 0;
}

/* A07から */