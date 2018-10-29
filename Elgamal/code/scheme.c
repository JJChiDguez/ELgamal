/* --------------------------- *
 * This code was done by:      *
 * Cuate-Gónzales Oliver and,  *
 * Chi-Domínguez Jesús-Javier. *
 * --------------------------- */
 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void Elgamal_Gen();
void Elgamal_encrypt();
void Elgamal_decrypt();
void Enc_Scheme();
void Dec_Scheme();
void Hash();
void get_message();
void generator_Gen();

int main(void)
{
 srand(time(NULL));
 int solve;
 mpz_t q;
 mpz_t p;

 mpz_init(q);
 mpz_init(p);

 /* In this part we read the possible prime that it's in file_name.txt */
 char ch, file_name[20000], message_file[10000];
 FILE *fp;
 char m0[100000],m1[100000];

 printf("Enter the name of file where the possible prime is:\n");
 scanf("%s",file_name);
 
 printf("\n");
 fp = fopen(file_name,"r"); // read mode

 if( fp == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
 }
 
 printf("The %s file contains the public prime q:\n", file_name);

 int nk = 0;
 while( fscanf(fp, "%s", file_name)  != EOF ) // reading file..
		nk += 1;
 
 mpz_t *array;

 array = (mpz_t*) malloc(nk * sizeof(mpz_t));
 int qwert;

 for(qwert = 0; qwert < nk; qwert++)
 	mpz_init(array[qwert]);
 
 rewind(fp);
 for(qwert = 0; qwert < nk; qwert++){
 	fscanf(fp, "%s", file_name);
 	mpz_set_str(array[qwert], file_name, 10);
 }

 /* set N to the number file_name (as a number and not as string)*/
 
 mpz_set(p, array[0]);
 mpz_set(q, array[1]);

 gmp_printf(" \n%Zd ", q);
 
 /* Miller-Rabin */
 solve = miller_rabin(q);
 
 gmp_printf("And 2*k*q + 1 is equal to: \n%Zd ", p);

 /* Miller-Rabin */
 solve = miller_rabin(p);
 
 get_message(m0,m1);

 /*EXAMPLE OF ELGAMAL*/
 mpz_t g;
 mpz_t sk;
 mpz_t pk;
 mpz_t m_0;
 mpz_t m_1;
 mpz_t c_0;
 mpz_t c_1;
 mpz_t m;

 mpz_init(g);
 mpz_init(pk);
 mpz_init(sk);
 mpz_init(m_0);
 mpz_init(m_1);
 mpz_init(c_0);
 mpz_init(c_1);
 mpz_init(m);

 generator_Gen(g, array, nk);
 mpz_set_str(m_0, m0, 10);
 mpz_set_str(m_1, m1, 10);
 
 printf("*********EXAMPLE OF ELGAMAL*********");
 
 Elgamal_Gen(sk, pk, g, p);
 
 gmp_printf("\nvalue of g: \n%Zd ", g);
 gmp_printf("\nvalue of mod: \n%Zd ", p);
 gmp_printf("\nvalue of pk: \n%Zd ", pk);
 gmp_printf("\nvalue of sk: \n%Zd\n", sk);

 gmp_printf("\nvalue of msg0: \n%Zd", m_0); 
 Elgamal_encrypt(c_0, c_1, m_0, pk, g, p);
 Elgamal_decrypt(m, c_0, c_1, sk, p);
 gmp_printf("\nvalue of c0: \n%Zd ", c_0);
 gmp_printf("\nvalue of c1: \n%Zd ", c_1);
 gmp_printf("\nvalue of m0: \n%Zd\n", m);

 gmp_printf("\nvalue of msg1: \n%Zd", m_1); 
 Elgamal_encrypt(c_0, c_1, m_1, pk, g, p);
 Elgamal_decrypt(m, c_0, c_1, sk, p);
 gmp_printf("\nvalue of c0: \n%Zd ", c_0);
 gmp_printf("\nvalue of c1: \n%Zd ", c_1);
 gmp_printf("\nvalue of m1: \n%Zd\n", m);

 printf("\n*********EXAMPLE OF OUR CRYPTOSYSTEM*********");

 gmp_printf("\nvalue of msg0: \n%Zd", m_0); 
 Enc_Scheme(c_0, c_1, m_0, pk, g, p);
 Dec_Scheme(m, c_0, c_1, sk, p);
 gmp_printf("\nvalue of c0: \n%Zd ", c_0);
 gmp_printf("\nvalue of c1: \n%Zd ", c_1);
 gmp_printf("\nvalue of m0: \n%Zd\n", m);

 gmp_printf("\nvalue of msg1: \n%Zd", m_1); 
 Enc_Scheme(c_0, c_1, m_1, pk, g, p);
 Dec_Scheme(m, c_0, c_1, sk, p);
 gmp_printf("\nvalue of c0: \n%Zd ", c_0);
 gmp_printf("\nvalue of c1: \n%Zd ", c_1);
 gmp_printf("\nvalue of m0: \n%Zd\n", m);


 
 /* free used memory */
 mpz_clear(q);
 mpz_clear(p);
 mpz_clear(g);
 mpz_clear(sk);
 mpz_clear(pk);
 mpz_clear(m_0);
 mpz_clear(m_1);
 mpz_clear(c_0);
 mpz_clear(c_1);

 for(qwert = 0; qwert < nk; qwert++)
 	mpz_clear(array[qwert]);

 free(array);
 fclose(fp);

 return EXIT_SUCCESS;
}

