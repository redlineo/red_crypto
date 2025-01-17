# RED_CRYPTO ver 0.2.0

## Description

Password manager for (almost) any QMK keyboards. 
Supports cryptographic algorithms:
- [ ] AES256,
- [x] Kuznechik (Russian GOST cipher),
- [ ] SHA256. 

## Installation

1. Clone this repo to your keymap folder.

2. Copy this string to *rules.mk*. Don't forget to insert your names. This is make rules for including all needed file to firmwire.

```make
include keyboards/<keyboard>/<path to your keymap folder>/red_crypto/red_crypto.mk 
```

3. Import library in *keymap.c*.

```c
#include "red_crypto/red_crypto.h"
```

4. Enable chosen algorithms. Now you can use only `Kuznechik`. 

```c
#define USE_KUZNECHIK_8
```

5. Encrypt your passwords with chosen algorithm (AES, Kuznechik). Now available only Kuznechik. You can use `CyberChef`. Don't use web version! We are here for security, right? Choose `GOST Cipher`, then choose type `Kuznechik 34.12-2014`, not Magma. Choose mode ECB. Now, there is no CBC or CTR. 

> Don't forget IV for GOST, get it from kuznechik_8bit.c.

6. Add your encrypted passwords in "keymap.c". Now you can encrypt 2 passwords with length <=16 byte. Keep this information in secret. Even if there are no plaintext. 

```c
const uint8_t encrypted_passwords[2][16] = { {0,0,0,...,0,0,0}, {0,0,0,...,0,0,0} };
```

7. Add new layer or set new keyes everywhere you want.

- `RED_TEST` - execute test functions of every algorithm with. Available only with `#define TEST_FUNCTIONS_ENABLED` and `COMMAND_ENABLE=yes`. You can see output via `hid_listen`.
- `GET_PASS` - execute reading password for encrypted passwords. After entering password, tap this key again. Now, you can use `RED_PASSX` keys.
- `RED_PASSX` - where `X` is number of your password in database. After tapping, keyboard send decrypted password.
- `RED_RNG` - use 32 times function named `tap_random_base64` from QMK. It's for generating random strings. But now random seed is not unique for every keyboard's power on.  

8. Copy this string to function *process_record_user* in *keymap.c* before all your code. It uses "switch case" construction inside. 

```c
    crypto_process_record_user(keycode,event,encrypted_passwords);
```

9. Flash your keyboard.


## Usage

Tap `GET_PASS`. Enter password. Tap `GET_PASS` again to confirm.
Now you can use `RED_PASSX` to send decrypted passwords.

If you have missed while entering password, just tap `GET_PASS` and tap `GET_PASS`. 
