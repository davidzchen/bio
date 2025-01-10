# Interval Library

This library provides utilities for working with the [Interval][interval]
format, as well as algorithms for efficiently finding intervals that overlap
with a given query interval.

[interval]: https://info.gersteinlab.org/RSEQtools#Interval

The Interval format is a tab-delimitied text format with eight required columns:

1.   Name of the interval
2.   Chromosome
3.   Strand
4.   Interval start (with respect to the "+")
5.   Interval end (with respect to the "+")
6.   Number of sub-intervals
7.   Sub-interval starts (comma delimited, with respect to the "+")
8.   Sub-interval ends (comma delimited, with respect to the "+")

The coordinates in the Interval format are zero-based and the end coordinate is
not included.

The algorithm is based on the [Nested Containment Lists][nclists] by Alekseyenko
et al.

[nclists]: https://academic.oup.com/bioinformatics/article/23/11/1386/199545
