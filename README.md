# Description

Hsh is a lightweight and versatile shell written in C. It is easy to edit and use.

# Instruction

1) Clone the repository
2) run Make command
3) Your executible is ready to go. Use it by typing ./hsh

# Assumptions

In seek function, if you try to seek for a file x, you would get the paths for x and any
file that starts with x. --> x(dot)
For example if you seek for file_name, you would get results as file_name, file_name.txt but not
file_name2.txt
Also in seek, I am assuming that '.' and '..' will not be the first argument.

In proclore, I am assuming that the state will have '+' if its pid matches with the terminal.

Length of maximum command name is assumed to be 256 characters.
Length of absolute path of a file is also assumed to be 256 characters.
Maximum characters in a line is assumed to be 4096 characters.
