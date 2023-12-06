#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm0;
  int n, k, N;
  double *x_real, *x_imag, *X_real, *X_imag;
  double W_real, W_imag;

  FILE *fp1, *fp2;

  fp1 = fopen("data1.txt", "w");
  fp2 = fopen("data2.txt", "w");

  mono_wave_read(&pcm0, "ex2_1.wav"); /* WAVEファイルからモノラルの音データを入力する */

  N = 64;                             /* DFTのサイズ */
  x_real = calloc(N, sizeof(double)); /* メモリの確保 */
  x_imag = calloc(N, sizeof(double)); /* メモリの確保 */
  X_real = calloc(N, sizeof(double)); /* メモリの確保 */
  X_imag = calloc(N, sizeof(double)); /* メモリの確保 */

  for (n = 0; n < N; n++)
  {
    x_real[n] = pcm0.s[n]; /* x(n)の実数部 */
    x_imag[n] = 0.0;       /* x(n)の虚数部 */
  }

  /* DFT */
  for (k = 0; k < N; k++)
  {
    for (n = 0; n < N; n++)
    {
      W_real = cos(2.0 * M_PI * k * n / N);
      W_imag = -sin(2.0 * M_PI * k * n / N);
      X_real[k] += W_real * x_real[n] - W_imag * x_imag[n]; /* X(k)の実数部 */
      X_imag[k] += W_real * x_imag[n] + W_imag * x_real[n]; /* X(k)の虚数部 */
    }
    fprintf(fp1, "%d %f\n", k, sqrt(pow(X_real[k], 2) + pow(X_imag[k], 2))); /* 振幅スペクトル */
    fprintf(fp2, "%d %f\n", k, asin(X_imag[k]));                             /* 位相スペクトル（ 誤差の影響で、k=2,62の時の値がnanになる。正しくはそれぞれ-π/2, π/2 ） */
  }

  /* 周波数特性 */
  for (k = 0; k < N; k++)
  {
    printf("%d %f+j%f\n", k, X_real[k], X_imag[k]);
  }

  free(pcm0.s); /* メモリの解放 */
  free(x_real); /* メモリの解放 */
  free(x_imag); /* メモリの解放 */
  free(X_real); /* メモリの解放 */
  free(X_imag); /* メモリの解放 */

  fclose(fp1);
  fclose(fp2);

  return 0;
}