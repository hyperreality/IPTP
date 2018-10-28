"""
Bindings to access the NTRU C shared object
"""

import ctypes

_ntru = ctypes.CDLL('crypto/ntru.so')

# Temporarily hardcoded values for convenience's sake
PUBKEYSIZE = 2*935
PRIVKEYSIZE = 2*402
CIPHERTEXTSIZE = 2*931
MAXMSGSIZE = 101
ERRSIZE = 100

def gen_keypair():
    pub_buf = ctypes.create_string_buffer(PUBKEYSIZE)
    priv_buf = ctypes.create_string_buffer(PRIVKEYSIZE)
    error = ctypes.create_string_buffer(ERRSIZE)

    ret = _ntru.gen_keypair(pub_buf, priv_buf, error)
    if int(ret) != 0:
        raise Exception(error.value)

    return {'pubkey': pub_buf.value, 'privkey': priv_buf.value}

def encrypt(pubkey, message):
    pub_str = ctypes.create_string_buffer(pubkey, PUBKEYSIZE)
    msg_str = ctypes.create_string_buffer(message, MAXMSGSIZE)
    enc_buf = ctypes.create_string_buffer(CIPHERTEXTSIZE)
    error = ctypes.create_string_buffer(ERRSIZE)

    ret = _ntru.encrypt(pub_str, msg_str, enc_buf, error)
    if int(ret) != 0:
        raise Exception(error.value)

    return enc_buf.value

def decrypt(pubkey, privkey, encrypted):
    pub_str = ctypes.create_string_buffer(pubkey, PUBKEYSIZE)
    priv_str = ctypes.create_string_buffer(privkey, PRIVKEYSIZE)
    enc = ctypes.create_string_buffer(encrypted, CIPHERTEXTSIZE)
    dec = ctypes.create_string_buffer(MAXMSGSIZE)
    error = ctypes.create_string_buffer(ERRSIZE)

    ret = _ntru.decrypt(pub_str, priv_str, enc, dec, error)
    if int(ret) != 0:
        raise Exception(error.value)

    return dec.value

keys = gen_keypair()
encrypted = encrypt(keys['pubkey'], b"data_to_encrypt")
print(encrypted)
print(decrypt(keys['pubkey'], keys['privkey'], encrypted))

