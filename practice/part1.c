#include <stdio.h>
#include <stdlib.h>

int simpleSort(int *input, int array);

int main(void)
{
    int array = 16;
    int input[array];

    printf("before\ninput = [ ");
    for (int i = 0; i < array; i++)
    {
        input[i] = rand() % 10 + 1;
        printf("%d ", input[i]);
        if (i == array - 1)
           printf("]\n");
    }

    int time = simpleSort(input, array);

    printf("after\ninput = [ ");
    for (int i = 0; i < array; i++)
    {
        printf("%d ", input[i]);
        if (i == array - 1)
            printf("]\n");
    }

    printf("time = %d\n", time);
    return 0;
}

int simpleSort(int *input, int array)
{
    int max = -1;
    int maxPoint = 0;
    int temp;
    int time = 0;

    while (array > 0)
    {
        for (int i = 0; i < array; i++)
        {
            time++;
            if (max < input[i])
            {
                max = input[i];
                maxPoint = i;
            }
        }

        temp = input[maxPoint];
        input[maxPoint] = input[array - 1];
        input[array - 1] = temp;
        array--;

        max = -1;
        maxPoint = 0;
    }

    return time;
}
