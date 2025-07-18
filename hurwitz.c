#include <stdio.h>
#include <stdlib.h>

int min(int x, int y);
void sigma_and_lambda(int n, int* sigma, int* lambda);
void hurwitz(int n, int * hurwitz_list);
void find_and_print_congruences(int n, int* hurwitz_list, int modulo, int show_trivial, int coefficent_bound);

int is_trivial(int a, int b, int c);
int is_trivial(int a, int b, int c) {
    for(int n = 0; n < 4; n++) {
        int residue = (a*n*n+b*n+c) % 4;
        if(residue == 0 || residue == 3) {
            return 0;
        }
    }
    return 1;
}
void find_and_print_congruences(int N, int* hurwitz_list, int modulo, int show_trivial, int coefficent_bound) {
    for(int a = 1; a <= coefficent_bound; a++) {
        for(int b = 0; b <= coefficent_bound; b++) {
            for(int c = 0; c <= coefficent_bound; c++) {
                if(is_trivial(a,b,c)) {
                    if(show_trivial) {
                        printf("%dn^2+%dn+%d\n", a, b, c);
                    }
                } else {
                    int n = 0;
                    int v = c;
                    int is_congruent = 1;
                    while(v < N && is_congruent) {
                        //printf("H(%d)=%d\n",v,hurwitz_list[v] % modulo);
                        if((hurwitz_list[v] % modulo) != 0) {
                            is_congruent = 0;
                        }
                        n += 1;
                        v = a*n*n+b*n+c;
                    }
                    if(is_congruent) {
                        printf("%dn^2+%dn+%d, %d\n", a, b, c, is_congruent);
                    }
                }
            }
        }
    }
}

int main() {
    int n = 100000;
    int* hurwitz_list = (int*) calloc(n + 1, sizeof(int));
    hurwitz(n, hurwitz_list);
    find_and_print_congruences(n,hurwitz_list,11,0,1000);
    free(hurwitz_list);
    return 0;
}

int min(int x, int y) {
    if (x <= y) {
        return x;
    }
    return y;
}

// return sigma(n) and 2*lambda(n)
void sigma_and_lambda(int n, int* sigma, int*lambda) {
    for (int d = 1; d <= n; ++d) {
        for (int m = d; m <= n; m += d) {
            sigma[m] += d;
            int q = m / d;
            lambda[m] += (d < q) ? d : q;
        }
    }
}

//return 12*H(n)
// hurwitz list should be of size n+1
// O(N^(3/2))
void hurwitz(int n, int * hurwitz_list) {
    int* sigma = (int*) calloc(n + 2, sizeof(int));
    int* lambda = (int*) calloc(n + 2, sizeof(int));
    sigma_and_lambda(n+1, sigma, lambda);
    for(int k = 0; k <= n; k++) {
        int res = k % 4;
        if(res == 1 || res == 2) {
            hurwitz_list[k] = 0;
        }
        else if (res == 3) {
            int sum = 0;
            for(int s = 1; 4*s*s < k; s++) {
                sum += hurwitz_list[k-4*s*s];
            }
            hurwitz_list[k] = 4*sigma[k]-6*lambda[k]-2*sum;
        }
        else {
            int sum = 0;
            for(int s = 1; 4*s*(s+1) <= k; s++) {
                sum += hurwitz_list[k-4*s*(s+1)];
            }
            hurwitz_list[k] = 2*sigma[k+1]-3*lambda[k+1]-sum;
        }
    }
    free(sigma);
    free(lambda);
}