#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  long n, m, i, I, J;
  double fc, Q, g, a[3], b[3], A[10][3], B[10][3];
  
  mono_wave_read(&pcm0, "sample04.wav"); /* WAVE�t�@�C�����烂�m�����̉��f�[�^����͂��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* �������̊m�� */
  
  I = 2; /* �x����̐� */
  J = 2; /* �x����̐� */
  
  fc = 31.25 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 0.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[0][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[0][m] = b[m];
  }
  
  fc = 62.5 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 2.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[1][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[1][m] = b[m];
  }
  
  fc = 125.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 2.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[2][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[2][m] = b[m];
  }
  
  fc = 250.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 0.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[3][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[3][m] = b[m];
  }
  
  fc = 500.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = -0.5;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[4][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[4][m] = b[m];
  }
  
  fc = 1000.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = -0.5;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[5][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[5][m] = b[m];
  }
  
  fc = 2000.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = -1.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[6][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[6][m] = b[m];
  }
  
  fc = 4000.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = -1.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[7][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[7][m] = b[m];
  }
  
  fc = 8000.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 8.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[8][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[8][m] = b[m];
  }
  
  fc = 16000.0 / pcm0.fs; /* ���S���g�� */
  Q = 2.0; /* �N�I���e�B�t�@�N�^ */
  g = 8.0;
  IIR_peaking(fc, Q, g, a, b); /* IIR�t�B���^�̐݌v */
  for (m = 0; m <= I; m++)
  {
    A[9][m] = a[m];
  }
  for (m = 0; m <= J; m++)
  {
    B[9][m] = b[m];
  }
  
  /* �C�R���C�U�i10�ш�j */
  for (i = 0; i < 10; i++)
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
  
  mono_wave_write(&pcm1, "ex7_2.wav"); /* WAVE�t�@�C���Ƀ��m�����̉��f�[�^���o�͂��� */
  
  free(pcm0.s); /* �������̉�� */
  free(pcm1.s); /* �������̉�� */
  
  return 0;
}
