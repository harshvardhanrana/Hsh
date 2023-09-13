[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

Hash is a lightweight and versatile shell written in C. It is easy to edit and use.

# Instruction

1) Clone the repository
2) run Make command
3) Your executible is ready to go. Use it by typing ./a.out

# Assumptions

In seek function, if you try to seek for a file x, you would get the paths for x and any
file that starts with x. --> x(dot)
For example if you seek for file_name, you would get results as file_name, file_name.txt but not
file_name2.txt
Also in seek, I am assuming that '.' and '..' will not be the first argument.

In proclore, I am assuming that the state will have '+' if its pid matches with the terminal.

If your previous command was echo 5 and you executed pastevents execute 1; then it would be saved as echo 5; and therefore it would be a new command. If it is just pastevents 1 it wont be saved as it will be echo 5 which is the same as previous command.

Length of maximum command name is assumed to be 256 characters.
Length of absolute path of a file is also assumed to be 256 characters.
Maximum characters in a line is assumed to be 4096 characters.

