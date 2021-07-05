#include "utils.h"

double plog_sum(Eigen::VectorXd&& x) {
  const double n = static_cast<double>(x.size());
  const double a1 = -std::log(n) - 1.5;
  const double a2 = 2.0 * n;
  const double a3 = -0.5 * n * n;
  double out = 0;
  for (unsigned int i = 0; i < x.size(); ++i) {
    out += n * x[i] < 1.0 ? a1 + a2 * x[i] + a3 * x[i] * x[i] : std::log(x[i]);
  }
  return out;
}
Eigen::ArrayXd dplog(Eigen::VectorXd&& x) {
  const double n = static_cast<double>(x.size());
  const double a1 = 2.0 * n;
  const double a2 = -1.0 * n * n;
  // Eigen::ArrayXd out(n);
  for (unsigned int i = 0; i < x.size(); ++i) {
    if (n * x[i] < 1.0) {
      x[i] = a1 + a2 * x[i];
    } else {
      x[i] = 1.0 / x[i];
    }
  }
  return x;
}
// Eigen::ArrayXd sqrt_neg_d2plog(const Eigen::Ref<const Eigen::VectorXd>& x) {
//   const unsigned int n = x.size();
//   const double a2 = 1.0 * n;
//   // const double a2 = 1.0 * n * n;
//   Eigen::ArrayXd out(n);
//   for (unsigned int i = 0; i < n; ++i) {
//     if (n * x[i] < 1) {
//       out[i] = a2;
//     } else {
//       out[i] = 1.0 / x[i];
//     }
//   }
//   return out;
// }

TEST::TEST(const Eigen::Ref<const Eigen::MatrixXd>& g,
           const int maxit,
           const double abstol) {
  // maximization
  Eigen::VectorXd lambda_t = Eigen::VectorXd::Zero(g.cols());
  double f1;
  int iterations_t = 0;
  bool convergence_t = false;
  while (!convergence_t && iterations_t != maxit) {
    // plog class
    PSEUDO_LOG log_tmp(Eigen::VectorXd::Ones(g.rows()) + g * lambda_t);
    // J matrix
    const Eigen::MatrixXd J = g.array().colwise() * log_tmp.sqrt_neg_d2plog;
    // prpose new lambda by NR method with least square
    Eigen::VectorXd step =
      (J.transpose() * J).ldlt().solve(
          J.transpose() * (log_tmp.dplog / log_tmp.sqrt_neg_d2plog).matrix());
    // update function value
    f1 = plog_sum(Eigen::VectorXd::Ones(g.rows()) + g * (lambda_t + step));
    // step halving to ensure validity
    while (f1 < log_tmp.plog_sum) {
      step /= 2;
      f1 = plog_sum(Eigen::VectorXd::Ones(g.rows()) + g * (lambda_t + step));
    }
    // update lambda
    lambda_t += step;
    // convergence check
    if (f1 - log_tmp.plog_sum < abstol) {
      convergence_t = true;
    } else {
      ++iterations_t;
    }
  }

  nlogLR = f1;
  lambda = lambda_t;
  iterations = iterations_t;
  convergence = convergence_t;
}

PSEUDO_LOG::PSEUDO_LOG(const Eigen::Ref<const Eigen::VectorXd>& x) {
  const double n = static_cast<double>(x.size());
  const double a1 = -std::log(n) - 1.5;
  const double a2 = 2.0 * n;
  const double a3 = -0.5 * n * n;

  double plog_sum_t = 0;
  Eigen::ArrayXd dplog_t(x.size());
  Eigen::ArrayXd sqrt_neg_d2plog_t(x.size());

  for (unsigned int i = 0; i < x.size(); ++i) {
    if (n * x[i] < 1.0) {
      plog_sum_t += a1 + a2 * x[i] + a3 * x[i] * x[i];
      dplog_t[i] = a2 + 2 * a3 * x[i];
      sqrt_neg_d2plog_t[i] = a2 / 2;
    } else {
      plog_sum_t += std::log(x[i]);
      dplog_t[i] = 1.0 / x[i];
      sqrt_neg_d2plog_t[i] = 1.0 / x[i];
    }
  }

  plog_sum = plog_sum_t;
  dplog = dplog_t;
  sqrt_neg_d2plog = sqrt_neg_d2plog_t;
}

