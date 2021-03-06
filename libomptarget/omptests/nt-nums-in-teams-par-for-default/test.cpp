#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <stdlib.h>

/*
export "LD_LIBRARY_PATH=/gsa/yktgsa/home/e/i/eichen/new-tlomp/lomp/source/lib64:/usr/local/cuda/lib64"
export LIBRARY_PATH="/home/eichen/eichen/lnew/obj/lib"

/gsa/yktgsa/home/e/i/eichen/lnew/obj/bin/clang++ -v  -I/gsa/yktgsa/home/e/i/eichen/new-tlomp/lomp/source/lib64/ -I/gsa/yktgsa/home/e/i/eichen/new-tlomp/lomp/source/   -L/gsa/yktgsa/home/e/i/eichen/new-tlomp/lomp/source/lib64/ -L/gsa/yktgsa/home/e/i/eichen/new-tlomp/lomp/source/lib64/ -target powerpc64le-ibm-linux-gnu -mcpu=pwr8 -fopenmp=libomp -fopenmp-targets=nvptx64-nvidia-cuda -O3 test-distr-par-for.cpp
 */

#define DEBUG 0

#define N 127*1024
int error=0;

#define CHECK(_m, _a, _b) if ((_a) != (_b)) printf("  error for %s: got %d, expected %d, error %d\n", (_m), (_a), (_b), ++error)

int A[N];
int  main() 
{
  int numFromEnv = 0;
  char *str = getenv("OMP_NUM_TEAMS");
  if (str) { 
    numFromEnv = atoi(str);
    printf("got num teams = %d\n", numFromEnv);
  }

  printf("test 1: use iteration trip count with default number of threads\n");
  int teams1, threads1;
  #pragma omp target teams distribute parallel for map(teams1, threads1) 
  for(int i=0; i<N; i++) {
    if (i==0) {
      teams1 = omp_get_num_teams();
      threads1 = omp_get_num_threads();
      if (DEBUG) printf("  num teams %d, num thread %d\n", teams1, threads1);
    }
    A[i] = 2*i;
  }  
  if (numFromEnv) {
    CHECK("test1, teams", teams1, numFromEnv);
  } else {
    CHECK("test1, teams", teams1, N/1024);
  }
  CHECK("test1, threads", threads1, 1024);
  printf("  completed\n");

  printf("test 2: use iteration trip count with 512 threads\n");
  int teams2, threads2;
  #pragma omp target teams distribute parallel for map(teams2, threads2) thread_limit(512)
  for(int i=0; i<N; i++) {
    if (i==0) {
      teams2 = omp_get_num_teams();
      threads2 = omp_get_num_threads();
      if (DEBUG) printf("  num teams %d, num thread %d\n", teams2, threads2);
    }
    A[i] += 2*i;
  }  
  if (numFromEnv) {
    CHECK("test2, teams", teams2, numFromEnv);
  } else {
    CHECK("test2, teams", teams2, N/512);
  }

  CHECK("test2, threads", threads2, 512);
  printf("  completed\n");

  printf("test 2: use iteration trip count with 25 teams & 512 threads\n");
  int teams3, threads3;
  #pragma omp target teams distribute parallel for map(teams3, threads3) num_teams(25) thread_limit(512)
  for(int i=0; i<N; i++) {
    if (i==0) {
      teams3 = omp_get_num_teams();
      threads3 = omp_get_num_threads();
      if (DEBUG) printf("  num teams %d, num thread %d\n", teams3, threads3);
    }
    A[i] += 2*i;
  }  
  CHECK("test3, teams", teams3, 25);
  CHECK("test4, threads", threads3, 512);
  printf("  completed\n");

  printf("tests completed with %d errors\n", error);
  return 0;
}
