Higher Order Countermeasures for AES and DES
======

We provide a C implementation of the anti-DPA countermeasure based on table recomputation described in [IACR ePrint Report 2013/700](http://eprint.iacr.org/2013/700).

The countermeasure is shown to be secure against a t-th order DPA attack, when the number of shares n is such that n>=2t+1.

The countermeasure is implemented for the DES and AES block-ciphers. For comparison we also implement the Rivain-Prouff countermeasure for AES, and the Carlet et al. countermeasure for DES.

What is implemented
-------------------

* AES without countermeasure
* DES without countermeasure
* AES with Rivain-Prouff countermeasure
* DES with Carlet et al. countermeasure, with the Parity-Split method for Sbox computation (requiring 10 non-linear multiplications), and the improved method from Roy-Vivek (requiring only 7 non-linear multiplications). The latter is used by default.
* AES with the new anti-DPA countermeasure based on table recomputation.
* DES with the new anti-DPA countermeasure based on table recomputation.

