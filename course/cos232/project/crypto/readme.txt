instructions for decrypting the files.

1. the passphrase.txt.gpg is encrypted via Dr. Stanley's public key and needs to be decrypted via his private key.

2. once you have the unencrypted passphrase, use gpg aes256 decrypt on the message.txt.gpg with the passphrase as the key.