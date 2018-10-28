#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <libntru/ntru.h>

#define NTRU_PARAMS EES677EP1

void arr_to_hex(uint8_t *in, size_t insz, char *out) {
    const char *hex = "0123456789abcdef";

    uint8_t *pin = in;
    char *pout = out;

    for (; pin < in+insz; pout += 2, pin++) {
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
    }
}

void hex_to_arr(char *in, size_t insz, uint8_t *out) {
    char *pin = in;
    uint8_t *pout = out;

    for (; pin < in+insz; pout++, pin += 2) {
        sscanf(pin, "%2hhx", pout);
    }
}

int get_params(char *param_set) {
    struct params {
        struct NtruEncParams ntru_params; = NTRU_PARAMS;
        size_t pub_len; = ntru_pub_len(&params);
        size_t priv_len; = ntru_priv_len(&params);
        size_t enc_len; = ntru_enc_len(&params);
    }

    return 0;


}

int gen_keypair(char *pub_buf, char *priv_buf, char *err) {

    NtruRandGen rng_def = NTRU_RNG_DEFAULT;
    NtruRandContext rand_ctx_def;

    if (ntru_rand_init(&rand_ctx_def, &rng_def) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to initialise RNG");
        return -1;
    }

    NtruEncKeyPair kp;
    if (ntru_gen_key_pair(&params, &kp, &rand_ctx_def) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to generate NTRU keypair");
        return -1;
    }

    uint8_t pub_arr[pub_len], priv_arr[priv_len];
    ntru_export_pub(&kp.pub, pub_arr);
    ntru_export_priv(&kp.priv, priv_arr);

    arr_to_hex(pub_arr, pub_len, pub_buf);
    arr_to_hex(priv_arr, priv_len, priv_buf);

    /* uint8_t pub_arr2[pub_len]; */
    /* hex_to_arr(pub_buf, pub_len*2, pub_arr2); */
    /* arr_to_hex(pub_arr2, pub_len, pub_buf); */

    if (ntru_rand_release(&rand_ctx_def) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to release RNG resources");
        return -1;
    }

    return 0;
}

int encrypt(char *pub_str, uint8_t *msg, char *enc_buf, char *err) {
    struct NtruEncParams params = NTRU_PARAMS;
    size_t pub_len = ntru_pub_len(&params);
    size_t enc_len = ntru_enc_len(&params);
    /* size_t max_msg_len = ntru_max_msg_len(&params); */

    NtruRandGen rng_def = NTRU_RNG_DEFAULT;
    NtruRandContext rand_ctx_def;

    if (ntru_rand_init(&rand_ctx_def, &rng_def) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to initialise RNG");
        return -1;
    }

    NtruEncKeyPair kp;

    uint8_t pub_arr[pub_len];
    hex_to_arr(pub_str, pub_len*2, pub_arr);
    ntru_import_pub(pub_arr, &kp.pub);

    uint8_t enc[enc_len];
    if (ntru_encrypt(msg, strlen((char *)msg), &kp.pub, &params, &rand_ctx_def, enc) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to encrypt");
        return -1;
    }

    arr_to_hex(enc, enc_len, enc_buf);

    return 0;
}

int decrypt(char *pub_str, char *priv_str, char *enc, uint8_t *dec, char *err) {
    struct NtruEncParams params = NTRU_PARAMS;
    size_t pub_len = ntru_pub_len(&params);
    size_t priv_len = ntru_priv_len(&params);
    size_t enc_len = ntru_enc_len(&params);

    NtruEncKeyPair kp;

    uint8_t pub_arr[pub_len];
    hex_to_arr(pub_str, pub_len*2, pub_arr);
    ntru_import_pub(pub_arr, &kp.pub);

    uint8_t priv_arr[priv_len];
    hex_to_arr(priv_str, priv_len*2, priv_arr);
    ntru_import_priv(priv_arr, &kp.priv);

    uint8_t enc_arr[enc_len];
    hex_to_arr(enc, enc_len*2, enc_arr);

    /* uint8_t dec[ntru_max_msg_len(&params)]; */
    uint16_t dec_len;
    if (ntru_decrypt(enc_arr, &kp, &params, dec, &dec_len) != NTRU_SUCCESS) {
        strcpy(err, "NTRU: Failed to decrypt");
        return -1;
    }

    return 0;
}

