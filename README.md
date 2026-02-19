# TechShell

**CSC 222 Systems Programming Project**

## Group Members

- Carter Landry
- Cole Pellegrin
- Edward Ware

## Overview

TechShell is a simplified Unix-like command line interpreter written in C. The program repeatedly prompts the user for input, parses the entered command, and executes it similarly to a standard shell such as Bash.

### Key Features

- Dynamic prompt showing the current working directory
- Execution of external programs using `fork()` and `exec()`
- Built-in commands handled directly by the shell
- Input (`<`) and output (`>`) redirection
- Error handling using standard system error reporting
- Demonstrates core systems programming concepts including process management, file descriptors, and OS interaction

## Implemented Features

- `touch` — Creates empty files
- `mkdir` — Creates directories  
- `rmdir` — Removes directories
- `cd` (alone) - returns to home directory

### Built-in Commands

| Command | Description |
|---------|-------------|
| `cd` | Changes the current directory. |
| `touch <filename>` | Creates an empty file if it does not exist. |
| `mkdir <dirname>` | Creates a new directory. |
| `rmdir <dirname>` | Removes an empty directory. |
| `exit` | Terminates the shell. |

### External Commands

Commands not implemented as built-ins are executed using:

- `fork()`
- `execvp()`
- `wait()`

**Examples:**

```
ls
ls -l
pwd
cat file.txt
```

### Input and Output Redirection

**Supported operators:**

- `command > file` — redirect standard output
- `command < file` — redirect standard input

**Examples:**

```
ls > output.txt
cat < input.txt
```

## Getting Started

### Compilation

```bash
gcc techshell.c -o techshell
```

### Running the Program

```bash
./techshell
```

To exit the shell, use the `exit` command.

## Project Structure

```
techshell.c    Main source file for the shell
README.md      Project documentation
```

## Known Limitations

The following features are not implemented:

- Pipe operator (`|`)
- Background execution (`&`)
- Command history
- Advanced parsing for quotes or escaped characters

All required functionality specified in the project prompt has been implemented.

## Testing

The shell has been tested with:

- Built-in commands
- Standard Unix commands
- File redirection scenarios
- Directory navigation
- Error conditions (invalid commands, missing files) with standard error reporting