EL getEL(const Eigen::Ref<const Eigen::MatrixXd>& g,
         const int maxit,
         const double abstol) {
  // maximization
  Eigen::VectorXd lambda = Eigen::VectorXd::Zero(g.cols());
  double f1;
  int iterations = 0;
  bool convergence = false;
  while (!convergence && iterations != maxit) {
    // plog class
    PSEUDO_LOG log_tmp(Eigen::VectorXd::Ones(g.rows()) + g * lambda);

    // // function evaluation
    // const double f0 = log_tmp.plog_sum;

    // J matrix
    const Eigen::MatrixXd J = g.array().colwise() * log_tmp.sqrt_neg_d2plog;

    // // J matrix & y vector
    // Eigen::ArrayXd v1 = log_tmp.sqrt_neg_d2plog;
    // Eigen::ArrayXd v2 = log_tmp.dplog;
    // Eigen::MatrixXd J = g.array().colwise() * v1;
    /// Eigen::VectorXd y = v2 / v1;

    // prpose new lambda by NR method with least square
    Eigen::VectorXd&& step =
      (J.transpose() * J).ldlt().solve(
          J.transpose() * (log_tmp.dplog / log_tmp.sqrt_neg_d2plog).matrix());

    // update function value
    f1 = plog_sum(Eigen::VectorXd::Ones(g.rows()) + g * (lambda + step));

    // step halving to ensure validity
    while (f1 < log_tmp.plog_sum) {
      step /= 2;
      f1 = plog_sum(Eigen::VectorXd::Ones(g.rows()) + g * (lambda + step));
    }

    // update lambda
    lambda += step;

    // convergence check
    if (f1 - log_tmp.plog_sum < abstol) {
      // Eigen::ArrayXd v1 = log_tmp.sqrt_neg_d2plog;
      // Eigen::ArrayXd v2 = log_tmp.dplog;
      // Eigen::MatrixXd J = g.array().colwise() * v1;
      // Eigen::VectorXd y = v2 / v1;
      convergence = true;
    } else {
      ++iterations;
    }
  }

  return {f1, lambda, iterations, convergence};
}

std::vector<std::array<int, 2>> all_pairs(const int p) {
  // initialize a vector of vectors
  std::vector<std::array<int, 2>> pairs;
  // the size of vector is p choose 2
  pairs.reserve(p * (p - 1) / 2);
  // fill in each elements(pairs)
  for (int i = 1; i < p + 1; ++i) {
    for (int j = i + 1; j < p + 1; ++j) {
      pairs.emplace_back(std::array<int, 2>{j, i});
      // pairs.emplace_back(std::vector<int> {i, j});
    }
  }
  return pairs;
}

Eigen::VectorXd linear_projection(
    const Eigen::Ref<const Eigen::VectorXd>& theta,
    const Eigen::Ref<const Eigen::MatrixXd>& lhs,
    const Eigen::Ref<const Eigen::VectorXd>& rhs) {
  return theta -
    lhs.transpose() * (lhs * lhs.transpose()).inverse() * (lhs * theta - rhs);
}
Eigen::VectorXd linear_projection_rref(
    Eigen::VectorXd&& theta,
    const Eigen::Ref<const Eigen::MatrixXd>& lhs,
    const Eigen::Ref<const Eigen::VectorXd>& rhs) {
  return theta -
    lhs.transpose() * (lhs * lhs.transpose()).inverse() * (lhs * theta - rhs);
}
// void linear_projection2(Eigen::Ref<Eigen::VectorXd> theta,
//                         const Eigen::Ref<const Eigen::MatrixXd>& lhs,
//                         const Eigen::Ref<const Eigen::VectorXd>& rhs) {
//   theta -=
//     lhs.transpose() * (lhs * lhs.transpose()).inverse() * (lhs * theta - rhs);
// }

Eigen::MatrixXd bootstrap_sample(const Eigen::Ref<const Eigen::MatrixXd>& x,
                                 const Eigen::Ref<const Eigen::ArrayXi>& index) {
  Eigen::MatrixXd out(x.rows(), x.cols());
  for (int i = 0; i < x.rows(); ++i) {
    out.row(i) = x.row(index(i));
  }
  return out;
}





// these are tmp
Eigen::RowVectorXd rowset(const Eigen::Ref<const Eigen::RowVectorXd>& x,
                          const Eigen::Ref<const Eigen::RowVectorXd>& c) {
  // first select nonzero elements from x
  std::vector<double> nonzeros;
  for (int j = 0; j < x.size(); ++j) {
    if (x[j] != 0) {
      nonzeros.push_back(x[j]);
    }
  }
  Eigen::RowVectorXd result = c;
  std::vector<double>::iterator it = nonzeros.begin();
  for (int j = 0; j < x.size(); ++j) {
    if (result[j] != 0) {
      result[j] = nonzeros[0];
      nonzeros.erase(it);
    }
  }
  return result;
}
Eigen::MatrixXd rowsetmat(const Eigen::Ref<const Eigen::MatrixXd>& x,
                          const Eigen::Ref<const Eigen::MatrixXd>& c) {
  Eigen::MatrixXd out(x.rows(), x.cols());
  for (int i = 0; i < x.rows(); ++i) {
    out.row(i) = rowset(x.row(i), c.row(i));
  }
  return out;
}




