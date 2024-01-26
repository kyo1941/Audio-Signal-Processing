void file_create(int count)
{
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    int *pf = [ fp1, fp2, fp3, fp4, fp5 ];

    for (int i = 0; i < count; i++)
    {
        char filename[20];
        sprintf(filename, "data", (char)i, ".txt");

        if(!pf[i]) {
            // TODO: 既存ファイルへの再書き込みの操作を実装する。
        }
        pf[i] = fopen(filename, "w");
    }
}