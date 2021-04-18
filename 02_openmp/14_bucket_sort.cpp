#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

#pragma omp declare reduction(vec_int_plus : std::vector<int> : \
                              std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), std::plus<int>())) \
                    initializer(omp_priv = decltype(omp_orig)(omp_orig.size()))


int main() {
  int n = 50;
  int range = 5;
  std::vector<int> key(n);
  std::vector<int>offset(range,0);
  std::vector<int> bucket(range,0);
#pragma omp parallel
{
#pragma omp for  
  for (int i=0; i<n; i++) {
    key[i] = rand() % range;
    printf("%d ",key[i]);
  }
#pragma omp single 
  printf("\n");


} 
#pragma omp parallel for  
for (int i=0; i<n; i++)
#pragma omp atomic update
    bucket[key[i]]+=1;
 
  for (int i=1; i<range; i++) 
    offset[i] = offset[i-1] + bucket[i-1];
#pragma omp parallel for 
  for (int i=0; i<range; i++) {
 int j = offset[i];
    for (j; bucket[i]>0;bucket[i]--) {
      key[j++] = i;
    }
  }
	
  for (int i=0; i<n; i++) {
    printf("%d ",key[i]);
  }
  printf("\n");
}
