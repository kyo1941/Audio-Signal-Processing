#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  int n;
  double gain, level;
  
  mono_wave_read(&pcm0, "sample02.wav"); /* WAVEファイルからモノラルの音データを入力 */
  
  pcm1.fs = pcm0.fs;                            /* 標本化周波数 */
  pcm1.bits = pcm0.bits;                        /* 量子化精度 */
  pcm1.length = pcm0.length;                    /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /*　メモリの確保 */
  
  gain = 10.0; /* ������ */
  level = 0.5; /* ���x�� */
  
  for (n = 0; n < pcm1.length; n++)
  {
    if (pcm0.s[n] < 0.0)
    {
      pcm0.s[n] *= -1.0; /* ���f�[�^�̑S�g���� */
    }
    
    pcm1.s[n] = pcm0.s[n] * gain; /* ���f�[�^�̑��� */
    
    if (pcm1.s[n] > 1.0)
    {
      pcm1.s[n] = 1.0;  /* �N���b�s���O */
    }
    else if (pcm1.s[n] < -1.0)
    {
      pcm1.s[n] = -1.0; /* �N���b�s���O */
    }
    
    pcm1.s[n] *= level; /* ���̑傫���𒲐߂��� */
  }
  
  mono_wave_write(&pcm1, "ex4_3.wav"); /* WAVEファイルにモノラルの音データを出力する */
  
  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */
  
  return 0;
}