// The following function computes: m*2^k
int m_times_2_to_the_k(mpz_t m, mpz_t N){
	int equal, k;
	mpz_t auxN;
	mpz_t residual;
	
	mpz_init(auxN);
	mpz_init(residual);
	
	mpz_set(auxN, N);
	mpz_mod_ui(residual, auxN, 2);
	equal = mpz_cmp_ui(residual, 0);
	k = 0;

	while( equal == 0){
		k += 1;
		mpz_divexact_ui(auxN, auxN, 2);
		mpz_mod_ui(residual, auxN, 2);
		equal = mpz_cmp_ui(residual, 0);
	}

	mpz_set(m, auxN);
	
	mpz_clear(auxN);
	mpz_clear(residual);
	return k;
}

// Miller rabin algorithm
int miller_rabin(mpz_t N){
 int seed = rand();

 int k;
 mpz_t N_one;
 mpz_t m;
 mpz_t a;
 mpz_t b;
 int equal;
 gmp_randstate_t r_state;
 
 mpz_init(N_one);
 mpz_init(m);
 mpz_init(b);

 gmp_randinit_default(r_state);
 gmp_randseed_ui(r_state, seed);
 mpz_init(a);
 
 mpz_sub_ui(N_one, N, 1);
 mpz_set_str(m, "0", 10);
 k = m_times_2_to_the_k(m, N_one);
 
 mpz_urandomm(a,r_state, N);
  
 mpz_powm(b, a, m, N);
 
 equal = mpz_cmp_ui(b, 1);
 if( equal == 0){
 	return 1;
 }
 else{
 	int i;
 	for (i = 0; i < k; ++i){
 		equal = mpz_cmp(b, N_one);
 		if( equal == 0){
 			return 1;
 			break;
 		}
 		else{
 			mpz_powm_ui(b, b, 2, N);
 		}
 	}
 	return 0;
 }

 /* free used memory */
 mpz_clear(N_one);
 mpz_clear(m);
 mpz_clear(b);
 gmp_randclear(r_state);
 mpz_clear(a);
}

// Key generator for our scheme
void generator_Gen(mpz_t g, mpz_t *primes, int sz){
	int seedg = rand();
	int cg, equal_1;
	mpz_t condition;
	int gen_true = 1, yeah = 0;
	mpz_t p_1;
	mpz_t pq_i;

	mpz_init(condition);
	mpz_init(p_1);
	mpz_init(pq_i);
	gmp_randstate_t rg_state;
	
   gmp_randinit_default(rg_state);
	gmp_randseed_ui(rg_state, seedg);
 	
 	mpz_sub_ui(p_1, primes[0], 1);

 	while( gen_true == 1){
 		mpz_urandomm(g, rg_state, primes[0]);
 		for (cg = 1; cg < sz; ++cg){
 			mpz_cdiv_q(pq_i, p_1, primes[cg]);
 			mpz_powm(condition, g, pq_i, primes[0]);
 			equal_1 = mpz_cmp_ui(condition, 1);
 			if( equal_1 == 0){
 				yeah = 1;
 				cg = sz;
 			}
 		}
 		if(yeah == 0)
 			gen_true = 0;
 		else
 			yeah = 0;
 	}

 	mpz_cdiv_q(pq_i, p_1, primes[1]);
 	mpz_powm(g, g, pq_i, primes[0]);
 	mpz_clear(condition);
 	mpz_clear(p_1);
 	mpz_clear(pq_i);

}

// Key generator for Elgamal
void Elgamal_Gen(mpz_t sk, mpz_t pk, mpz_t g, mpz_t modulus){	
	int seedx = rand();
	gmp_randstate_t ry_state;
	
   gmp_randinit_default(ry_state);
	gmp_randseed_ui(ry_state, seedx);
 	
 	mpz_urandomm(sk, ry_state, modulus);
 	mpz_powm(pk, g, sk, modulus);
	gmp_randclear(ry_state);
}

