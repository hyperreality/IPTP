CFLAGS=-std=c99 -Wall -Wextra

ntru: IPTP/crypto/ntru.c
	$(CC) $(CFLAGS) -shared -fPIC -lntru -o IPTP/crypto/ntru.so IPTP/crypto/ntru.c

clean-pyc:
	find . -name '*.pyc' -exec rm --force {} +
	find . -name '*.pyo' -exec rm --force {} +
	find . -name '*~' -exec rm --force  {} +

test: clean-pyc
	cd IPTP && python3 -m unittest