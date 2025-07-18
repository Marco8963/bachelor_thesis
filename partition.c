#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FL/math.h>
#include <mpfr.h>
#define PRECISION 64
void kloosterman_sum_mpfr(int k, int n, mpfr_t);
void partition_function_mpfr(int n, mpfr_t);
void smallest_parts_function_mpfr(int n, mpfr_t);
long double kloosterman_sum(int k, int n);
long double smallest_parts_function(int n);
int main() {
    for(int n = 1; n <= 100; n++) {
        long double res =  smallest_parts_function(5*n+4);
        printf("spt(%d)=%Le\n", n, res);
        printf("       =%d\n\n", (int) roundl(res) % 5);
    }
    /*mpfr_t y;
    mpfr_init2(y, PRECISION);
    for(int n = 1; n <= 100; n++) {
        smallest_parts_function_mpfr(n, y);
        mpfr_out_str (stdout, 10, 0, y, MPFR_RNDN);
        printf("\n");
    }
    mpfr_clear(y);
    mpfr_free_cache();*/
    return 0;
}

void kloosterman_sum_mpfr(int k, int n, mpfr_t result) {
    mpfr_set_d(result, 0, MPFR_RNDN);
    mpfr_t x;
    mpfr_init2 (x, PRECISION);
    for(int l = 0; l < 2*k; l++) {
        if (((3*l*l+l)/2+n) % k == 0) {
            mpfr_set_d(x, M_PI*((double) 6*l+1)/(6*k), MPFR_RNDN);
            mpfr_cos(x, x, MPFR_RNDN);
            if(l % 2 == 0) {
                mpfr_add(result, result, x, MPFR_RNDN);
            } else {
                mpfr_sub(result, result, x, MPFR_RNDN);
            }
        }
    }
    mpfr_clear(x);
}

long double kloosterman_sum(int k, int n) {
    long double sum = 0;
    for(int l = 0; l < 2*k; l++) {
        if (((3*l*l+l)/2+n) % k == 0) {
            double long summand = cos(M_PI*((double) 6*l+1)/(6*k));
            if(l % 2 == 0) {
                sum += summand;
            } else {
                sum -= summand;
            }
        }
    }
    return sum;
}

void partition_function_mpfr(int n, mpfr_t result) {
    int bound = 10000;
    long double sum = 0;
    long double lambda = sqrt(24*n-1);
    for (int k = 1; k <= bound; k++) {
        long double z = (M_PI*lambda) / (6*k);
        //sum += kloosterman_sum(k, n)*(cosh(z)-sinh(z)/z);
    }
    //return 4*sum/(24*n-1);
}

void smallest_parts_function_mpfr(int n, mpfr_t result) {
    int bound = 2000;
    mpfr_set_d(result, 0, MPFR_RNDN);
    mpfr_t z;
    mpfr_t c;
    mpfr_t part;
    mpfr_t sinh_value;
    mpfr_t cosh_value;
    mpfr_t kloosterman_sum_value;
    mpfr_init2(kloosterman_sum_value, PRECISION);
    mpfr_init2(c, PRECISION);
    mpfr_init2 (z, PRECISION);
    mpfr_init2 (sinh_value, PRECISION);
    mpfr_init2 (cosh_value, PRECISION);
    mpfr_init2 (part, PRECISION);
    mpfr_set_d(z, 24*n-1, MPFR_RNDN);
    mpfr_sqrt(z,z, MPFR_RNDN);
    for (int k = 1; k <= bound; k++) {
        mpfr_set_d(c, M_PI/(6*k), MPFR_RNDN);
        mpfr_mul(z, z, c, MPFR_RNDN);
        mpfr_sinh_cosh(sinh_value, cosh_value, z, MPFR_RNDN);

        mpfr_set_d(part, 1, MPFR_RNDN);
        mpfr_div(part, part, z, MPFR_RNDN);
        mpfr_add_d(part, part, 1, MPFR_RNDN);
        mpfr_mul(part, part, sinh_value, MPFR_RNDN);
        mpfr_sub(part, part, cosh_value, MPFR_RNDN);
        kloosterman_sum_mpfr(k, n, kloosterman_sum_value);
        mpfr_mul(part, kloosterman_sum_value, part, MPFR_RNDN);
        mpfr_add(result, result, part, MPFR_RNDN);
    }
    mpfr_div_d(result, result, 3, MPFR_RNDN);
    mpfr_clear(z);
    mpfr_clear(c);
    mpfr_clear(sinh_value);
    mpfr_clear(cosh_value);
    mpfr_clear(part);
    mpfr_clear(kloosterman_sum_value);
}

long double smallest_parts_function(int n) {
    int bound = 10000+n*n;
    long double result = 0;
    long double z = M_PI*sqrt(24*n-1)/6;
    for(int k = 1; k <= bound; k++) {
        long double lambda = z/k;
        result += kloosterman_sum(k, n)*( sinhl(lambda)*(1+1/lambda)-coshl(lambda) );
    }
    return result/3;
}