// Encryption function for Elgamal
void Elgamal_encrypt(mpz_t cipher0, mpz_t cipher1, mpz_t message, mpz_t pk, mpz_t g, mpz_t modulus){
	int seedy = rand();
	
	mpz_t y;
	mpz_t pky;
	gmp_randstate_t ry_state;
	
	mpz_init(y);
	mpz_init(pky);
    gmp_randinit_default(ry_state);
	gmp_randseed_ui(ry_state, seedy);
 	
 	mpz_urandomm(y,ry_state, modulus);
 	
 	mpz_powm(pky, pk, y, modulus);
 	mpz_mul(cipher0, message, pky);
 	mpz_mod(cipher0, cipher0, modulus);
 	mpz_powm(cipher1, g, y, modulus);

	mpz_clear(y);
	mpz_clear(pky);
	gmp_randclear(ry_state);
}

// Decryption function for Elgamal
void Elgamal_decrypt(mpz_t message,mpz_t cipher0, mpz_t cipher1, mpz_t sk, mpz_t modulus){
	mpz_t gxy;
	mpz_t inv_gxy;

	mpz_init(gxy);
	mpz_init(inv_gxy);

	mpz_powm(gxy, cipher1, sk, modulus);
	mpz_invert(inv_gxy, gxy, modulus);
	mpz_mul(message, cipher0, inv_gxy);
	mpz_mod(message, message, modulus);

	mpz_clear(gxy);
	mpz_clear(inv_gxy);
}

/*
void Hash(mpz_t h,mpz_t x,mpz_t modulus){
	mpz_mod(h, x, modulus);
}
*/

// Hash function implementation
void Hash( mpz_t h, mpz_t K, mpz_t M){
	double c = 0.6180339887;
	mpf_t prod;
	mpf_t cons;
	mpf_t aux;

	mpf_init(prod);
	mpf_init(cons);
	mpf_init(aux);
	mpf_set_z(prod,K);
	mpf_set_d(cons, c);

	mpf_mul(prod, prod, cons);
	mpf_floor(aux, prod);
	mpf_sub(prod, prod, aux);
	mpf_set_z(aux,M);
	mpf_mul(prod, prod, aux);
	mpf_floor(prod, prod);
	mpz_set_f(h, prod);
}

// Encryption function for our scheme
void Enc_Scheme(mpz_t cipher0, mpz_t cipher1, mpz_t message, mpz_t pk, mpz_t g, mpz_t modulus){
	int seedy = rand();
	mpz_t b;
	mpz_t m;
	mpz_t pkb;
	mpz_t h;
	gmp_randstate_t ry_state;
	
	mpz_init(b);
	mpz_init(m);
	mpz_init(pkb);
	mpz_init(h);
   gmp_randinit_default(ry_state);
	gmp_randseed_ui(ry_state, seedy);
 	mpz_urandomm(b,ry_state, modulus);
 	
 	mpz_powm(pkb, pk, b, modulus);
	Hash(h,pkb,modulus);
	mpz_xor (m, message, h);
 	mpz_mul(cipher0, m, pkb);
 	mpz_mod(cipher0, cipher0, modulus);
 	mpz_powm(cipher1, g, b, modulus);

	mpz_clear(b);
	mpz_clear(pkb);
	gmp_randclear(ry_state);
}

// Decryption function for our scheme
void Dec_Scheme(mpz_t message, mpz_t cipher0, mpz_t cipher1, mpz_t sk, mpz_t modulus){
	mpz_t gxy;
	mpz_t h;
	mpz_t inv_gxy;

	mpz_init(gxy);
	mpz_init(h);
	mpz_init(inv_gxy);

	mpz_powm(gxy, cipher1, sk, modulus);
	mpz_invert(inv_gxy, gxy, modulus);
	Hash(h, gxy, modulus);
	mpz_mul(message, cipher0, inv_gxy);
	mpz_mod(message, message, modulus);
	mpz_xor(message, message, h);

	mpz_clear(gxy);
	mpz_clear(inv_gxy);
}

// Read the message from file
void get_message(char *m_0, char *m_1){
	 FILE *fm;
	 char message_file[100];
	 printf("\nEnter the name of file where the message is:\n");
	 scanf("%s", message_file);

	 printf("\n");
	 fm = fopen(message_file,"r"); // read mode

	 if( fm == NULL ){
	    perror("Error while opening the file.\n");
	    exit(EXIT_FAILURE);
	 }

	 fseek(fm, 0, SEEK_END);
	 int dig = ftell(fm)-1;
	 rewind(fm);
	 fgets(m_0, dig/2+1, fm);
	 fgets(m_1, dig, fm);
	 fclose(fm);
}
