# nthash
Self contained C code to generate NTLMv1 password hash

nthash from Christian Hesse &lt;mail@eworm.de&gt;<br/>
https://github.com/eworm-de/nthash#nthash

md4 routines from Alexander Peslyak &lt;solar@openwall.com&gt; <br/>
http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4<br/>
so that there is no dependence on openssl or nettle etc

The orignal code has been hacked just to produce the password hashes.

# Usage
```
 nthash cleartext-password
 
 nthash <<<cleartext-password
 printf "cleartext-password" | nthash
 ```
