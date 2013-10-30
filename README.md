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

Notes
----
* We have not protected the key-schedule. Therefore we assume that the block-cipher initially receives the shares of the subkeys, instead of the shares of the key. Moreover we have not implemented the refresh of the key between executions; therefore the implementation would be secure only in a restricted model in which always the same intermediate variables are probed. To get security in the full model one would need to refresh the subkeys between executions. 

* We do not claim that in practice the implementation would be secure against a t-th order attack. Namely the implementation is only provided for illustrative purpose, and timing comparisons. Obtaining a secure implementation would require to carefully examine the assembly code. In particular one should make sure that no two shares of the same variable are stored in the same register.

