% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{el_mean}
\alias{el_mean}
\title{Empirical likelihood test for mean}
\usage{
el_mean(theta, x, maxit = 100L, abstol = 1e-08)
}
\arguments{
\item{theta}{a vector of parameters to be tested.}

\item{x}{a matrix or vector of data. Each row is an observation vector.}

\item{maxit}{an optional value for the maximum number of iterations. Defaults to 50.}

\item{abstol}{an optional value for the absolute convergence tolerance. Defaults to 1e-8.}
}
\description{
Compute empirical likelihood for mean
}
