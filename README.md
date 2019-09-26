Higher Order Countermeasures for AES and DES
======

We provide a C implementation of the anti-DPA countermeasure based on table recomputation described in [Cor14], and its variants described in [CRZ17]. We also provide a C implementation of the Rivain-Prouff countermeasure for AES [RP10] and of the Carlet et al. countermeasure [CGP+12] implemented with the Coron-Roy-Vivek technique [CRV14].

These countermeasures are shown to be secure against a t-th order DPA attack, when the number of shares n is such that n>=2t+1 (or n>=t+1 in some cases).

The countermeasure are implemented for the DES and AES block-ciphers.


What is implemented
-------------------

* AES without countermeasure
* AES with the Rivain-Prouff countermeasure [RP10]
* AES with the table recomputation countermeasure and its variants. [Cor14]
* AES with various PRG constructions. 
* DES without countermeasure
* DES with Carlet et al. countermeasure, with the Parity-Split method for Sbox computation (requiring 10 non-linear multiplications) [CGP+12]
* DES with the improved method from Roy-Vivek (requiring 7 non-linear multiplications).
* DES with the [CRV14] method (requiring only 4 non-linear multiplications). 
* DES with the table recomputation countermeasure and its variants. [Cor14,CRZ17]

Notes
----
* We have not protected the key-schedule. Therefore we assume that the block-cipher initially receives the shares of the subkeys, instead of the shares of the key. Moreover we have not implemented the refresh of the key between executions; therefore the implementation would be secure only in a restricted model in which always the same intermediate variables are probed. To get security in the full model one would need to refresh the subkeys between executions. 

* We do not claim that in practice the implementation would be secure against a t-th order attack. Namely the implementation is only provided for illustrative purpose, and timing comparisons. Obtaining a secure implementation would require to carefully examine the assembly code. In particular one should make sure that no two shares of the same variable are stored in the same register.

References
----------

[Cor14] Jean-Sébastien Coron. Higher order masking of look-up tables. In Advances in Cryptology - EUROCRYPT 2014 - 33rd Annual International Conference on the Theory and Applications of Cryptographic Techniques, Copenhagen, Denmark, May 11-15, 2014. Proceedings, pages 441–458, 2014.

[RP10] Matthieu Rivain and Emmanuel Prouff. Provably secure higher-order masking of AES. In CHES, pages 413–427, 2010.

[CGP+12] Claude Carlet, Louis Goubin, Emmanuel Prouff, Michaël Quisquater, and Matthieu Rivain. Higher-order masking schemes for s-boxes. In FSE, pages 366–384, 2012.

[CRV14] Jean-Sébastien Coron, Arnab Roy, and Srinivas Vivek. Fast evaluation of polynomials over binary finite fields and application to side-channel counter- measures. In Cryptographic Hardware and Embedded Systems - CHES 2014 - 16th International Workshop, Busan, South Korea, September 23-26, 2014. Proceedings, pages 170–187, 2014.

[CRZ17] Jean-Sébastien Coron, Franck Rondepierre, Rina Zeitoun:
High Order Masking of Look-up Tables with Common Shares. IACR Cryptology ePrint Archive 2017: 271 (2017).