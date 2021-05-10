#ifndef UTILS_H
#define UTILS_H

#include <RcppArmadillo.h>
struct EL {
  double nlogLR;
  arma::vec lambda;
  arma::vec gradient;
  int iterations;
  bool convergence;
};

Rcpp::NumericVector plog(Rcpp::NumericVector x, double threshold);
Rcpp::NumericVector dplog(Rcpp::NumericVector x, double threshold);
Rcpp::NumericVector d2plog(Rcpp::NumericVector x, double threshold);
EL getEL(const arma::mat& g,
         const int& maxit = 100,
         const double& abstol = 1e-8);
std::vector<std::vector<int>> all_pairs(const int& p);
arma::mat g_mean(const arma::vec& theta, arma::mat x);
arma::mat g_ibd(const arma::vec& theta, const arma::mat& x, const arma::mat& c);
arma::mat cov_ibd(const arma::mat& x, const arma::mat& c);
arma::vec linear_projection(const arma::vec& theta,
                            const arma::mat& L,
                            const arma::vec& rhs);
arma::vec lambda2theta_ibd(const arma::vec& lambda,
                           const arma::vec& theta,
                           const arma::mat& g,
                           const arma::mat& c,
                           const double& gamma);
double threshold_pairwise_ibd(const arma::mat& x,
                              const arma::mat& c,
                              const int& B,
                              const double& level);
#endif
