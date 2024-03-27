## ch3 proj1

---

### history_shell

This project consists of designing a C program to serve as a shell
interface that accepts user commands and then executes each
command in a separate process

#### start

- `gcc history_shell.c ; ./a.out`

#### run command

##### run any command on your computer

- `osh> cal`
- `osh> cal -3`

##### run history command

- `osh> !!` to run last command

##### pipe

- `osh> cal | grep Su` to run cal and as input to grep

##### redirect input and output

- `osh> cal > test.txt` to run cal and as input to wirte in test.txt
- `osh> grep Su < test.txt` to let test.txt as input of grep

#### screenshot
![shell](./img/shell.png)