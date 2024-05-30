#include "random.h"

dlib::matrix<double> randn(int n, double scale) {		//n=199 or 100
	dlib::matrix<double> res(n, 1);
	std::default_random_engine e;
	e.seed(time(NULL));
	std::normal_distribution<double> dis(0, scale);		//normal_distributionÕýÌ¬·Ö²¼
	for (int i = 0; i < n; i++)
		res(i) = dis(e);
	return res;
}
