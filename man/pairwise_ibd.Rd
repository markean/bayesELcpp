% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{pairwise_ibd}
\alias{pairwise_ibd}
\title{Pairwise comparison for Incomplete Block Design}
\usage{
pairwise_ibd(
  x,
  c,
  interval = FALSE,
  B = 10000L,
  level = 0.05,
  method = "PB",
  correction = FALSE,
  approx_lambda = FALSE,
  ncores = 1L,
  maxit = 10000L,
  abstol = 1e-08
)
}
\arguments{
\item{x}{a matrix of data .}

\item{c}{an incidence matrix.}

\item{interval}{whether to compute interval. Defaults to FALSE.}

\item{B}{number of bootstrap replicates.}

\item{level}{level.}

\item{method}{the method to be used; either 'PB' or 'NB' is supported. Defaults to 'PB'.}

\item{correction}{whether to use blocked bootstrap. Defaults to FALSE.}

\item{approx_lambda}{whether to use the approximation for lambda. Defaults to FALSE.}

\item{ncores}{number of cores(threads) to use. Defaults to 1.}

\item{maxit}{an optional value for the maximum number of iterations. Defaults to 1000.}

\item{abstol}{an optional value for the absolute convergence tolerance. Defaults to 1e-8.}
}
\description{
Pairwise comparison for Incomplete Block Design
}
