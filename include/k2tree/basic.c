#include "basic.h"

const unsigned char __popcount_tab[] = {
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
};

const unsigned char select_tab[] = {
  0,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  6,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  7,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  6,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  8,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  6,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  7,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
  6,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,5,1,2,1,3,1,2,1,4,1,2,1,3,1,2,1,
};

const unsigned char prev_tab[] = {
  0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};


/* bits needed to represent a number between 0 and n */
uint bits (uint n) {
  uint b = 0;
  while (n) { b++; n >>= 1; }
  return b;
}




uint GetField(uint *A, register  uint len, register uint index) {
  register uint i=index*len/WORD, j=index*len-WORD*i, result;
  if (j+len <= WORD)
    result = (A[i] << (WORD-j-len)) >> (WORD-len);
  else {
    result = A[i] >> j;
    result = result | (A[i+1] << (WW-j-len)) >> (WORD-len);
  }
  return result;
}


void SetField(uint *A,register uint len, register uint index,register  uint x) {
  uint i=index*len/WORD, j=index*len-i*WORD;
  uint mask = ((j+len) < WORD ? ~0u << (j+len) : 0) | ((WORD-j) < WORD ? ~0u >> (WORD-j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j+len>WORD) {
    mask = ((~0u) << (len+j-WORD));
    A[i+1] = (A[i+1] & mask)| x >> (WORD-j);
  }
}


uint GetVarField(uint *A, register  uint ini, register uint fin) {
  register uint i=ini/WORD, j=ini-WORD*i, result;
  register uint len = (fin-ini+1);
  if (j+len <= WORD)
    result = (A[i] << (WORD-j-len)) >> (WORD-len);
  else {
    result = A[i] >> j;
    result = result | (A[i+1] << (WW-j-len)) >> (WORD-len);
  }
  return result;
}


void SetVarField(uint *A,register uint ini, register uint fin,register  uint x) {
  uint i=ini/WORD, j=ini-i*WORD;
  uint len = (fin-ini+1);
  uint mask = ((j+len) < WORD ? ~0u << (j+len) : 0) | ((WORD-j) < WORD ? ~0u >> (WORD-j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j+len>WORD) {
    mask = ((~0u) << (len+j-WORD));
    A[i+1] = (A[i+1] & mask)| x >> (WORD-j);
  }
}


unsigned GetFieldW32(uint *A,register uint index) {
  return A[index];
}


void SetField32(uint *A, register uint index,register uint x) {
  A[index]=x;
}


unsigned GetFieldW16(uint *A,register uint index) {
  register uint i=index/2, j=(index&1)<<4, result;
  result = (A[i] << (16-j)) >> (16);
  return result;
}

unsigned GetFieldW4(uint *A,register uint index) {
  register uint i=index/8, j=(index&0x7)<<2;
  /*register unsigned i=index/8, j=index*4-32*i; */
  return (A[i] << (28-j)) >> (28);
}


uint popcount (register int x) {
  return __popcount_tab[(x >>  0) & 0xff]  + __popcount_tab[(x >>  8) & 0xff]  + __popcount_tab[(x >> 16) & 0xff] + __popcount_tab[(x >> 24) & 0xff];
}


uint popcount16 (register int x) {
  return __popcount_tab[x & 0xff]  + __popcount_tab[(x >>  8) & 0xff];
}


uint popcount8 (register int x) {
  return __popcount_tab[x & 0xff];
}

