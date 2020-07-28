#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define n_threads 4

	/* VARIAVEIS GLOBAIS */
int primos[1000];
int numero_atual = 0;
int numeros_inseridos;
int cont_primos = 0;
int parada = 0;
pthread_mutex_t trava;

	/* FUNÇÃO CHECAGEM DE PRIMO */
int eh_primo(long int num){
  int eh;
  int divisores = 0; // número é primo se possui 2 divisores (ele próprio e 1)
  for(long int i = 1; i <= num; i++){
    if(num % i == 0)
	divisores++;
    if(divisores > 2)
    	break;
  }
  if(divisores == 2)
    eh = 1;
  else
    eh = 0;
  return eh;
}

	/* FUNÇÃO THREAD */
void* funcao_thread(void* arg){
  while (parada == 0){
    pthread_mutex_lock(&trava); // lock e unlock garantem 'comunicação' entre as threads executando 'simultâneamente' a mesma função 
    
    if(numero_atual == numeros_inseridos)
      parada = 1; // para após análise do último numero inserido, threads não mais incrementam o contador de primos
      
    cont_primos += eh_primo(primos[numero_atual]);
    numero_atual++;
    
    pthread_mutex_unlock(&trava);
  }
  return NULL;
}

 
int main() {

	/* LEITURA DOS NUMEROS ENTRADOS */
  int num;
  numeros_inseridos = 0;
  char c; // caractere sendo lido 
  do{
    scanf("%d", &num);
    primos[numeros_inseridos] = num;
    numeros_inseridos++;
  }while(c = getchar() != '\n'); // a leitura da linha inserida acontece até encontrar a quebra de linha

	/* THREADS */
  pthread_t threads[n_threads];
  void* args;// os args nem serão usados pois as operações acontece sob variáveis globais
  int narg; // args e narg foram definidos apenas para chamada da função
  
  for(int i = 0; i < n_threads; i++){
    if( i <= numeros_inseridos){ // garante que quando inseridos menos de 4 números, não são criadas mais que i threads
      narg = 0;
      args = (void*) &narg;
      pthread_create(&(threads[i]), NULL, funcao_thread, args); // serão iniciadas 4 threads no max
    }
  }
  
  for (int j = 0; j < n_threads; j++)
    pthread_join(threads[j], NULL);
  
  	/**/
  printf("%d\n", cont_primos); 
  return 0;
}
