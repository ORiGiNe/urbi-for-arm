/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 *
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.
 * This code was originally written by Colin Plumb in 1993 (in public domain).
 */

#include <cstring>
#include "md5-impl.hh"


/* Little-endian byte-swapping routines.  Note that these do not
   depend on the size of datatypes such as unsigned int, nor do they require
   us to detect the endianness of the machine we are running on.  It
   is possible they should be macros for speed, but I would be
   surprised if they were a performance bottleneck for MD5.  */

static
unsigned int
getu32(const unsigned char* addr)
{
  return (((((unsigned long)addr[3] << 8) | addr[2]) << 8)
    | addr[1]) << 8 | addr[0];
}

static
void
putu32(unsigned int data, unsigned char* addr)
{
  addr[0] = (unsigned char)data;
  addr[1] = (unsigned char)(data >> 8);
  addr[2] = (unsigned char)(data >> 16);
  addr[3] = (unsigned char)(data >> 24);
}


/*
 * Default constructor, just call init.
 */
MD5::MD5()
{
  init();
}


/*
 * Other constructor, calls init and update.
 */
MD5::MD5(const std::string& s)
{
  init();
  update(s);
}


/*
 * Start MD5 accumulation. Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void
MD5::init()
{
  is_computed_ = false;

  ctx_.buf[0] = 0x67452301;
  ctx_.buf[1] = 0xefcdab89;
  ctx_.buf[2] = 0x98badcfe;
  ctx_.buf[3] = 0x10325476;

  ctx_.bits[0] = 0;
  ctx_.bits[1] = 0;
}


/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void
MD5::update(const std::string& s)
{
  unsigned int t;
  const char* buf = s.c_str();
  unsigned int len = s.size();

  /* Update bitcount */
  t = ctx_.bits[0];
  if ((ctx_.bits[0] = (t + ((unsigned int)len << 3)) & 0xffffffff) < t)
    ctx_.bits[1]++; /* Carry from low to high */
  ctx_.bits[1] += len >> 29;

  t = (t >> 3) & 0x3f;    /* Bytes already in shsInfo->data */

  /* Handle any leading odd-sized chunks */
  if (t) {
    unsigned char* p = ctx_.in + t;

    t = 64-t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    transform();
    buf += t;
    len -= t;
  }

  /* Process data in 64-byte chunks */
  while (len >= 64) {
    memcpy(ctx_.in, buf, 64);
    transform();
    buf += 64;
    len -= 64;
  }

  /* Handle any remaining bytes of data. */
  memcpy(ctx_.in, buf, len);
}


/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void
MD5::final()
{
  unsigned int count;
  unsigned char* p;

  /* Compute number of bytes mod 64 */
  count = (ctx_.bits[0] >> 3) & 0x3F;

  /* Set the first char of padding to 0x80.  This is safe since there is
     always at least one byte free */
  p = ctx_.in + count;
  *p++ = 0x80;

  /* Bytes of padding needed to make 64 bytes */
  count = 64 - 1 - count;

  /* Pad out to 56 mod 64 */
  if (count < 8) {
    /* Two lots of padding:  Pad the first block to 64 bytes */
    memset(p, 0, count);
    transform();

    /* Now fill the next block with 56 bytes */
    memset(ctx_.in, 0, 56);
  } else {
    /* Pad block to 56 bytes */
    memset(p, 0, count-8);
  }

  /* Append length in bits and transform */
  putu32(ctx_.bits[0], ctx_.in + 56);
  putu32(ctx_.bits[1], ctx_.in + 60);

  transform();
  putu32(ctx_.buf[0], digest_);
  putu32(ctx_.buf[1], digest_ + 4);
  putu32(ctx_.buf[2], digest_ + 8);
  putu32(ctx_.buf[3], digest_ + 12);
  memset(&ctx_, 0, sizeof(ctx_));    /* In case it's sensitive */
  is_computed_ = true;
}


