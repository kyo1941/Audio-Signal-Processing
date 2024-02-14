#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  int n, m, d, I, J;
  double fc, Q, depth, rate, a[3], b[3];
  
  mono_wave_read(&pcm0, "sample04.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  
  I = 2; /* �x����̐� */
  J = 2; /* �x����̐� */
  
  depth = 800.0; /* 800Hz */
  rate = 1.0; /* 1Hz */
  
  /* ���E */
  for (n = 0; n < pcm1.length; n++)
  {
    fc = (1000.0 + depth * sin(2.0 * M_PI * rate * n / pcm1.fs)) / pcm1.fs; /* ���S���g�� */
    Q = 2.0; /* �N�I���e�B�t�@�N�^ */
    
    IIR_resonator(fc, Q, a, b); /* IIR�t�B���^�̐݌v */
    
    for (m = 0; m <= J; m++)
    {
      if (n - m >= 0)
      {
        pcm1.s[n] += b[m] * pcm0.s[n - m];
      }
    }
    for (m = 1; m <= I; m++)
    {
      if (n - m >= 0)
      {
        pcm1.s[n] += -a[m] * pcm1.s[n - m];
      }
    }
  }
  
  mono_wave_write(&pcm1, "ex7_3.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */
  
  free(pcm0.s); /* �������̉�� */
  free(pcm1.s); /* �������̉�� */
  
  return 0;
}
