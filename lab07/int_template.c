#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10
#define NaN NAN

// pointer to function of one variable
// typedef ... Func1vFp ...;
typedef double (*Func1vFp)(double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
	return (double)(2*pow(x,5)-4*pow(x,4)+3.5*pow(x,2)+1.35*x-6.25);
}

double f_rat(double x) {
	return (double)(1/(pow(x-0.5,2)+0.01));
}

double f_exp(double x) {
	return (double)(2*x*exp(-1.5*x)-1);
}

double f_trig(double x) {
	return (double)(x*tan(x));
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
	double p=(b-a)/(double) n;
	double sum=0.0;
	double left=a,right=p;
	for (int i=1;i<=n;i++){
		right=(double)i*p+a;
		left=a+(double)(i-1)*p;
		sum+=((double)(right-left)*f1(left));
	}

	return sum;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
	double p=(b-a)/(double) n;
	double sum=0.0;
	double left=a,right=p;
	for (int i=1;i<=n;i++){
		right=(double)i*p+a;
		left=a+(double)(i-1)*p;
		sum+=((double)(right-left)*f1(right));
	}

	return sum;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
	double p=(b-a)/(double) n;
	double sum=0.0;
	double left=a,right=p;
	for (int i=1;i<=n;i++){
		right=(double)i*p+a;
		left=a+(double)(i-1)*p;
		sum+=((double)(right-left)*f1((left+right)/2.0));
	}

	return sum;
	// return h * sum;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
	double p=(b-a)/(double) n;
	double sum=0.0;
	double left=a,right=p;
	for (int i=1;i<=n;i++){
		right=(double)i*p+a;
		left=a+(double)(i-1)*p;
		sum+=((double)(right-left)/2.0 *(func(left)+func(right)));
	}

	return sum;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
	double p=(b-a)/(double) n;
	double sum=0.0;
	double left=a,right=p;
	for (int i=1;i<=n;i++){
		right=(double)i*p+a;
		left=a+(double)(i-1)*p;
		sum+=((double)((right-left)/6.0) *(double)(f(left)+4.0*f((left+right)/2.0)+f(right)));
	}

	return sum;
}

// pointer to quadrature function
// typedef ... QuadratureFp ...;
typedef double (*QuadratureFp)();

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    Func1vFp func = func_tab[fun_no];

    QuadratureFp quad = quad_tab[quad_no];

    return quad(func, a, b, n);
}


// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
	double c = (a + b) / 2.0;   
    double S1 = quad(f, a, c, 1);   
    double S2 = quad(f, c, b, 1);   
    double err = fabs(S - (S1 + S2));   
    
    if (err <= delta) {
        return S1 + S2;  
    }
    else if (level == RECURS_LEVEL_MAX) {
        return NaN;   
    }
    else {
        double S1_recurs = recurs(f, a, c, S1, delta/2.0, quad, level+1);   
        double S2_recurs = recurs(f, c, b, S2, delta/2.0, quad, level+1);   
        return S1_recurs + S2_recurs;   
    }
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
	double S = quad(f, a, b, 1); 
    return recurs(f,a,b,S,delta,quad,1);
}

// double integrals

// pointer to function of two variables
// typedef ... Func2vFp ...;
typedef double (*Func2vFp)(double x, double y);


double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}


// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
	double hx = (x2 - x1) / nx;
    double hy = (y2 - y1) / ny;
    double result = 0.0;
	double xi = x1;
	
    for (int i = 0; i < nx; i++) {
		double yj = y1;
        for (int j = 0; j < ny; j++) {
            double fval = f(xi, yj);
            result += fval;
			yj+=hy;
        }
		xi+=hx;
    }

    return result*hx*hy;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {
	double hx = (x2 - x1) / nx;
    double result = 0.0;
    double xi = x1+0.5*hx;
    double sub_hy;

    for (int i = 0; i < nx; i++) {
        int ny = ceil((fh(xi) - fg(xi)) / hy);
        sub_hy = (fh(xi) - fg(xi)) / ny;
        double y1 = fg(xi);
        double y2 = y1 + sub_hy;
        double val = 0.0;
        for (int j = 0; j < ny; j++) {
            val += f(xi, (y1 + y2) / 2.0);
            y1 += sub_hy;
            y2 += sub_hy;
        }
        val *= sub_hy;
        result += val;
        xi += hx;
    }

    return result * hx;
}



// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh){
	double hx = (x2 - x1) / nx, hy = (y2 - y1) / ny, sum = 0, xi, gi, hi, yi;

    xi = x1;
    for(int i = 0; i < nx; i++) {
        gi = fg(xi);
        hi = fh(xi);
        double upBound = fmin(y2, hi);
        double downBound = fmax(y1, gi);

        int currNy = ceil((upBound - downBound) / hy);
        double currHy = (upBound - downBound) / currNy;

        yi = fmax(y1, gi);
        for (int j = 0; j < currNy; j++) {
            sum += currHy * f(xi, yi);
            yi += currHy;
        }
        xi += hx;
    }
    return hx * sum;
}


// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
	double h[3], x[3], xi[3], val = 0.0;
    int i, j, k, flag;
    for (i = 0; i < 3; i++) {
        h[i] = (variable_lim[i][1] - variable_lim[i][0]) / tn[i];
    }
	xi[0] = variable_lim[0][0]+h[0];
    for (i = 0; i < tn[0]; i++) {
		xi[1] = variable_lim[1][0]+h[1];
        for (j = 0; j < tn[1]; j++) {
			xi[2] = variable_lim[2][0]+h[2];
            for (k = 0; k < tn[2]; k++) {
                flag = 1;
                if (boundary != NULL) {
                    flag = boundary(xi,3);
                }
                if (flag) {
                    x[0] = xi[0]; x[1] = xi[1]; x[2] = xi[2];
                    val += f(x,3) ;
                }
				xi[2]+= h[2];
            }
			xi[1]+= h[1];
        }
		xi[0]+= h[0];
    }
    return val* h[0] * h[1] * h[2];
}


// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[], double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) {
	if (level == variable_no) {
		int flag = 1;
		if (boundary != NULL) {
			flag = boundary(t_variable, variable_no);
		}
		if (flag) {
			double val = f(t_variable, variable_no);
			for (int i = 0; i < variable_no; i++) {
				val *= ((variable_lim[i][1] - variable_lim[i][0]) / tn[i]);
			}
			*p_sum += val;
		}
	}
	else {
		t_variable[level] = variable_lim[level][0] + 0.5 * ((variable_lim[level][1] - variable_lim[level][0]) / tn[level]);
		for (int i = 0; i < tn[level]; i++) {
			recur_quad_rect_mid(p_sum, f, variable_no, t_variable, variable_lim, tn, level + 1, boundary);
			t_variable[level] += ((variable_lim[level][1] - variable_lim[level][0]) / tn[level]);
		}
	}
}


int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.0;
			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}

