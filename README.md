# RED_CRYPTO ver 0.5.0

## Description

Password manager for (almost) any QMK keyboards. 
Supports cryptographic algorithms:
- [x] Kuznechik (Russian GOST cipher)
- [x] SHA256 (for hashing key)
- [ ] Streebog (for hashing key, Russian GOST hash algorithm)
- [ ] AES256

## Features and plans

- [x] add read all ASCII-printable symbols for key
- [x] add Kuznechik lib for non 128 bit MCU
- [x] use SHA256 for hashing key to set key length to 32 bytes
- [ ] expand stored passwords length from 16 byte to 64
- [ ] expand number of stored passwords, may be with auto-adding new keycodes
- [ ] add CBC mode for Kuznechik, it will increase security

## Installation

1. Clone this repo to your keymap folder.

2. Copy this string to **rules.mk**. Don't forget to insert your names. This is make rules for including all needed file to firmwire.

```make
include keyboards/<your keyboard>/<path to your keymap folder>/red_crypto/red_crypto.mk 
```

3. Import library in **keymap.c**.

```c
#include "red_crypto/red_crypto.h"
```

4. Define chosen algorithms and parameters in `config.h`. Now you can use only `Kuznechik`. 

```c
#define USE_KUZNECHIK_8
```

If you want use SHA256 of your key to encrypt passwords, define this in `config.h`.

```c
#define USE_SHA256_KEY
```

Change storage size (default `4`) and maximum password length (default `64`). 

```c
#define STORAGE_SIZE 4
#define STORAGE_PASS_LEN 64 // use a multiple of 16, e.g. 16, 32, 64... but don't forget about available memory! 
```

5. Encrypt your passwords with chosen algorithm (AES, Kuznechik). Now available only Kuznechik. You can use `CyberChef`. 

> Don't use web version! We are here for security, right?

If you've enabled SHA256 for key, choose `SHA2`, then `SHA256` mode and `64` rounds. Hash your key, then copy it to next step.

Choose `GOST Encrypt`, then choose type `GOST R 34.12 (Kuznechik, 2015)`, not `Magma`. Choose mode `ECB`. Now there is no other modes for `Kuznechik`.

> Don't forget IV for GOST
 ```
 94208510C2C001FB01C0C21085209401
 ```

6. Add your encrypted passwords in **keymap.c**. Now you can encrypt 2 passwords with length <=16 byte. Keep this information in secret. Even if there are no plaintext. 

```c
const uint8_t encrypted_passwords[2][16] = { {0,0,0,...,0,0,0}, {0,0,0,...,0,0,0} };
```

7. Add new layer or set new keyes everywhere you want.

- `RED_TEST` - execute test functions of every algorithm with. Available only with `#define TEST_FUNCTIONS_ENABLED` and `COMMAND_ENABLE=yes`. You can see output via `hid_listen`.
- `RED_CRY_M` - execute reading password for encrypted passwords. After entering password, tap this key again. Now, you can use `RED_PASSX` keys.
- `RED_PASSX` - where `X` is number of your password in database. After tapping, keyboard send decrypted password.
- `RED_RNG` - use 32 times function named `tap_random_base64` from QMK. It's for generating random strings. But now random seed is not unique for every keyboard's power on.  

8. Copy this string to function **process_record_user** in **keymap.c** before all your code. It uses "switch case" construction inside. 

```c
    uint8_t crypto_return = crypto_process_record_user(keycode, event, encrypted_passwords);
    if (crypto_return == 1){
        return false;
    }
```

9. Compile and flash your keyboard.


## Usage

Tap `RED_CRY_M`. Enter password. Tap `RED_CRY_M` again to confirm.
Now you can use `RED_PASSX` to send decrypted passwords.

If you have missed while entering password, just tap `RED_CRY_M` and tap `RED_CRY_M`. 


## Related projects

Thanks to the authors of these repositories:

- https://github.com/mjosaarinen/kuznechik - kuznechik implementation for 8bit and 128bit
- https://github.com/B-Con/crypto-algorithms - other crypto algorithms