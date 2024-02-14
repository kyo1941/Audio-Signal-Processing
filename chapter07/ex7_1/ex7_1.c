#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  long n, m, i, I, J;
  double fc, Q, g, a[3], b[3], A[3][3], B[3][3];
  
  mono_wave_read(&pcm0, "sample04.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  
  I = 2; /* �x����̐� */
  J = 2; /* �x����̐� */
  
  fc = 500.0 / pcm0.fs; /* �Ւf���g�� */
  Q = 1.0 / sqrt(2.0); /* �N�I���e�B�t�@�N�^ */
  g = -1.0;
  IIR_low_shelving(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[0][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[0][m] = b[m];
  }
  
  fc = 1000.0 / pcm0.fs; /* ���S���g�� */
  Q = 1.0 / sqrt(2.0); /* �N�I���e�B�t�@�N�^ */
  g = 1.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[1][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[1][m] = b[m];
  }
  
  fc = 2000.0 / pcm0.fs; /* �Ւf���g�� */
  Q = 1.0 / sqrt(2.0); /* �N�I���e�B�t�@�N�^ */
  g = -1.0;
  IIR_high_shelving(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[2][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[2][m] = b[m];
  }
  
  /* �C�R���C�U�i3�ш�j */
  for (i = 0; i < 3; i++)
  {
    for (n = 0; n < pcm1.length; n++)
    {
      pcm1.s[n] = 0.0;
    }
    
    for (n = 0; n < pcm1.length; n++)
    {
      for (m = 0; m <= J; m++)
      {
        if (n - m >= 0)
        {
          pcm1.s[n] += B[i][m] * pcm0.s[n - m];
        }
      }
      for (m = 1; m <= I; m++)
      {
        if (n - m >= 0)
        {
          pcm1.s[n] += -A[i][m] * pcm1.s[n - m];
        }
      }
    }
    
    for (n = 0; n < pcm1.length; n++)
    {
      pcm0.s[n] = pcm1.s[n];
    }
  }
  
  mono_wave_write(&pcm1, "ex7_1.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */
  
  free(pcm0.s); /* �������̉�� */
  free(pcm1.s); /* �������̉�� */
  
  return 0;
}
