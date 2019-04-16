#include <stdlib.h>

#define SETBIT(x, y) (x |= (1 << y))
#define TOTAL 2000000
#define ARRAYLENGTH 31250

const int seeds[8] = {3, 7, 31, 127, 8191, 131071, 524287, 2147483647};

void sample_string_A(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_B(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(1 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}
void sample_string_C(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = 'a';
   s[3] = (char)(1 + ((i*(i-3)) %217));
   s[4] = (char)(1 + ((17*i+129)%233 ));
   s[5] = '\0';
}
void sample_string_D(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'b';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_E(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(2 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}

struct bf_t {
   unsigned long bfArray[TOTAL / 64];     // 250,000
   bf_t(): bfArray() {}
};

bf_t* create_bf() {
   return new bf_t();
}

unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 

void insert_bf(bf_t* b, char* s) {
   int i;
   unsigned int hashValue;
   unsigned int index;
   unsigned int n_bit; 
   for(i = 0; i < 8; i++) {
      hashValue = MurmurHash2(s, strlen(s), seeds[i]);
      hashValue %= TOTAL;
      index = hashValue / 64;       // each long is 64-bit.
      n_bit = hashValue % 64;
      SETBIT(b->bfArray[index * ARRAYLENGTH], n_bit);
   }
} 

int is_element(bf_t* b, char* q) {
   int isEle = 1;
   int i;
   unsigned int hashValue;
   unsigned int index;
   unsigned int n_bit; 
   unsigned long tmp;
   for(i = 0; i < 8; i++) {
      hashValue = MurmurHash2(s, strlen(s), seeds[i]);
      hashValue %= TOTAL;
      index = hashValue / 64;       // each long is 64-bit.
      n_bit = hashValue % 64;
      tmp = b->bfArray[index];
      tmp = (tmp >> n_bit) & 1;
      isEle &= tmp;
   }

   return isEle;
}

int main()
{  long i,j; 
   bf_t * bloom;
   bloom = create_bf();
   printf("Created Filter\n");
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      insert_bf( bloom, s );
   }
   printf("inserted 2,000,000 strings of length 8,7,6.\n");
   
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (1)\n"); exit(0); }
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (2)\n"); exit(0); }
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      if( is_element( bloom, s ) != 1 )
	{  printf("found negative error (3)\n"); exit(0); }
   }
   j = 0;
   for( i= 0; i< 500000; i++ )
   {  char s[8];
      sample_string_D(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_E(s,i);
      if( is_element( bloom, s ) != 0 )
	j+=1;
   }
   printf("Found %d positive errors out of 1,000,000 tests.\n",j);
   printf("Positive error rate %f\%.\n", (float)j/10000.0);

} 
