# Vernam CLI Tool

Perfect secrecy cipher algorithm with truly random key implemented in C

# Install
```sh
$ make
```

# Usage
```sh
$ ./vernam
# To encrypt/decrypt
$ vernam -i message_to_tom -k key-1 -o encrypted_to_tom
# To generate key
$ vernam -g <keys_count>
```

# Features
- Support command line arguments to encrypt
- Creation of thousands of key files at one time

# Contributions
Contributions are welcomed, feel free to open a pull request.

# License
This project is licensed under the GNU Public License v3.0. See [LICENSE](https://github.com/night0721/vernam/blob/master/LICENSE) for more information.