/* The four core functions - F1 is optimized somewhat */

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
  ( w += f(x, y, z) + data, w &= 0xffffffff, w = w<<s | w>>(32-s), w += x )


/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data. MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void
MD5::transform()
{
  register unsigned int a, b, c, d;
  unsigned int in[16];
  int i;

  for (i = 0; i < 16; ++i)
    in[i] = getu32(ctx_.in + 4 * i);

  a = ctx_.buf[0];
  b = ctx_.buf[1];
  c = ctx_.buf[2];
  d = ctx_.buf[3];

  MD5STEP(F1, a, b, c, d, in[ 0]+0xd76aa478,  7);
  MD5STEP(F1, d, a, b, c, in[ 1]+0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[ 2]+0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[ 3]+0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[ 4]+0xf57c0faf,  7);
  MD5STEP(F1, d, a, b, c, in[ 5]+0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[ 6]+0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[ 7]+0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[ 8]+0x698098d8,  7);
  MD5STEP(F1, d, a, b, c, in[ 9]+0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10]+0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11]+0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12]+0x6b901122,  7);
  MD5STEP(F1, d, a, b, c, in[13]+0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14]+0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15]+0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[ 1]+0xf61e2562,  5);
  MD5STEP(F2, d, a, b, c, in[ 6]+0xc040b340,  9);
  MD5STEP(F2, c, d, a, b, in[11]+0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[ 0]+0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[ 5]+0xd62f105d,  5);
  MD5STEP(F2, d, a, b, c, in[10]+0x02441453,  9);
  MD5STEP(F2, c, d, a, b, in[15]+0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[ 4]+0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[ 9]+0x21e1cde6,  5);
  MD5STEP(F2, d, a, b, c, in[14]+0xc33707d6,  9);
  MD5STEP(F2, c, d, a, b, in[ 3]+0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[ 8]+0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13]+0xa9e3e905,  5);
  MD5STEP(F2, d, a, b, c, in[ 2]+0xfcefa3f8,  9);
  MD5STEP(F2, c, d, a, b, in[ 7]+0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12]+0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[ 5]+0xfffa3942,  4);
  MD5STEP(F3, d, a, b, c, in[ 8]+0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11]+0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14]+0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[ 1]+0xa4beea44,  4);
  MD5STEP(F3, d, a, b, c, in[ 4]+0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[ 7]+0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10]+0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13]+0x289b7ec6,  4);
  MD5STEP(F3, d, a, b, c, in[ 0]+0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[ 3]+0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[ 6]+0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[ 9]+0xd9d4d039,  4);
  MD5STEP(F3, d, a, b, c, in[12]+0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15]+0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[ 2]+0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[ 0]+0xf4292244,  6);
  MD5STEP(F4, d, a, b, c, in[ 7]+0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14]+0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[ 5]+0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12]+0x655b59c3,  6);
  MD5STEP(F4, d, a, b, c, in[ 3]+0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10]+0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[ 1]+0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[ 8]+0x6fa87e4f,  6);
  MD5STEP(F4, d, a, b, c, in[15]+0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[ 6]+0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13]+0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[ 4]+0xf7537e82,  6);
  MD5STEP(F4, d, a, b, c, in[11]+0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[ 2]+0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[ 9]+0xeb86d391, 21);

  ctx_.buf[0] += a;
  ctx_.buf[1] += b;
  ctx_.buf[2] += c;
  ctx_.buf[3] += d;
}


static const char hex_chars[] = {
  '0', '1', '2', '3',
  '4', '5', '6', '7',
  '8', '9', 'a', 'b',
  'c', 'd', 'e', 'f'
};


std::string
MD5::digest_get()
{
  if (!is_computed_)
    final();
  std::string s;
  for (unsigned int i = 0; i < 16; ++i)
  {
    s += hex_chars[(digest_[i] >> 4) & 0x0F];
    s += hex_chars[digest_[i] & 0x0F];
  }
  return s;
}

