FT_SSL:
A miniature implementation of openssl implementing several features, namely:
- md5
- sha-256
- base64
- des-ecb
- des-cbC
- des-pcbc
- des3-ecb
- des3-cbc
- des3-pcbc
- scrypt

USAGE:
```
echo "We can encode base64" | ./ft_ssl base64
echo "And decode it" | ./ft_ssl base64 | ./ft_ssl base64 -d

export DES_KEY="C0FFEE69DEADBEEF"
echo "We can encrypt with DES3" | ./ft_ssl des3 -k "$DES_KEY" -a -o test.txt
# and decrypt it
./ft_ssl des3 -a -k "$DES_KEY" -i test.txt

./ft_ssl des3 -p "we can also generate secure keys using scrypt"
```

Disclaimers:
This project was created for educational use only, and is not intended for use in secure commincations. This project does not protect against any side-channel attacks or employ methods of securely generating cryptographic messages, suchs as overwriting stack memory or key values in memory. Additionally, input size is implicitly limited to < 2gb.


REFERENCES:

- http://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm
- https://csrc.nist.gov/csrc/media/publications/fips/81/archive/1980-12-02/documents/fips81.pdf
- https://csrc.nist.gov/csrc/media/publications/fips/46/3/archive/1999-10-25/documents/fips46-3.pdf
- https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
- https://tools.ietf.org/html/rfc4648#page-5
- https://github.com/monolifed/pbkdf2-hmac-sha256/blob/master/pbkdf2_hmac_sha256.c
- https://github.com/sclolus/ft_ssl_md5/blob/27bd47c609e5534810f36f3435fd1932c61b7c92/srcs/bit_permutation.c
- https://crypto.stackexchange.com/questions/9674/how-does-des-decryption-work-is-it-the-same-as-encryption-or-the-reverse
- https://en.wikipedia.org/wiki/DES_supplementary_material
- https://en.wikipedia.org/wiki/S-box
- https://www.di-mgt.com.au/cryptopad.html
