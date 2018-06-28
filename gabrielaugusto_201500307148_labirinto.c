#include <stdio.h>
#include <stdlib.h>

typedef struct Lab{
	char** lab;
	int x;
	int y;
}Lab;

typedef struct Stack{
	int x;
	int y;
}Stack;

void scanner(Lab lb, Stack* stk, int pi, int iniX, int iniY, FILE* out){
	int i = iniY, j = iniX;
	
	while(pi >= -1){
		//Verifica se é saída, caso não for, verifica se há lugares não explorados
		if(i+1 == lb.y && (lb.lab[i][j+1] == '1' && lb.lab[i][j-1] == '1')){
			fprintf(out, "SAIDA [%d,%d]\n", i, j);
			break;
		}
		else if(j+1 == lb.x && (lb.lab[i+1][j] == '1' && lb.lab[i-1][j] == '1')){
			fprintf(out, "SAIDA [%d,%d]\n", i, j);
			break;
		}
		else if(i-1 < 0 && (lb.lab[i][j+1] == '1' && lb.lab[i][j-1] == '1')){
			fprintf(out, "SAIDA [%d,%d]\n", i, j);
			break;
		}
		else if(j-1 < 0 && (lb.lab[i+1][j] == '1' && lb.lab[i-1][j] == '1')){
			fprintf(out, "SAIDA [%d,%d]\n", i, j);
			break;
		}
		else{
			if(j+1 < lb.x && lb.lab[i][j+1] != '1'){
				fprintf(out, "D [%d,%d]->[%d,%d]\n", i, j, i, j+1);
				lb.lab[i][j+1] = 'X';
				lb.lab[i][j] = '1';
				//Empilha a coordenada visitada
				stk[++pi].x = j;
				stk[pi].y = i;
				j++;
			}
			else if(i-1 >= 0 && lb.lab[i-1][j] != '1'){
				fprintf(out, "F [%d,%d]->[%d,%d]\n", i, j, i-1, j);
				lb.lab[i-1][j] = 'X';
				lb.lab[i][j] = '1';
				//Empilha a coordenada visitada
				stk[++pi].x = j;
				stk[pi].y = i;
				i--;
			}
			else if(j-1 >= 0 && lb.lab[i][j-1] != '1'){
				fprintf(out, "E [%d,%d]->[%d,%d]\n", i, j, i, j-1);
				lb.lab[i][j-1] = 'X';
				lb.lab[i][j] = '1';
				//Empilha a coordenada visitada
				stk[++pi].x = j;
				stk[pi].y = i;
				j--;
			}
			else if(i+1 < lb.y && lb.lab[i+1][j] != '1'){
				fprintf(out, "T [%d,%d]->[%d,%d]\n", i, j, i+1, j);
				lb.lab[i+1][j] = 'X';
				lb.lab[i][j] = '1';
				//Empilha a coordenada visitada
				stk[++pi].x = j;
				stk[pi].y = i;
				i++;
			}
			else{
				if(pi >= 0){
					fprintf(out, "BT [%d,%d]<-[%d,%d]\n", stk[pi].y, stk[pi].x, i, j);
					lb.lab[i][j] = '1';
					//Desempilha a coordenada que ainda falta ser totalmente explorada
					i = stk[pi].y;
					j = stk[pi--].x;
					lb.lab[i][j] = 'X';
				}
				else{
					fprintf(out, "SEM SAIDA\n");
					break;
				}
			}
		}
	}
	
	fflush(out);
}

int main(int argc, char* argv[]){
	unsigned int i, j, k, qtdLabs, iniX, iniY;
	int pi;
	Lab* l;
	Stack** stack; //Pilha com as coordenadas visitadas
	FILE* file1 = fopen(argv[1], "r");
	FILE* file2 = fopen(argv[2], "w");
	
	fscanf(file1, "%d ", &qtdLabs);
	l = (Lab*)malloc(qtdLabs * sizeof(Lab));
	stack = (Stack**)malloc(qtdLabs * sizeof(Stack*));
	
	for(i = 0; i < qtdLabs; i++){
		fscanf(file1, "%d %d ", &l[i].x, &l[i].y);
		l[i].lab = (char**)malloc(l[i].y * sizeof(char*));
		stack[i] = (Stack*)calloc(l[i].x * l[i].y, sizeof(Stack)); //Aloca posições para a pilha de coordenadas
		//Lendo o labirinto
		for(j = 0; j < l[i].y; j++){
			l[i].lab[j] = (char*)malloc((l[i].x + 1) * sizeof(char));
			for(k = 0; k < l[i].x; k++){
				fscanf(file1, "%c ", &l[i].lab[j][k]);
				//Ponto de partida
				if(l[i].lab[j][k] == 'X'){
					iniX = k;
					iniY = j;
				}
			}
		}
		fprintf(file2, "L%d\nINICIO [%d,%d]\n", i, iniX, iniY);
		pi = -1; //Inicializa a posição da pilha
		scanner(l[i], stack[i], pi, iniX, iniY, file2); //O robô vai escanear o mapa a procura da saída
	}
	
	fclose(file1);
	fclose(file2);
	free(l);
	free(stack);
	
	return 0;
}
