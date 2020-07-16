#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <windows.h>

sem_t n;
sem_t e;
sem_t mutex;

int capacidadActual = 0;
int tiempoEnProducir = 0;
int tiempoEnConsumir = 0;

const int CAPACIDAD_MAXIMA = 3;
const int CANTIDAD_PRODUCTORES = 5;
const int CANTIDAD_CONSUMIDORES = 5;

void *productor(void *arg)
{
	for (int prodActual = 1; prodActual <= CANTIDAD_PRODUCTORES; prodActual++)
	{
		printf("\nProductor  %d - Produce\n", prodActual);
		// CASO 1 - Se produce equitativamente a lo que se cunsume.
		//tiempoEnProducir = 2000;
		// CASO 2 - Se produce mucho mas rapido que lo que se consume.
		//tiempoEnProducir = 2000;
		// CASO 3 - Se produce mucho mas lento que lo que se consume.
		//tiempoEnProducir = 6000;
		// CASO 4 - Se produce y se consume con un tiempo aleatorio entre 2000 y 8000.
		tiempoEnProducir = (rand() % 6001) + 2000;
		Sleep(tiempoEnProducir);

		printf("\nProductor  %d - P (e)\n", prodActual);
		sem_wait(&e);

		printf("\nProductor  %d - P (mutex)\n", prodActual);
		sem_wait(&mutex);

		printf("\nProductor  %d - Agrega producto\n", prodActual);
		capacidadActual++;
		printf("Cantidad de productos : %d\n", capacidadActual);

		printf("\nProductor  %d - V (mutex)\n", prodActual);
		sem_post(&mutex);

		printf("\nProductor  %d - V (n)\n", prodActual);
		sem_post(&n);
	}
};

void *consumidor(void *arg)
{
	for (int consActual = 1; consActual <= CANTIDAD_CONSUMIDORES; consActual++)
	{
		printf("\nConsumidor %d - P (n)\n", consActual);
		sem_wait(&n);

		printf("\nConsumidor %d - P (mutex)\n", consActual);
		sem_wait(&mutex);

		printf("\nConsumidor %d - Toma producto\n", consActual);
		capacidadActual--;
		printf("Cantidad de productos : %d\n", capacidadActual);

		printf("\nConsumidor %d - V (mutex)\n", consActual);
		sem_post(&mutex);

		printf("\nConsumidor %d - V (e)\n", consActual);
		sem_post(&e);

		printf("\nConsumidor %d - Consume\n", consActual);
		// CASO 1 - Se produce equitativamente a lo que se cunsume.
		tiempoEnConsumir = 2500;
		// CASO 2 - Se produce mucho mas rapido a lo que se consume.
		//tiempoEnConsumir = 6500;
		// CASO 3 - Se produce mucho mas lento a lo que se consume.
		//tiempoEnConsumir = 2500;
		// CASO 4 - Se produce y se consume con un tiempo aleatorio entre 2000 y 8000 mili-segundos.
		tiempoEnConsumir = (rand() % 6001) + 2000;
		Sleep(tiempoEnConsumir);
	}
};

int main(void)
{
	// Declaro los 2 threads que voy a usar.
	pthread_t thread0;
	pthread_t thread1;

	// Inicializo los semaforos, con valor 0.
	sem_init(&n, 0, 0);
	sem_init(&e, 0, CAPACIDAD_MAXIMA);
	sem_init(&mutex, 0, 1);

	// Se crean dos threads ejecutando el proceso consumidor en uno y productor en el otro.
	pthread_create(&thread0, NULL, consumidor, NULL);
	pthread_create(&thread1, NULL, productor, NULL);

	// Se unen ambos threads una vez que culminen.
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);

	// Destruyo los semaforos.
	sem_destroy(&e);
	sem_destroy(&n);
	sem_destroy(&mutex);

	Sleep(100000);

	return 0;
};