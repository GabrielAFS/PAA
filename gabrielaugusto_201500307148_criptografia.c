#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

//Divide o expoente até que ele chegue em 2
uint64_t calcule(unsigned int exp, unsigned int base, unsigned int p){
	uint64_t tot = 1;
	
	if(exp > 2){
		tot = calcule(exp/2, base, p);
		tot = (uint64_t) (tot*tot) % p; //equivalente a base ^ expoente
	}
	else
		tot = (uint64_t) pow(base, exp) % p;
	
	return tot;
}

//Exponenciação
unsigned int expMod(unsigned int k, unsigned int g, unsigned int p){
	uint8_t bit;
	int i = 0;
	unsigned int expoente = k;
	uint64_t total = 1;
	
	while(expoente > 0){
		bit = expoente & 0x01; //bit menos significativo
		expoente = expoente >> 1; //remove o bit menos significativo
		//Caso 1, total é atualizado com a exponenciação parcial
		if(bit != 0)
			total = (total * calcule(pow(2, i), g, p)) % p;
		i++;
	}
	
	return total % p;
}

//Fatores de geração de chave
const uint32_t M = 1103515245;
const uint16_t D = 12345;
uint32_t key; //Chave geradora
unsigned char G(uint32_t k){
	static unsigned char j = 0;
	if(j == 0)
		key = M * key + D;
	unsigned char z = (key >> j) & 0x01;
	j = (j + 1) % 32;
	return z;
}

int main(int argc, char* argv[]){
	unsigned int a, b, p, g, s, i, numSMS;
	unsigned char c, k;
	FILE* file1 = fopen(argv[1], "r");
	FILE* file2 = fopen(argv[2], "w");
	
	fscanf(file1, "%*c %d ", &a);
	fscanf(file1, "%*c %d ", &b);
	fscanf(file1, "%*c%*c %d %d ", &p, &g);
	s = expMod(a, expMod(b, g, p), p); //Gera chave pública
	fprintf(file2, "A->B: %d\nB->A: %d\n", expMod(a, g, p), expMod(b, g, p)); //Chaves criptografads
	
	fscanf(file1, "%d ", &numSMS);
	key = s; //inicializa com a chave pública
	for(i = 0; i < numSMS; i++){
		if(i % 2 == 0)
			fprintf(file2, "A->B: ");
		else
			fprintf(file2, "B->A: ");
		while((c = fgetc(file1)) != '\n'){
			unsigned int j = 0, m = c;
			while(j < 8){
				k = G(s);
				m = m ^ (k << j);
				j++;
			}
			fprintf(file2, "%d ", m);
		}
		fprintf(file2, "\n");
	}
	
	fclose(file1);
	fclose(file2);
	
	return 0;
}
